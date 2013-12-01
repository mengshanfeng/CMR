/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cassert>
#include "common/CMRDebug.h"
#include "CMRApplication.h"
#include <runner/CMRRunner.h>

/*******************  FUNCTION  *********************/
CMRApplication::CMRApplication ( void )
{
	this->state = CMR_INIT_STATE_NONE;
}

/*******************  FUNCTION  *********************/
CMRApplication::~CMRApplication ( void )
{
	wassume(this->state == CMR_INIT_STATE_FINISH_DEPS,"Caution, you try to exit but application is not in FINISHED state.");
}

/*******************  FUNCTION  *********************/
void CMRApplication::checkReadyForStep ( CMRInitState nextState )
{
	switch(nextState)
	{
		case CMR_INIT_STATE_NONE:
			fatal("Invalid nextState NONE in checkReadyForStep().");
			break;
		case CMR_INIT_STATE_INIT_DEPS:
			assume(this->state == CMR_INIT_STATE_NONE,"Invalide state movement for application live cycle, require NONE -> INIT_DEPS, but get %d -> INIT_DEPS.",state);
			break;
		case CMR_INIT_STATE_INIT:
			assume(this->state == CMR_INIT_STATE_INIT_DEPS,"Invalide state movement for application live cycle, require INIT_DEPS -> INIT, but get %d -> INIT.",state);
			break;
		case CMR_INIT_STATE_FINISHED:
			assume(this->state == CMR_INIT_STATE_INIT,"Invalide state movement for application live cycle, require INIT -> FINISHED, but get %d -> FINISHED.",state);
			break;
		case CMR_INIT_STATE_FINISH_DEPS:
			assume(this->state == CMR_INIT_STATE_FINISHED,"Invalide state movement for application live cycle, require FINISHED -> FINISH_DEPS, but get %d -> FINISH_DEPS.",state);
			break;
	}
}

/*******************  FUNCTION  *********************/
void CMRApplication::markStep ( CMRInitState nextState )
{
	assume (nextState != CMR_INIT_STATE_INIT,"For INIT state, please use markInitStep() to provide the runner to the CMRApplication base class.");
	checkReadyForStep(nextState);
	this->state = nextState;
}

/*******************  FUNCTION  *********************/
void CMRApplication::markInitStep ( CMRRunner* runner )
{
	assume(runner != NULL,"Please provide a non NULL CMRRunner to make init step.");
	checkReadyForStep(CMR_INIT_STATE_INIT);
	this->runner = runner;
}

/*******************  FUNCTION  *********************/
void CMRApplication::addInitAction ( CMRMeshOperation* op, CMRRect rect )
{
	assert(runner != NULL);
	this->runner->addInitAction(op,rect);
}

/*******************  FUNCTION  *********************/
void CMRApplication::addLoopAction ( CMRMeshOperation* op, CMRRect rect )
{
	assert(runner != NULL);
	this->runner->addLoopAction(op,rect);
}

/*******************  FUNCTION  *********************/
void CMRApplication::addPrepareWriteAction ( CMRMeshOperation* op, CMRRect rect )
{
	assert(runner != NULL);
	this->runner->addPrepareWriteAction(op,rect);
}

/*******************  FUNCTION  *********************/
CMRRect CMRApplication::getGlobalRect ( void )
{
	assert(runner != NULL);
	return runner->getGlobalRect();
}

/*******************  FUNCTION  *********************/
CMRRect CMRApplication::getLocalRect ( void )
{
	assert(runner != NULL);
	return runner->getLocalRect();
}

/*******************  FUNCTION  *********************/
CMRRunner& CMRApplication::getRunner ( void )
{
	assert(runner != NULL);
	return *runner;
}
