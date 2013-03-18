#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <cassert>
#include <common/CMRDebug.h>
#include <communication/CMRMPIComm.h>
#include <MockAbstractDomain.h>
#include <communication/CMRMPICommFactory.h>
#include <communication/CMRCommSchem.h>
#include <communication/CMRMPIReduction.h>
#include <communication/CMRReductionDescriptor.h>
#include <domain/CMRDomainStorage.h>
#include <domain/CMRCellAccessor.h>
#include <domain/CMRMemoryModels.h>
#include <domain/CMRGenericMemoryAccessor.h>
#include <domain/CMRVarSystem.h>
#include <domain/CMRMPIDomainBuilder.h>
#include <common/CMRSpaceSplitter.h>
#include <CMROperation.h>
#include <cycle.h>

using namespace std;

struct GlobalVariables
{
	GlobalVariables() {dt = 0;};
	float dt;
};

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
		GlobalVariables &getGlobals(void) {return globals;};
		const GlobalVariables &getGlobals(void) const {return globals;};
	private:
		GlobalVariables globals;
};

VarSystem::VarSystem ( CMRDomainBuilder * builder)
	:CMRVarSystem(builder)
{
	this->addVariable("density",sizeof(float),1);
	this->addVariable("variation",sizeof(float),1);
	//this->addGlobalVariable("dt",sizeof(double),1);
}

VarSystem::CellAccessor::CellAccessor ( CMRVarSystem& sys, int tstep, int x, int y ,bool absolute)
	:density(*sys.getDomain(0,tstep),x,y,absolute),variation(*sys.getDomain(1,tstep),x,y,absolute)
{
	
}

VarSystem::CellAccessor::CellAccessor ( CellAccessor& acc, int x, int y ,bool absolute)
	:density(acc.density,x,y,absolute),variation(acc.variation,x,y,absolute)
{
}

struct ActionInc
{
	static void cellAction(const VarSystem::CellAccessor & in,VarSystem::CellAccessor& out,int x,int y)
	{
		//debug("Update cell : %p",&in.density(0,0));
		//out.density(0,0) += (in.density(0,0) * 3 + 5) / in.variation(0,0);
		*out.density(x,y) = *in.density(x,y) + 1;
	}
};

struct ActionIncInPlace
{
	static void cellAction(VarSystem::CellAccessor & cell)//,int x,int y)
	{
		//debug("Update cell : %p",&in.density(0,0));
		//out.density(0,0) += (in.density(0,0) * 3 + 5) / in.variation(0,0);
		*cell.density(0,0) += 1.0;
	}
	
	static void cellAction(VarSystem::CellAccessor & cell,int x,int y)
	{
		//debug("Update cell : %p",&in.density(0,0));
		//out.density(0,0) += (in.density(0,0) * 3 + 5) / in.variation(0,0);
		*cell.density(x,y) += 1.0;
	}
};

struct ActionInit
{
	static void cellAction(const VarSystem::CellAccessor & in,VarSystem::CellAccessor& out,const CMRCellPosition & pos,int x,int y)
	{
		*out.density(x,y) = pos.cellPos.y;
		*out.variation(x,y) = 1.0;
	}
};

struct ActionDTReduce
{
	//init reduction
	ActionDTReduce() {localValue = 0;};
	
	//cell action
	void cellAction(const VarSystem::CellAccessor & in,int x,int y)
	{
		localValue += *in.variation(x,y);
	}

	//eventual custom op
	void doReduce(const ActionDTReduce & rightMember)
	{
		localValue += rightMember.localValue;
	}
	
	//get final result
	void applyFinalResult(GlobalVariables & gbl)
	{
		gbl.dt = localValue * 4;
	}
	
	float localValue;
};

ticks testsimple(const CMRRect rect)
{
	float * buffer = new float[rect.surface()];
	memset(buffer,0,rect.surface() * sizeof(float));

	ticks t0 = getticks();
	for (int y = 0 ; y < rect.height ; y++)
		for (int x = 0 ; x < rect.width ; x++)
			buffer[x + y * rect.width]+=1.0;
	ticks t1 = getticks();
	return t1 - t0;
};

int main(int argc, char * argv[])
{
	//init MPI
	MPI_Init(&argc,&argv);
	info_on_master("Start with np = %d",cmrGetMPISize());
	MPI_Barrier(MPI_COMM_WORLD);
	
	//try space splitter
	CMRBasicSpaceSplitter splitter(0,0,1000,1000,cmrGetMPISize(),0);
	splitter.printDebug(CMR_MPI_MASTER);
	
	//try system computation
	CMRMPIDomainBuilder builder(&splitter);
	VarSystem sys(&builder);
	
	//get rect
	CMRRect localRect = splitter.getLocalDomain(cmrGetMPIRank());
	
	//init the mesh
	CMRMeshOperationSimpleLoopWithPos<VarSystem,ActionInit> loopInit(&sys);
	
	//init all steps
	loopInit.run(localRect.expended(1));
	
	//print current state
	//sys.getDomain(0,CMR_PREV_STEP)->printDebug();
// 	sys.getDomain(0,CMR_CURRENT_STEP)->printDebug();
	
	//permut
	sys.permutVar(CMR_ALL);
	
	//compute
	CMRMeshOperationSimpleLoop<VarSystem,ActionInc> loop(&sys);
	loop.run(localRect);
	
	//reduction
	info("Before reduction, dt = %f",sys.getGlobals().dt);
	ActionDTReduce reduction;
	CMRMeshReduction<VarSystem,ActionDTReduce> red(&sys,&reduction);
	red.run(localRect);
	reduction.applyFinalResult(sys.getGlobals());
	info("After reduction, dt = %f",sys.getGlobals().dt);
	CMRMPIReduction mpiReduce(new CMRGenericReductionDescriptor<ActionDTReduce>(&reduction));
	mpiReduce.run();
	reduction.applyFinalResult(sys.getGlobals());
	info("After MPI reduction, dt = %f",sys.getGlobals().dt);
	
	////////////////////////////////
	CMRMeshOperationSimpleLoopInPlace<VarSystem,ActionIncInPlace> loop2(&sys);
	ticks t0 = getticks();
	loop2.run(localRect);
	ticks t1 = getticks();
	info("Time of CMR loop : %f per cell",((float)(t1-t0))/(float)localRect.surface());
	
	t0 = getticks();
	loop2.run(localRect);
	t1 = getticks();
	info("Time of CMR loop (cached) : %f per cell",((float)(t1-t0))/(float)localRect.surface());
	
	t0 = testsimple(localRect);
	info("Time of std loop (cached) : %f per cell",((float)(t0))/(float)localRect.surface());
	////////////////////////////////
	
	//print current
	//sys.getDomain(0,CMR_PREV_STEP)->printDebug();
// 	sys.getDomain(0,CMR_CURRENT_STEP)->printDebug();
	
	//sync
	CMRCommSchem schem("Sync1");
	sys.getDomain(0,CMR_CURRENT_STEP)->syncAllGhosts(schem,1);
	sys.getDomain(1,CMR_CURRENT_STEP)->syncAllGhosts(schem,1);
	schem.printDebug();
	schem.run();
	
	//print current
	//sys.getDomain(0,CMR_PREV_STEP)->printDebug();
// 	sys.getDomain(0,CMR_CURRENT_STEP)->printDebug();
	
	//permut
	sys.permutVar(CMR_ALL);

	//Finish
	MPI_Barrier(MPI_COMM_WORLD);
	info_on_master("Finish");	
	MPI_Finalize();
	return EXIT_SUCCESS;
}
