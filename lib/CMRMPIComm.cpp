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
#include "CMRDebug.h"
#include "CMRMPIComm.h"
#include "CMRAbstractDomain.h"
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
	int res = MPI_Recv(buffer,size,MPI_CHAR,communicator->getTargetId(),communicator->getSendId(),MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	assume(res == 0,"MPI_Recv failure : %d",res);
}

/*******************  FUNCTION  *********************/
void CMRMPIComm::runSend ( void* buffer, size_t size )
{
	int res = MPI_Send(buffer,size,MPI_CHAR,communicator->getTargetId(),communicator->getSendId(),MPI_COMM_WORLD);
	assume(res == 0,"MPI_Send failure : %d",res);
}
