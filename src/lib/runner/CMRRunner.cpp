/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <mpi.h>
#include <cassert>
#include "common/CMRDebug.h"
#include <common/CMRCommon.h>
#include <common/CMRSpaceSplitter.h>
#include "CMRRunner.h"
#include "CMRRunnerInitFactory.h"

/*******************  FUNCTION  *********************/
CMRRunner::CMRRunner ( int& argc, char**& argv,const CMRRect & globalDomainSize,CMRRunnerInitFactory * factory)
{
	//init
	initMPI(argc,argv);
	this->splitter = NULL;
	this->system = NULL;
	//setup some vars
	this->globalRect = globalDomainSize;
	assert(globalDomainSize.x == 0);
	assert(globalDomainSize.y == 0);
	this->localId = -1;
	//if not null factory, setup domain)
	if (factory != NULL)
		initDomain(factory);
}

/*******************  FUNCTION  *********************/
CMRRunner::~CMRRunner(void)
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
void CMRRunner::initMPI ( int & argc, char **& argv )
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
void CMRRunner::initDomain ( CMRRunnerInitFactory* factory )
{
	//try space splitter
	splitter = factory->createSplitter(globalRect);
	splitter->printDebug(CMR_MPI_MASTER);
	
	//try system computation
	CMRDomainBuilder * builder = factory->createDomainBuilder(splitter);
	this->system = factory->createVarSystem(builder);
	
	//get local domaine
	this->localId = factory->getLocalId();
	this->localRect = splitter->getLocalDomain(localId);
}

/*******************  FUNCTION  *********************/
void CMRRunner::addInitAction ( CMRMeshOperation* op, CMRRect rect )
{
	assert(op != NULL);
	initActions.push_back(CMRMeshOperationNode(op,rect));
}

/*******************  FUNCTION  *********************/
void CMRRunner::addLoopAction ( CMRMeshOperation* op, CMRRect rect )
{
	assert(op != NULL);
	loopActions.push_back(CMRMeshOperationNode(op,rect));
}

/*******************  FUNCTION  *********************/
void CMRRunner::addPrepareWriteAction ( CMRMeshOperation* op, CMRRect rect )
{
	assert(op != NULL);
	writeActions.push_back(CMRMeshOperationNode(op,rect));
}

/*******************  FUNCTION  *********************/
void CMRRunner::setWriter ( CMROutputer* outputer, int writeInterval )
{
	this->outputer = outputer;
	this->writeInterval = writeInterval;
}

/*******************  FUNCTION  *********************/
CMRRect CMRRunner::getLocalRect(void) const
{
	return localRect;
}

/*******************  FUNCTION  *********************/
CMRRect CMRRunner::getGlobalRect(void) const
{
	return globalRect;
}

/*******************  FUNCTION  *********************/
const CMRAbstractSpaceSplitter & CMRRunner::getSplitter(void) const
{
	return *splitter;
}
