#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <cassert>
#include <common/CMRDebug.h>
#include <communication/CMRMPIComm.h>
#include <MockAbstractDomain.h>
#include <communication/CMRMPICommFactory.h>
#include <communication/CMRCommSchem.h>
#include <domain/CMRDomainStorage.h>
#include <common/CMRSpaceSplitter.h>
#include <CMROperation.h>
#include <domain/CMRMemoryModels.h>
#include <domain/CMRCellAccessor.h>
#include <domain/CMRVarSystem.h>
#include <domain/CMRMPIDomainBuilder.h>
#include <domain/CMRMemoryModels.h>

using namespace std;

#define DIRECTIONS 9
#define DIMENSIONS 2
#define RELAX_PARAMETER 1
//=>(lbm_gbl_config.relax_parameter)
#define INFLOW_MAX_VELOCITY 1
//=>(lbm_gbl_config.inflow_max_velocity)

/** Représentation d'un vecteur pour la manipulation des vitesses macroscopiques. **/
typedef double LBMVect[DIMENSIONS];

/*********************  ENUM  ***********************/
/**
 * Definition des différents type de cellule pour savoir quel traitement y appliquer
 * lors du calcule.
**/
enum LBMCellType
{
	/** Cellule de fluide standard, uniquement application des collisions. **/
	CELL_FUILD,
	/** Cellules de l'obstacle ou des bordure supérieures et inférieurs. Application de réflexion. **/
	CELL_BOUNCE_BACK,
	/** Cellule de la paroie d'entrée. Application de Zou/He avec V fixé. **/
	CELL_LEFT_IN,
	/** Cellule de la paroie de sortie. Application de Zou/He avec gradiant de densité constant. **/
	CELL_RIGHT_OUT
};

/********************** CONSTS **********************/
/**
 * Definitions des 9 vecteurs de base utilisé pour discrétiser les directions sur chaque mailles.
**/
#if DIRECTIONS == 9 && DIMENSIONS == 2
const LBMVect direction_matrix[DIRECTIONS] = {
	{+0.0,+0.0},
	{+1.0,+0.0}, {+0.0,+1.0}, {-1.0,+0.0}, {+0.0,-1.0},
	{+1.0,+1.0}, {-1.0,+1.0}, {-1.0,-1.0}, {+1.0,-1.0}
};
#else
#error Need to defined adapted direction matrix.
#endif

/********************** CONSTS **********************/
/**
 * Poids utilisé pour compenser les différentes de longueur des 9 vecteurs directions.
**/
#if DIRECTIONS == 9
const double equil_weight[DIRECTIONS] = {
	4.0/9.0 ,
	1.0/9.0 , 1.0/9.0 , 1.0/9.0 , 1.0/9.0,
	1.0/36.0, 1.0/36.0, 1.0/36.0, 1.0/36.0
};
//opposite directions, for bounce back implementation
const int opposite_of[DIRECTIONS] = { 0, 3, 4, 1, 2, 7, 8, 5, 6 };
#else
#error Need to defined adapted equibirium distribution function
#endif

class VarSystem : public CMRVarSystem
{
	public:
		struct CellAccessor
		{
			CellAccessor(CMRVarSystem & sys,int tstep,int x,int y,bool absolute = true);
			CellAccessor(CellAccessor & acc,int x,int y,bool absolute = false);
			CMRCellAccessor<float[DIRECTIONS],CMRMemoryModelRowMajor> directions;
			CMRCellAccessor<LBMCellType,CMRMemoryModelRowMajor> cellType;
		};
		VarSystem(CMRDomainBuilder * builder);
};

VarSystem::VarSystem ( CMRDomainBuilder * builder)
:CMRVarSystem(builder)
{
	this->addVariable("directions",sizeof(float[DIRECTIONS]),1);
	this->addVariable("cellType",sizeof(LBMCellType),1);
}



VarSystem::CellAccessor::CellAccessor (CMRVarSystem & sys,int tstep,int x,int y,bool absolute)
	:directions(*(sys.getDomain(0,tstep)),x,y,absolute),cellType(*sys.getDomain(1,tstep),x,y,absolute)
{
	
}

VarSystem::CellAccessor::CellAccessor ( CellAccessor& acc, int x, int y,bool absolute)
	:directions(acc.directions,x,y,absolute),cellType(acc.cellType,x,y,absolute)
{
}

/*******************  FUNCTION  *********************/
/**
 * Renvoie le résultat du produit des deux vecteurs passés en paramêtre.
**/
double get_vect_norme_2(const LBMVect vect1,const LBMVect vect2)
{
	//vars
	int k;
	double res = 0.0;

	//loop on dimensions
	for ( k = 0 ; k < DIMENSIONS ; k++)
		res += vect1[k] * vect2[k];

	return res;
}

