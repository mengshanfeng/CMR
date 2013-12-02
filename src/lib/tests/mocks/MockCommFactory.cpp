/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "MockCommFactory.h"

/*******************  FUNCTION  *********************/
CMRComm* MockCommFactory::createComm ( CMRAbstractDomain* domain, const CMRRect& rect, CMRCommType commType )
{
	MockComm * comm = new MockComm;
	comm->rect = rect;
	comm->id = id;
	comm->dest = dest;
	return comm;
}

/*********************  CLASS  **********************/
void MockComm::run(void )
{
	this->hasRun = true;
}