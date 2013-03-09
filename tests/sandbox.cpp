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
#include <domain/CMRCellAccessor.h>
#include <domain/CMRMemoryModels.h>
#include <domain/CMRGenericMemoryAccessor.h>
#include <domain/CMRVarSystem.h>
#include <domain/CMRMPIDomainBuilder.h>
#include <common/CMRSpaceSplitter.h>
#include <CMROperation.h>

using namespace std;

class VarSystem : public CMRVarSystem
{
	public:
		struct CellAccessor
		{
			CellAccessor(CMRVarSystem & sys,int tstep,int x,int y,bool absolute = true);
			CellAccessor(CellAccessor & acc,int x,int y,bool absolute = false);
			CMRCellAccessor<float,CMRMemoryModelRowMajor> density;
			CMRCellAccessor<float,CMRMemoryModelRowMajor> variation;
		};
		VarSystem(CMRDomainBuilder * builder);
};

VarSystem::VarSystem ( CMRDomainBuilder * builder)
	:CMRVarSystem(builder)
{
	this->addVariable("density",sizeof(float),1);
	this->addVariable("variation",sizeof(float),1);
}

VarSystem::CellAccessor::CellAccessor ( CMRVarSystem& sys, int tstep, int x, int y ,bool absolute)
	:density(*sys.getDomain(0,tstep),x,y,absolute),variation(*sys.getDomain(1,tstep),x,y,absolute)
{
	
}

VarSystem::CellAccessor::CellAccessor ( CellAccessor& acc, int x, int y ,bool absolute)
	:density(acc.density,x,y,absolute),variation(acc.variation,x,y,absolute)
{
}

class ActionInc
{
	public:
		static void cellAction(VarSystem::CellAccessor & in,VarSystem::CellAccessor& out);
};

void ActionInc::cellAction ( VarSystem::CellAccessor& in, VarSystem::CellAccessor& out )
{
	//debug("Update cell : %p",&in.density.getCell(0,0));
	out.density.getCell(0,0) += (in.density.getCell(0,0) * 3 + 5) / in.variation.getCell(0,0);
}

int main(int argc, char * argv[])
{
	//init MPI
	MPI_Init(&argc,&argv);
	info_on_master("Start with np = %d",cmrGetMPISize());
	MPI_Barrier(MPI_COMM_WORLD);
	
	//try space splitter
	CMRBasicSpaceSplitter splitter(0,0,80,60,cmrGetMPISize(),0);
	splitter.printDebug(CMR_MPI_MASTER);

	//try comm
	/*CMRDomainStorage domain(sizeof(float),CMRRect(0,0,800,600),1);
	domain.setMemoryAccessor(new CMRGenericMemoryAccessor<float,CMRMemoryModelRowMajor>());
	
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
	schem.run();*/
	
	//try system computation
	CMRMPIDomainBuilder builder(&splitter);
	VarSystem sys(&builder);
	CMRMeshOperationSimpleLoop<VarSystem,ActionInc> loop(&sys);
	
	CMRRect localRect = splitter.getLocalDomain(cmrGetMPIRank());
	loop.run(localRect);
	
	CMRCommSchem schem("Sync1");
	sys.getDomain(0,0)->syncAllGhosts(schem,1);
	sys.getDomain(1,0)->syncAllGhosts(schem,1);
	schem.printDebug();
	schem.run();

	//Finish
	MPI_Barrier(MPI_COMM_WORLD);
	info_on_master("Finish");	
	MPI_Finalize();
	return EXIT_SUCCESS;
}