/*******************  FUNCTION  *********************/
/**
 * Calcule la densité macroscopiques de la cellule en sommant ses DIRECTIONS
 * densités microscopiques.
**/
double get_cell_density(const VarSystem::CellAccessor & in,int x,int y)
{
	//vars
	int k;
	double res = 0.0;

	//loop on directions
	for( k = 0 ; k < DIRECTIONS ; k++)
		res += (*in.directions(x,y))[k];

	//return res
	return res;
}

/*******************  FUNCTION  *********************/
/**
 * Calcule la vitesse macroscopiques de la cellule en sommant ses DIRECTIONS
 * densités microscopiques.
 * @param cell_density Densité macroscopique de la cellules.
**/
void get_cell_velocity(LBMVect v,const VarSystem::CellAccessor & in,double cell_density,int x,int y)
{
	//vars
	int k,d;

	//loop on all dimensions
	for ( d = 0 ; d < DIMENSIONS ; d++)
	{
		//reset value
		v[d] = 0.0;

		//sum all directions
		for ( k = 0 ; k < DIRECTIONS ; k++)
			v[d] += (*in.directions(x,y))[k] * direction_matrix[k][d];

		//normalize
		v[d] = v[d] / cell_density;
	}
}

/*******************  FUNCTION  *********************/
/**
 * Calcule le profile de densité microscopique à l'équilibre pour une direction donnée.
 * C'est ici qu'intervient un dérivé de navier-stokes.
 * @param velocity Vitesse macroscopique du fluide sur la maille.
 * @param density Densité macroscopique du fluide sur la maille.
 * @param direction Direction pour laquelle calculer l'état d'équilibre.
**/
double compute_equilibrium_profile(LBMVect velocity,double density,int direction)
{
	//vars
	double v2;
	double p;
	double p2;
	double feq;

	//velocity norme 2 (v * v)
	v2 = get_vect_norme_2(velocity,velocity);

	//calc e_i * v_i / c
	p = get_vect_norme_2(direction_matrix[direction],velocity);
	p2 = p * p;

	//terms without density and direction weight
	feq = 1.0
		+ (3.0 * p)
		+ ((9.0 / 2.0) * p2)
		- ((3.0 / 2.0) * v2);

	//mult all by density and direction weight
	feq *= equil_weight[direction] * density;

	return feq;
}

/*******************  FUNCTION  *********************/
/**
 * Applique une reflexion sur les différentes directions pour simuler la présence d'un solide.
**/
void compute_bounce_back(const VarSystem::CellAccessor & in,VarSystem::CellAccessor & out,int x,int y)
{
	//vars
	int k;
	double tmp[DIRECTIONS];

	//compute bounce back
	for ( k = 0 ; k < DIRECTIONS ; k++)
		tmp[k] = (*in.directions(x,y))[opposite_of[k]];

	//compute bounce back
	for ( k = 0 ; k < DIRECTIONS ; k++)
		(*out.directions(x,y))[k] = tmp[k];
}

/*******************  FUNCTION  *********************/
/**
 * Fournit la vitesse de poiseuille pour une position donnée en considérant un tube de taille donnée.
 * @param i Position pour laquelle on cherche la vitesse.
 * @param size diamètre du tube.
**/
double helper_compute_poiseuille(int i,int size)
{
	double y = (double)(i - 1);
	double L = (double)(size - 1);
	return 4.0 * INFLOW_MAX_VELOCITY / ( L * L ) * ( L * y - y * y );
}

/*******************  FUNCTION  *********************/
/**
 * Applique la méthode de Zou/He pour simler un fluidre entrant dans le domain de gauche vers la droite sur une
 * interface verticale. Le profile de vitesse du fluide entrant suit une distribution de poiseuille.
 * @param mesh Maillage considéré (surtout pour avoir la hauteur.)
 * @param cell Maille à mettre à jour.
 * @param id_y Position en y de la cellule pour savoir comment calculer la vitesse de poiseuille.
**/
void compute_inflow_zou_he_poiseuille_distr(const VarSystem::CellAccessor & in,VarSystem::CellAccessor & out,const CMRCellPosition & pos,int x,int y)
{
	//vars
	double v;
	double density;

	//errors
	#if DIRECTIONS != 9
	#error Implemented only for 9 directions
	#endif

	//set macroscopic fluide info
	//poiseuille distr on X and null on Y
	//we just want the norm, so v = v_x
	v = helper_compute_poiseuille(pos.getAbsY(x),pos.globalMesh.height);
	const float * cellIn = *in.directions(x,y);
	float * cellOut = *out.directions(x,y);

	//compute rho from u and inner flow on surface
	density = (cellIn[0] + cellIn[2] + cellIn[4] + 2 * ( cellIn[3] + cellIn[6] + cellIn[7] )) / (1.0 - v) ;

	//now compute unknown microscopic values
	cellOut[1] = cellIn[3];// + (2.0/3.0) * density * v_y <--- no velocity on Y so v_y = 0
	cellOut[5] = cellIn[7] - (1.0/2.0) * (cellIn[2] - cellIn[4])
	                         + (1.0/6.0) * (density * v);
	                       //+ (1.0/2.0) * density * v_y    <--- no velocity on Y so v_y = 0
	cellOut[8] = cellIn[6] + (1.0/2.0) * (cellIn[2] - cellIn[4])
	                         + (1.0/6.0) * (density * v);
	                       //- (1.0/2.0) * density * v_y    <--- no velocity on Y so v_y = 0

	//no need to copy already known one as the value will be "loss" in the wall at propagatation time
}

