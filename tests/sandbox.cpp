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

using namespace std;

struct VarSystem
{
	struct CellAccessor
	{
		CellAccessor(VarSystem & sys,int x,int y);
		CellAccessor(CellAccessor & acc,int x,int y);
		CMRTypedDomainStorage<float>::CellAccessor density;
		CMRTypedDomainStorage<float>::CellAccessor variation;
	};
	VarSystem(int width,int height,int ghostDepth,int origX,int origY);
	CMRTypedDomainStorage<float> density;
	CMRTypedDomainStorage<float> variation;
};

VarSystem::VarSystem ( int width, int height, int ghostDepth, int origX, int origY )
	:density(width,height,ghostDepth,origX,origY),variation(width,height,ghostDepth,origX,origY)
{
}



VarSystem::CellAccessor::CellAccessor ( VarSystem& sys, int x, int y )
	:density(sys.density.getCellAccessor(x,y)),variation(sys.variation.getCellAccessor(x,y))
{
	
}

VarSystem::CellAccessor::CellAccessor ( CellAccessor& acc, int x, int y )
	:density(acc.density,x,y),variation(acc.variation,x,y)
{
}

class ActionInc
{
	public:
		static void cellAction(VarSystem::CellAccessor & in,VarSystem::CellAccessor& out);
};

void ActionInc::cellAction ( VarSystem::CellAccessor& in, VarSystem::CellAccessor& out )
{
	debug("Update cell : %p",&in.density.getCell(0,0));
	out.density.getCell(0,0) += (in.density.getCell(0,0) * 3 + 5) / in.variation.getCell(0,0);
}

int main(int argc, char * argv[])
{
	//init MPI
	MPI_Init(&argc,&argv);
	info_on_master("Start with np = %d",cmrGetMPISize());
	MPI_Barrier(MPI_COMM_WORLD);
	
	//try space splitter
	CMRBasicSpaceSplitter splitter(0,0,800,600,cmrGetMPISize(),0);
	splitter.printDebug(CMR_MPI_MASTER);

	//try comm
	CMRTypedDomainStorage<float> domain(800,600,1,0,0);
	
	assert(cmrGetMPISize() == 2);
	
	CMRCommSchem schem("sync_left_right");
	switch (cmrGetMPIRank())
	{
		case 0:
			domain.setCommunicator(1,0,new CMRMPICommFactory(1,1,1));
			domain.fillWithUpdateComm(schem,1,0,1,CMR_COMM_SEND);
			break;
		case 1:
			domain.setCommunicator(-1,0,new CMRMPICommFactory(0,0,1));
			domain.fillWithUpdateComm(schem,-1,0,1,CMR_COMM_RECV);
			break;
		default:
			fatal("This test only work with exactly 2 MPI tasks.");
	}

	//to comm
	schem.printDebug();
	schem.run();
	
	//try system computation
	VarSystem sys(800,600,1,0,0);
	CMRMeshOperationSimpleLoop<VarSystem,ActionInc> loop(&sys,&sys);
	loop.run(CMRRect(10,10,40,40));

	//Finish
	MPI_Barrier(MPI_COMM_WORLD);
	info_on_master("Finish");	
	MPI_Finalize();
	return EXIT_SUCCESS;
}
