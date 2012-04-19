/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "MockCommunicator.h"

/*******************  FUNCTION  *********************/
CMRComm* MockCommunicator::createComm ( CMRAbstractDomain* domain, const CMRRect2D& rect, CMRCommType commType )
{
	MockCommunication * comm = new MockCommunication;
	comm->rect = rect;
	comm->id = id;
	comm->dest = dest;
	return comm;
}

/*********************  CLASS  **********************/
void MockCommunication::run(void )
{
	this->hasRun = true;
}