/*******************  FUNCTION  *********************/
/**
 * Applique la méthode de Zou/He pour simler un fluidre sortant du domain de gauche vers la droite sur une
 * interface verticale. La condition appliquer pour construire l'équation est le maintient d'un gradiant de densité
 * nulle à l'interface.
 * @param mesh Maillage considéré (surtout pour avoir la hauteur.)
 * @param cell Maille à mettre à jour
 * @param id_y Position en y de la cellule pour savoir comment calculer la vitesse de poiseuille.
**/
void compute_outflow_zou_he_const_density(const VarSystem::CellAccessor & in,VarSystem::CellAccessor & out,int x,int y)
{
	//vars
	const double density = 1.0;
	double v;

	//errors
	#if DIRECTIONS != 9
	#error Implemented only for 9 directions
	#endif
	
	const float * cellIn = *in.directions(x,y);
	float * cellOut = *out.directions(x,y);

	//compute macroscopic v depeding on inner flow going onto the wall
	v = -1.0 + (1.0 / density) * (cellIn[0] + cellIn[2] + cellIn[4] + 2 * (cellIn[1] + cellIn[5] + cellIn[8]));

	//now can compute unknown microscopic values
	cellOut[3] = cellIn[1] - (2.0/3.0) * density * v;
	cellOut[7] = cellIn[5] + (1.0/2.0) * (cellIn[2] - cellIn[4])
	                       //- (1.0/2.0) * (density * v_y)    <--- no velocity on Y so v_y = 0
	                         - (1.0/6.0) * (density * v);
	cellOut[6] = cellIn[8] + (1.0/2.0) * (cellIn[4] - cellIn[2])
	                       //+ (1.0/2.0) * (density * v_y)    <--- no velocity on Y so v_y = 0
	                         - (1.0/6.0) * (density * v);
}

/*******************  FUNCTION  *********************/
struct ActionPropagation
{
	//TODO invert ii = x - ...  => ii <!-> x into in/out
	static void cellAction(const VarSystem::CellAccessor & in,VarSystem::CellAccessor& out,const CMRCellPosition & pos,int x,int y)
	{
		int k;
		int ii;
		int jj;
		
		//for all direction
		for ( k  = 0 ; k < DIRECTIONS ; k++)
		{
			//compute destination point
			ii = x+(direction_matrix[k][0]);
			jj = y+(direction_matrix[k][1]);
			//propagate to neighboor nodes
			if (pos.cellExist(ii,jj,1))
				(*out.directions(ii,jj))[k] = (*in.directions(x,y))[k];
		}
	}
};

struct ActionSpecialCells
{
	static void cellAction(const VarSystem::CellAccessor & in,VarSystem::CellAccessor & out,const CMRCellPosition & pos,int x,int y)
	{
		switch (*in.cellType(x,y))
		{
			case CELL_FUILD:
				break;
			case CELL_BOUNCE_BACK:
				compute_bounce_back(in,out,x,y);
				break;
			case CELL_LEFT_IN:
				compute_inflow_zou_he_poiseuille_distr(in,out ,pos,x,y);
				break;
			case CELL_RIGHT_OUT:
				compute_outflow_zou_he_const_density(in,out,x,y);
				break;
		}
	}
};

/*******************  FUNCTION  *********************/
struct ActionCollision
{
	static void cellAction(const VarSystem::CellAccessor & in,VarSystem::CellAccessor & out,int x,int y)
	{
		//vars
		int k;
		double density;
		LBMVect v;
		double feq;

		//compute macroscopic values
		density = get_cell_density(in,x,y);
		get_cell_velocity(v,in,density,x,y);

		//loop on microscopic directions
		for( k = 0 ; k < DIRECTIONS ; k++)
		{
			//compute f at equilibr.
			feq = compute_equilibrium_profile(v,density,k);
			//compute fout
			(*out.directions(x,y))[k] = (*in.directions(x,y))[k] - RELAX_PARAMETER * ((*in.directions(x,y))[k] - feq);
		}
	};
};

