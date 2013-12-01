/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "operations/CMRMeshOperation.h"
#include "domain/CMRVarSystem.h"
#include "CMRRunnerSeq.h"
#include "outputer/CMROutputer.h"

/*******************  FUNCTION  *********************/
CMRRunnerSeq::CMRRunnerSeq ( void )
{
}

/*******************  FUNCTION  *********************/
CMRRunnerSeq::~CMRRunnerSeq ( void )
{
}

/*******************  FUNCTION  *********************/
void CMRRunnerSeq::runInitStep ( CMRMeshOperationNodeVector& actions )
{
	//init CURRENT and PREV
	for (CMRMeshOperationNodeVector::iterator it = actions.begin() ; it != actions.end() ; ++it)
		it->op->run(system,it->rect);
	system->permutVar(CMR_ALL);
	for (CMRMeshOperationNodeVector::iterator it = actions.begin() ; it != actions.end() ; ++it)
		it->op->run(system,it->rect);
}

/*******************  FUNCTION  *********************/
void CMRRunnerSeq::prepareWrite ( CMRMeshOperationNodeVector& actions )
{
	for (CMRMeshOperationNodeVector::iterator it = actions.begin() ; it != actions.end() ; ++it)
	{
		if (it->op->checkNeedPermut())
			fatal("Hum, check if there is no issue if permut here.");
		it->op->run(system,it->rect);
	}
}

/*******************  FUNCTION  *********************/
void CMRRunnerSeq::doWrite ( CMROutputer& outputer )
{
	outputer.write(system,globalRect,localRect);
}

/*******************  FUNCTION  *********************/
void CMRRunnerSeq::runCompute ( CMRMeshOperationNodeVector& actions )
{
	for (CMRMeshOperationNodeVector::iterator it = actions.begin() ; it != actions.end() ; ++it)
	{
		if (it->op->checkNeedPermut())
			system->permutVar(CMR_ALL);
		it->op->run(system,it->rect);
	}
}

/*******************  FUNCTION  *********************/
void CMRRunnerSeq::run ( int iterations )
{
	runInitStep(initActions);
	
	//save step
	if ( outputer != NULL)
	{
		prepareWrite(writeActions);
		doWrite(*outputer);
	}
	
	for (int i = 1 ; i <= iterations ; i++)
	{
		//progression
		if (i % 100 == 0)
			info_on_master("Progress [%5d / %5d]",i,iterations);
		
		runCompute(loopActions);
		
		//save step
		if ( i % writeInterval == 0 && outputer != NULL)
		{
			prepareWrite(writeActions);
			doWrite(*outputer);
		}
	}
}
