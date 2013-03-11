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
#include <math.h>
#include <cstdio>
#include <stdint.h>

using namespace std;

#define DIRECTIONS 9
#define DIMENSIONS 2
//=>(lbm_gbl_config.relax_parameter)
#define INFLOW_MAX_VELOCITY 0.1
//=>(lbm_gbl_config.inflow_max_velocity)
#define REYNOLDS 300
#define KINETIC_VISCOSITY (INFLOW_MAX_VELOCITY * 2.0 * OBSTACLE_R / REYNOLDS)
#define RELAX_PARAMETER (1.0 / (3.0 * KINETIC_VISCOSITY + 1.0/2.0))
#define WIDTH 800
#define HEIGHT 100
#define OBSTACLE_R ((HEIGHT)/8.0)
#define ITERATIONS 8000
#define WRITE_STEP_INTERVAL 50

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

/********************  STRUCT  **********************/
/** Structure des en-tête utilisé dans le fichier de sortie. **/
struct LBMFileHeader
{
	/** Pour validation du format du fichier. **/
	uint32_t magick;
	/** Taille totale du maillage simulé (hors mailles fantômes). **/
	uint32_t mesh_width;
	/** Taille totale du maillage simulé (hors mailles fantômes). **/
	uint32_t mesh_height;
	/** Number of vertical lines. **/
	uint32_t lines;
};

/********************  STRUCT  **********************/
/** Une entrée du fichier, avec les deux grandeurs macroscopiques. **/
struct LBMFileEntry
{
	float v;
	float density;
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
			CMRCellAccessor<LBMFileEntry,CMRMemoryModelColMajor> fileout;
		};
		VarSystem(CMRDomainBuilder * builder);
};

VarSystem::VarSystem ( CMRDomainBuilder * builder)
:CMRVarSystem(builder)
{
	this->addVariable("directions",sizeof(float[DIRECTIONS]),1);
	this->addVariable("cellType",sizeof(LBMCellType),1);
	this->addVariable("fileout",sizeof(LBMFileEntry),0);
}



VarSystem::CellAccessor::CellAccessor (CMRVarSystem & sys,int tstep,int x,int y,bool absolute)
	:directions(*(sys.getDomain(0,tstep)),x,y,absolute),cellType(*sys.getDomain(1,tstep),x,y,absolute),fileout(*sys.getDomain(2,tstep),x,y,absolute)
{
	
}

VarSystem::CellAccessor::CellAccessor ( CellAccessor& acc, int x, int y,bool absolute)
	:directions(acc.directions,x,y,absolute),cellType(acc.cellType,x,y,absolute),fileout(acc.cellType,x,y,absolute)
{
}