/*******************  FUNCTION  *********************/
struct ActionInitStatePoiseuil
{
	static void cellAction(const VarSystem::CellAccessor & in,VarSystem::CellAccessor & out,const CMRCellPosition & pos,int x,int y)
	{
		//vars
		int k;
		const double density = 1.0;
		LBMVect v = {0.0,0.0};
		
		//compute poiseuil speed
		v[0] = helper_compute_poiseuille(pos.getAbsY(x),pos.globalMesh.height);

		//apply poiseuil for all nodes except on top/bottom border
		for ( k = 0 ; k < DIRECTIONS ; k++)
			((*out.directions(x,y)))[k] = compute_equilibrium_profile(v,density,k);
	}
};

/*******************  FUNCTION  *********************/
struct ActionInitCellType
{
	ActionInitCellType(LBMCellType cellType)
	{
		this->cellType = cellType;
	}

	void cellAction(VarSystem::CellAccessor & cell,int x,int y) const
	{
		*cell.cellType(x,y) = cellType;
	}

	LBMCellType cellType;
};

/*******************  FUNCTION  *********************/
void setup_init_state(VarSystem & sys,const CMRRect & globalRect,const CMRRect & localRect)
{
	//setup all as poiseuil profile and setup all as fluide cell
	CMRMeshOperationSimpleLoopWithPos<VarSystem,ActionInitStatePoiseuil> initPoiseuil(&sys);
	initPoiseuil.run(localRect);
	
	//setup left border (flow in)
	CMRMeshOperationSimpleLoopInPlace<VarSystem,ActionInitCellType> initInCells(&sys,new ActionInitCellType(CELL_LEFT_IN));
	initInCells.run(globalRect.expended(1).getBorder(CMR_LEFT));
	
	//sys.getDomain(1,CMR_PREV_STEP)->printDebug();
	
	//setup right border (flow out)
	CMRMeshOperationSimpleLoopInPlace<VarSystem,ActionInitCellType> initOutCells(&sys,new ActionInitCellType(CELL_RIGHT_OUT));
	initOutCells.run(globalRect.expended(1).getBorder(CMR_RIGHT));
	
	//sys.getDomain(1,CMR_PREV_STEP)->printDebug();
	
	//setup top bounce back
	CMRMeshOperationSimpleLoopInPlace<VarSystem,ActionInitCellType> initBounceBack(&sys,new ActionInitCellType(CELL_BOUNCE_BACK));
	initBounceBack.run(globalRect.expended(1).getBorder(CMR_TOP));
	
	//sys.getDomain(1,CMR_PREV_STEP)->printDebug();
	
	//setup bottom bounce back
	initBounceBack.run(globalRect.expended(1).getBorder(CMR_BOTTOM));
	
	//sys.getDomain(1,CMR_PREV_STEP)->printDebug();
}

/*******************  FUNCTION  *********************/
int main(int argc, char * argv[])
{
	//init MPI
	MPI_Init(&argc,&argv);
	info_on_master("Start with np = %d",cmrGetMPISize());
	MPI_Barrier(MPI_COMM_WORLD);
	
	//try space splitter
	CMRRect globalRect(0,0,20,20);
	CMRBasicSpaceSplitter splitter(globalRect,cmrGetMPISize(),0);
	splitter.printDebug(CMR_MPI_MASTER);
	
	//try system computation
	CMRMPIDomainBuilder builder(&splitter);
	VarSystem sys(&builder);

	//init
	setup_init_state(sys,globalRect,splitter.getLocalDomain(cmrGetMPIRank()));
	
// 	CMRMeshOperationSimpleLoop<VarSystem,ActionCollision> loop1(&sys);
// 	loop1.run(CMRRect(10,10,40,40));
// 	
// 	CMRMeshOperationSimpleLoopWithPos<VarSystem,ActionSpecialCells> loop2(&sys);
// 	loop2.run(CMRRect(10,10,40,40));
// 	
// 	CMRMeshOperationSimpleLoopWithPos<VarSystem,ActionPropagation> loop3(&sys);
// 	loop3.run(CMRRect(10,10,40,40));

	//Finish
	MPI_Barrier(MPI_COMM_WORLD);
	info_on_master("Finish");	
	MPI_Finalize();
	return EXIT_SUCCESS;
}
