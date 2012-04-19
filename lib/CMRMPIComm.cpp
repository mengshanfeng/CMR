/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cassert>
#include <cstdlib>
#include <mpi.h>
#include "CMRMPIComm.h"
#include "CMRAbstractDomain.h"
#include "CMRMPICommFactory.h"

/*******************  FUNCTION  *********************/
CMRMPIComm::CMRMPIComm ( CMRMPICommFactory * communicator, CMRAbstractDomain* domain, const CMRRect2D& rect, CMRCommType commType ) 
{
	//errors
	assert(communicator != NULL);
	assert(domain != NULL);

	//set default
	this->communicator = communicator;
	this->domain = domain;
	this->rect = rect;
	this->commType = commType;
	this->commStatus = CMR_COMM_STATUS_INIT;
}

/*******************  FUNCTION  *********************/
void CMRMPIComm::run ( void )
{
	//errors
	assert(this->commStatus != CMR_COMM_STATUS_INIT);

	//new status
	this->commStatus = CMR_COMM_STATUS_STARTED;
	
	switch(commType)
	{
		case CMR_COMM_SEND:
			this->runSend();
			break;
		case CMR_COMM_RECV:
			this->runReceive();
			break;
		default:
			assert(false);
			abort();
	}

	//final status
	this->commStatus = CMR_COMM_STATUS_DONE;
}

/*******************  FUNCTION  *********************/
void CMRMPIComm::runReceive ( void )
{
	//vars
	char * buffer = NULL;
	size_t size = 0;
	int res;

	//get size
	size = domain->getGhostSize(rect);
	assert(size > 0);

	//get buffer
	if (domain->isContiguousGhost(rect))
	{
		buffer = (char*)domain->getContiguousGhost(rect);
	} else {
		buffer = new char[size];
		assert(buffer != NULL);
	}

	//do the send
	res = MPI_Recv(buffer,size,MPI_CHAR,communicator->getTargetId(),communicator->getSendId(),MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	assert(res == 0);
	

	//free the buffer
	if (domain->isContiguousGhost(rect))
	{
		domain->copyGhostFromBuffer(buffer,size,rect);
		delete buffer;
	}
}

/*******************  FUNCTION  *********************/
void CMRMPIComm::runSend ( void )
{
	//vars
	char * buffer = NULL;
	size_t size = 0;
	int res;

	//get size
	size = domain->getGhostSize(rect);
	assert(size > 0);

	//get buffer
	if (domain->isContiguousGhost(rect))
	{
		buffer = (char*)domain->getContiguousGhost(rect);
	} else {
		buffer = new char[size];
		assert(buffer != NULL);
		domain->copyGhostToBuffer(buffer,size,rect);
	}

	//do the send
	res = MPI_Send(buffer,size,MPI_CHAR,communicator->getTargetId(),communicator->getSendId(),MPI_COMM_WORLD);
	assert(res == 0);

	//free the buffer
	if (domain->isContiguousGhost(rect))
		delete buffer;
}
