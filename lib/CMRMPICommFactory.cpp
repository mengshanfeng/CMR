/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "CMRMPICommFactory.h"
#include "CMRMPIComm.h"

/*******************  FUNCTION  *********************/
CMRMPICommFactory::CMRMPICommFactory ( int targetId ,int sendId, int receiveId)
{
	this->targetId = targetId;
	this->sendId = sendId;
	this->receiveId = receiveId;
}

/*******************  FUNCTION  *********************/
CMRComm* CMRMPICommFactory::createComm ( CMRAbstractDomain* domain, const CMRRect2D& rect, CMRCommType commType )
{
	return new CMRMPIComm(this,domain,rect,commType);
}

/*******************  FUNCTION  *********************/
int CMRMPICommFactory::getTargetId ( void ) const
{
	return this->targetId;
}

/*******************  FUNCTION  *********************/
int CMRMPICommFactory::getReceiveId ( void ) const
{
	return this->receiveId;
}

/*******************  FUNCTION  *********************/
int CMRMPICommFactory::getSendId ( void ) const
{
	return this->sendId;
}
