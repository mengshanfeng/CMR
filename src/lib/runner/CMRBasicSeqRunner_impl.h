/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_BASIC_SEQ_RUNNER_IMPL_H
#define CMR_BASIC_SEQ_RUNNER_IMPL_H

/********************  HEADERS  *********************/
#include <mpi.h>
#include <cassert>
#include "common/CMRDebug.h"
#include <common/CMRCommon.h>
#include "CMRBasicSeqRunner.h"
#include "CMROperation.h"
#include <mpi/CMRMPIDomainBuilder.h>

/*******************  FUNCTION  *********************/
template <class TVarSystem>
CMRBasicSeqRunner<TVarSystem>::CMRBasicSeqRunner ( int& argc, char**& argv, const CMRRect& globalDomainSize )
{
	//setup some vars
	this->globalRect = globalDomainSize;
	assert(globalDomainSize.x == 0);
	assert(globalDomainSize.y == 0);
	//init
	initMPI(argc,argv);
	initDomain(globalDomainSize);
}

/*******************  FUNCTION  *********************/
template <class TVarSystem>
CMRBasicSeqRunner<TVarSystem>::~CMRBasicSeqRunner(void)
{
	//check MPI status
	int res;
	MPI_Finalized(&res);
	
	//Finish
	if (res == 0)
	{	
		MPI_Barrier(MPI_COMM_WORLD);
		info_on_master("Finish");	
		MPI_Finalize();
	}
}

/*******************  FUNCTION  *********************/
template <class TVarSystem>
void CMRBasicSeqRunner<TVarSystem>::initMPI ( int & argc, char **& argv )
{
	//check MPI status
	int res;
	MPI_Initialized(&res);
	
	//init MPI
	if (res == 0)
	{
		MPI_Init(&argc,&argv);
		info_on_master("Start with np = %d",cmrGetMPISize());
		MPI_Barrier(MPI_COMM_WORLD);
	}
}

/*******************  FUNCTION  *********************/
template <class TVarSystem>
void CMRBasicSeqRunner<TVarSystem>::initDomain ( const CMRRect& globalDomainSize )
{
	//try space splitter
	splitter = new CMRBasicSpaceSplitter(globalRect,cmrGetMPISize(),0);
	splitter->printDebug(CMR_MPI_MASTER);
	
	//try system computation
	CMRMPIDomainBuilder * builder = new CMRMPIDomainBuilder(splitter);
	this->system = new TVarSystem(builder);
	
	//get local domaine
	this->localRect = splitter->getLocalDomain(cmrGetMPIRank());
}

/*******************  FUNCTION  *********************/
template <class TVarSystem>
void CMRBasicSeqRunner<TVarSystem>::addInitAction ( CMRMeshOperation* op, CMRRect rect )
{
	assert(op != NULL);
	initActions.push_back(CMRMeshOperationNode(op,rect));
}

/*******************  FUNCTION  *********************/
template <class TVarSystem>
void CMRBasicSeqRunner<TVarSystem>::addLoopAction ( CMRMeshOperation* op, CMRRect rect )
{
	assert(op != NULL);
	loopActions.push_back(CMRMeshOperationNode(op,rect));
}

/*******************  FUNCTION  *********************/
template <class TVarSystem>
void CMRBasicSeqRunner<TVarSystem>::addPrepareWriteAction ( CMRMeshOperation* op, CMRRect rect )
{
	assert(op != NULL);
	writeActions.push_back(CMRMeshOperationNode(op,rect));
}

/*******************  FUNCTION  *********************/
template <class TVarSystem>
void CMRBasicSeqRunner<TVarSystem>::setWriter ( CMROutputer* outputer, int writeInterval )
{
	this->outputer = outputer;
	this->writeInterval = writeInterval;
}

/*******************  FUNCTION  *********************/
template <class TVarSystem>
void CMRBasicSeqRunner<TVarSystem>::run ( int iterations )
{
	//init CURRENT and PREV
	for (CMRMeshOperationNodeVector::iterator it = initActions.begin() ; it != initActions.end() ; ++it)
		it->op->run(system,it->rect);
	system->permutVar(CMR_ALL);
	for (CMRMeshOperationNodeVector::iterator it = initActions.begin() ; it != initActions.end() ; ++it)
		it->op->run(system,it->rect);
	
	//save step
	if ( outputer != NULL)
	{
		for (CMRMeshOperationNodeVector::iterator it = writeActions.begin() ; it != writeActions.end() ; ++it)
			it->op->run(system,it->rect);
		outputer->write(system,globalRect,localRect);
	}
	
	for (int i = 1 ; i <= iterations ; i++)
	{
		//progression
		if (i % 100 == 0)
			info_on_master("Progress [%5d / %5d]",i,iterations);
		
		for (CMRMeshOperationNodeVector::iterator it = loopActions.begin() ; it != loopActions.end() ; ++it)
		{
			it->op->run(system,it->rect);
			system->permutVar(CMR_ALL);
		}
		
		//save step
		if ( i % writeInterval == 0 && outputer != NULL)
		{
			for (CMRMeshOperationNodeVector::iterator it = writeActions.begin() ; it != writeActions.end() ; ++it)
				it->op->run(system,it->rect);
			outputer->write(system,globalRect,localRect);
		}
	}
}

/*******************  FUNCTION  *********************/
template <class TVarSystem>
CMRRect CMRBasicSeqRunner<TVarSystem>::getLocalRect(void) const
{
	return localRect;
}

/*******************  FUNCTION  *********************/
template <class TVarSystem>
CMRRect CMRBasicSeqRunner<TVarSystem>::getGlobalRect(void) const
{
	return globalRect;
}

/*******************  FUNCTION  *********************/
template <class TVarSystem>
const CMRAbstractSpaceSplitter & CMRBasicSeqRunner<TVarSystem>::getSplitter(void) const
{
	return *splitter;
}

#endif //CMR_BASIC_SEQ_RUNNER_IMPL_H