/*******************  FUNCTION  *********************/
std::string cmrTypeToString(const LBMFileEntry & value)
{
	return "??";
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
	v = helper_compute_poiseuille(pos.getAbsY(y),pos.globalMesh.height);
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
			ii = x-(direction_matrix[k][0]);
			jj = y-(direction_matrix[k][1]);
			//propagate to neighboor nodes
			//if (pos.cellExist(ii,jj,1))
			(*out.directions(x,y))[k] = (*in.directions(ii,jj))[k];
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
		v[0] = helper_compute_poiseuille(pos.getAbsY(y),pos.globalMesh.height);
		if (x == 40)
			v[0] = 0.0;

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
struct ActionUpdateFileout
{
	void cellAction(VarSystem::CellAccessor & cell,int x,int y) const
	{
		LBMVect v;
		double density;
		
		density = get_cell_density(cell,x,y);
		get_cell_velocity(v,cell,density,x,y);

		//compute macrospic values
		cell.fileout(x,y)->density = density;
		cell.fileout(x,y)->v       = sqrt(get_vect_norme_2(v,v));
	}
};

struct ActionSetupObstable
{
	ActionSetupObstable(float x,float y)
	{
		OBSTACLE_X = x;
		OBSTACLE_Y = y;
	}

	void cellAction(VarSystem::CellAccessor & cell,int x,int y) const
	{
		if ( ( (x-OBSTACLE_X) * (x-OBSTACLE_X) ) + ( (y-OBSTACLE_Y) * (y-OBSTACLE_Y) ) <= OBSTACLE_R * OBSTACLE_R )
		{
			(*cell.cellType(x,y)) = CELL_BOUNCE_BACK;
			LBMVect v = {0.0,0.0};
			//apply poiseuil for all nodes except on top/bottom border
			for ( int k = 0 ; k < DIRECTIONS ; k++)
				((*cell.directions(x,y)))[k] = compute_equilibrium_profile(v,1.0,k);
		}
	}

	float OBSTACLE_X;
	float OBSTACLE_Y;
};

#define RESULT_MAGICK 0x12345
#define RESULT_FILENAME "out.raw"

/*******************  FUNCTION  *********************/
/**
 * Ecrit l'en-tête du fichier de sortie. Cet en-tête sert essentiellement à fournir les informations
 * de taille du maillage pour les chargements.
 * @param fp Descripteur de fichier à utiliser pour l'écriture.
**/
void write_file_header(FILE * fp,const CMRAbstractSpaceSplitter & splitter)
{
	//setup header values
	LBMFileHeader header;
	header.magick      = RESULT_MAGICK;
	header.mesh_height = splitter.getDomain().height;
	header.mesh_width  = splitter.getDomain().width;
	header.lines       = splitter.getSplittingSize().y;

	//write file
	fwrite(&header,sizeof(header),1,fp);
}

/*******************  FUNCTION  *********************/
FILE * open_output_file(const CMRAbstractSpaceSplitter & splitter)
{
	//vars
	FILE * fp;

	//open result file
	fp = fopen(RESULT_FILENAME,"w");

	//errors
	if (fp == NULL)
	{
		perror(RESULT_FILENAME);
		abort();
	}

	//write header
	write_file_header(fp,splitter);

	return fp;
}

/*******************  FUNCTION  *********************/
void write_to_file(FILE * fp,VarSystem & sys,const CMRRect & globalRect,const CMRRect & localRect)
{
	assert(fp != NULL);
	CMRMeshOperationSimpleLoopInPlace<VarSystem,ActionUpdateFileout> initBounceBack(&sys);
	initBounceBack.run(localRect);
	assert(sys.getDomain(2,CMR_CURRENT_STEP)->isContiguousGhost(localRect));
	size_t size = sys.getDomain(2,CMR_CURRENT_STEP)->getGhostSize(localRect);
	void * buffer = sys.getDomain(2,CMR_PREV_STEP)->getContiguousGhost(localRect);
	assert(buffer != NULL);
	debug("Write size = %lu",size);
	fwrite(buffer,1,size,fp);
}

/*******************  FUNCTION  *********************/
void setup_init_state(VarSystem & sys,const CMRRect & globalRect,const CMRRect & localRect)
{
	//setup all as poiseuil profile and setup all as fluide cell
	CMRMeshOperationSimpleLoopWithPos<VarSystem,ActionInitStatePoiseuil> initPoiseuil(&sys);
	initPoiseuil.run(localRect.expended(1));
	
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
	CMRMeshOperationSimpleLoopInPlace<VarSystem,ActionSetupObstable> initObstacle(&sys,new ActionSetupObstable(WIDTH/10.0 + 1.0,HEIGHT/2.0+3.0));
	initObstacle.run(globalRect);
}

/*******************  FUNCTION  *********************/
int main(int argc, char * argv[])
{
	//vars
	FILE * fp = NULL;

	//init MPI
	MPI_Init(&argc,&argv);
	info_on_master("Start with np = %d",cmrGetMPISize());
	MPI_Barrier(MPI_COMM_WORLD);
	
	//try space splitter
	CMRRect globalRect(0,0,WIDTH,HEIGHT);
	CMRBasicSpaceSplitter splitter(globalRect,cmrGetMPISize(),0);
	splitter.printDebug(CMR_MPI_MASTER);
	
	//try system computation
	CMRMPIDomainBuilder builder(&splitter);
	VarSystem sys(&builder);
	
	//get local domaine
	CMRRect localDomain = splitter.getLocalDomain(cmrGetMPIRank());
	
	//open fileout
	if (cmrIsMPIMaster())
		fp = open_output_file(splitter);

	//init CURRENT and PREV
	setup_init_state(sys,globalRect,localDomain);
	sys.permutVar(CMR_ALL);
	setup_init_state(sys,globalRect,localDomain);
	
	//write to fileo
	if (fp != NULL)
		write_to_file(fp,sys,globalRect,localDomain);
	
	CMRBasicSpaceSplitter splitterBlocking(localDomain,1,0);
	splitterBlocking.printDebug();
	
	//time steps
	for ( int i = 1 ; i < ITERATIONS ; i++ )
	{
		//progression
		if (i % 10 == 0)
			info_on_master("Progress [%5d / %5d]",i,ITERATIONS);

		for (int block = 0 ; block < 1 ; block++)
		{
			const CMRRect blockRect = splitterBlocking.getLocalDomain(block);
		
			//compute special actions (border, obstacle...)
			//special_cells( &mesh, &mesh_type, &mesh_comm);
			CMRMeshOperationSimpleLoopWithPos<VarSystem,ActionSpecialCells> loop2(&sys);
			loop2.run(blockRect.expended(1));
			sys.permutVar(CMR_ALL);

			//compute collision term
			//collision( &temp, &mesh);
			CMRMeshOperationSimpleLoop<VarSystem,ActionCollision> loop1(&sys);
			loop1.run(blockRect);
			sys.permutVar(CMR_ALL);

			//propagate values from node to neighboors
			//lbm_comm_ghost_exchange( &mesh_comm, &temp );
			
			//prop
			//propagation( &mesh, &temp);
			CMRMeshOperationSimpleLoopWithPos<VarSystem,ActionPropagation> loop3(&sys);
			loop3.run(blockRect);
		}

		//save step
		if ( i % WRITE_STEP_INTERVAL == 0 && fp != NULL)
			write_to_file(fp,sys,globalRect,localDomain);
	}
	
	//close file
	if (fp != NULL)
		fclose(fp);

	//Finish
	MPI_Barrier(MPI_COMM_WORLD);
	info_on_master("Finish");	
	MPI_Finalize();
	return EXIT_SUCCESS;
}
