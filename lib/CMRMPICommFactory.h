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
#include "CMRCommFactory.h"

/*********************  CLASS  **********************/
class CMRMPICommFactory : public CMRCommFactory
{
	public:
		CMRMPICommFactory(int targetId,int sendId,int receiveId);
		virtual CMRComm* createComm ( CMRAbstractDomain* domain, const CMRRect2D& rect, CMRCommType commType );
		int getTargetId(void) const;
		int getSendId(void) const;
		int getReceiveId(void) const;
	private:
		int targetId;
		int sendId;
		int receiveId;
};

#endif // CMR_MPI_COMMUNICATOR_H
