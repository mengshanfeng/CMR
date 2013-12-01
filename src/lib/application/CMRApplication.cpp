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
#include "runner/CMRRunner.h"
#include "CMRApplication.h"

/*******************  FUNCTION  *********************/
CMRApplication::CMRApplication ( void )
{
	this->runner = NULL;
}

/*******************  FUNCTION  *********************/
CMRApplication::~CMRApplication ( void )
{
}

/*******************  FUNCTION  *********************/
void CMRApplication::setRunner ( CMRRunner* runner )
{
	assume(runner != NULL,"Please provide a non NULL CMRRunner to make init step.");
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
