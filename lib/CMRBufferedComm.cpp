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
#include "CMRDebug.h"
#include "CMRBufferedComm.h"
#include "CMRAbstractDomain.h"

/*******************  FUNCTION  *********************/
CMRBufferedComm::CMRBufferedComm ( CMRAbstractDomain* domain, const CMRRect2D& rect, CMRCommType commType )
{
	//errors
	assert(domain != NULL);

	//set defaults
	this->rect         = rect;
	this->buffer       = NULL;
	this->bufferSize   = 0;
	this->commType     = commType;
	this->domain       = domain;
	this->commStatus   = CMR_COMM_STATUS_INIT;
	this->isTempBuffer = false;
}

/*******************  FUNCTION  *********************/
CMRBufferedComm::~CMRBufferedComm ( void )
{
	//check that comm is finish
	assume(commStatus != CMR_COMM_STATUS_STARTED,"Invalid status while trying to delete CMRBufferedComm object.");
	//check buffer
	if (buffer != NULL)
		freeBuffer(buffer,bufferSize);
	buffer = NULL;
}

/*******************  FUNCTION  *********************/
void* CMRBufferedComm::allocateBuffer ( size_t size )
{
	return malloc(size);
}

/*******************  FUNCTION  *********************/
void CMRBufferedComm::freeBuffer ( void* buffer, size_t size )
{
	//error
	assert(buffer != NULL);

	//free the memoty
	free(buffer);
}

/*******************  FUNCTION  *********************/
void CMRBufferedComm::setupBuffer ( void )
{
	//nothing to do
	if (buffer != NULL)
		return;

	//get size
	bufferSize = domain->getGhostSize(rect);
	assert(bufferSize > 0);

	//get buffer
	if (domain->isContiguousGhost(rect))
	{
		buffer = (char*)domain->getContiguousGhost(rect);
		isTempBuffer = false;
	} else {
		buffer = allocateBuffer(bufferSize);
		assume(buffer != NULL,"Error while requesting buffer memory for communication.");
		isTempBuffer = true;
	}
}

/*******************  FUNCTION  *********************/
void CMRBufferedComm::run ( void )
{
	//vars

	//check state
	assume(commStatus == CMR_COMM_STATUS_INIT,"Invalid communication state, must be in INIT state, has %d.",commStatus);
	wassume(buffer == NULL,"Maybe this is a bug, it already has a buffer : %p.",buffer);

	//update status
	commStatus = CMR_COMM_STATUS_STARTED;

	//setup buffer
	setupBuffer();

	//if send, to copy
	switch(commType)
	{
		case CMR_COMM_SEND:
			domain->copyGhostToBuffer(buffer,bufferSize,rect);
			runSend(buffer,bufferSize);
			break;
		case CMR_COMM_RECV:
			runReceive(buffer,bufferSize);
			domain->copyGhostFromBuffer(buffer,bufferSize,rect);
			break;
		default:
			fatal("Invalid comm type : %d.",commType);
			abort();
	}

	//free the memory if buffed.
	if (isTempBuffer)
		freeBuffer(buffer,bufferSize);
	buffer = NULL;
	bufferSize = 0;

	//update status
	commStatus = CMR_COMM_STATUS_DONE;
}

