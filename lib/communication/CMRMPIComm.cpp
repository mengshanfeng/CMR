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
#include <cstdio>
#include <mpi.h>
#include "../common/CMRDebug.h"
#include "CMRMPIComm.h"
#include "../common/CMRCommon.h"
#include "../domain/CMRAbstractDomain.h"
#include "CMRMPICommFactory.h"

/*******************  FUNCTION  *********************/
CMRMPIComm::CMRMPIComm ( CMRMPICommFactory* communicator, CMRAbstractDomain* domain, const CMRRect2D& rect, CMRCommType commType )
	: CMRBufferedComm ( domain, rect, commType )
{
	//errors
	assert(communicator != NULL);

	//set default
	this->communicator = communicator;
}

/*******************  FUNCTION  *********************/
void CMRMPIComm::runReceive ( void* buffer, size_t size )
{
	debug("Start MPI_Recv( %2d <-- %2d [tag = %d , size = %lu] )",cmrGetMPIRank(),communicator->getRecvRank(),communicator->getTagBase(),size);
	int res = MPI_Recv(buffer,size,MPI_CHAR,communicator->getRecvRank(),communicator->getTagBase(),MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	assume(res == 0,"MPI_Recv failure : %d",res);
}

/*******************  FUNCTION  *********************/
void CMRMPIComm::runSend ( void* buffer, size_t size )
{
	debug("Start MPI_Send( %2d --> %2d [tag = %d , size = %lu] )",cmrGetMPIRank(),communicator->getSendRank(),communicator->getTagBase(),size);
	int res = MPI_Send(buffer,size,MPI_CHAR,communicator->getSendRank(),communicator->getTagBase(),MPI_COMM_WORLD);
	assume(res == 0,"MPI_Send failure : %d",res);
}

/*******************  FUNCTION  *********************/
std::string CMRMPIComm::getDebugString ( void ) const
{
	char buffer[1024];
	size_t size;

	switch(this->getCommType())
	{
		case CMR_COMM_SEND:
			size = sprintf(buffer,"MPI_Send( %2d --> %2d [tag = %d , size = %lu] )",cmrGetMPIRank(),communicator->getSendRank(),communicator->getTagBase(),this->getBufferSize());
			assert(size < sizeof(buffer));
			break;
		case CMR_COMM_RECV:
			size = sprintf(buffer,"MPI_Recv( %2d <-- %2d [tag = %d , size = %lu] )",cmrGetMPIRank(),communicator->getRecvRank(),communicator->getTagBase(),this->getBufferSize());
			assert(size < sizeof(buffer));
			break;
		default:
			fatal("Invalid communication type : %d\n",this->getCommType());
	}
	
	return buffer;
}
