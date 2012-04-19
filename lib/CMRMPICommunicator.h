/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_MPI_COMMUNICATOR_H
#define CMR_MPI_COMMUNICATOR_H

/********************  HEADERS  *********************/
#include "CMRCommunicator.h"

/*********************  CLASS  **********************/
class CMRMPICommunicator : public CMRCommunicator
{
	public:
		CMRMPICommunicator(int targetId,int sendId,int receiveId);
		virtual CMRCommunication* createComm ( CMRAbstractDomain* domain, const CMRRect2D& rect, CMRCommType commType );
		int getTargetId(void) const;
		int getSendId(void) const;
		int getReceiveId(void) const;
	private:
		int targetId;
		int sendId;
		int receiveId;
};

#endif // CMR_MPI_COMMUNICATOR_H
