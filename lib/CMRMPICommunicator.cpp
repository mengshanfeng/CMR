/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "CMRMPICommunicator.h"
#include "CMRMPICommunication.h"

/*******************  FUNCTION  *********************/
CMRMPICommunicator::CMRMPICommunicator ( int targetId ,int sendId, int receiveId)
{
	this->targetId = targetId;
	this->sendId = sendId;
	this->receiveId = receiveId;
}

/*******************  FUNCTION  *********************/
CMRCommunication* CMRMPICommunicator::createComm ( CMRAbstractDomain* domain, const CMRRect2D& rect, CMRCommType commType )
{
	return new CMRMPICommunication(this,domain,rect,commType);
}

/*******************  FUNCTION  *********************/
int CMRMPICommunicator::getTargetId ( void ) const
{
	return this->targetId;
}

/*******************  FUNCTION  *********************/
int CMRMPICommunicator::getReceiveId ( void ) const
{
	return this->receiveId;
}

/*******************  FUNCTION  *********************/
int CMRMPICommunicator::getSendId ( void ) const
{
	return this->sendId;
}
