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
#include <domain/CMRVarSystem.h>

/*******************  FUNCTION  *********************/
CMRRunner::CMRRunner ( )
{
	//setup default values
	this->system = NULL;
	this->outputer = NULL;
	this->writeInterval = 1;
}

/*******************  FUNCTION  *********************/
CMRRunner::~CMRRunner(void)
{
}

/*******************  FUNCTION  *********************/
void CMRRunner::initLibs ( int& argc, char**& argv )
{
}

/*******************  FUNCTION  *********************/
void CMRRunner::finishLibs ( void )
{
}

/*******************  FUNCTION  *********************/
// void CMRRunner::initDomain ( CMRRunnerInitFactory* factory )
// {
// 	//try space splitter
// 	splitter = factory->createSplitter(globalRect);
// 	splitter->printDebug(CMR_MPI_MASTER);
// 	
// 	//try system computation
// 	CMRDomainBuilder * builder = factory->createDomainBuilder(splitter);
// 	this->system = factory->createVarSystem(builder);
// 	
// 	//get local domaine
// 	this->localId = this->getLocalId();
// 	this->localRect = splitter->getLocalDomain(localId);
// }

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
void CMRRunner::init ( CMRVarSystem* varSystem, const CMRRect & globalRect,const CMRRect & localRect )
{
	this->system = varSystem;
	this->globalRect = globalRect;
	this->localRect = localRect;
}
