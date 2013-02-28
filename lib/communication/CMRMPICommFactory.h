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
		CMRMPICommFactory(int sendToRank,int recvFromRank,int tagBase);
		virtual CMRComm* createComm ( CMRAbstractDomain* domain, const CMRRect& rect, CMRCommType commType );
		int getTagBase(void) const;
		int getSendRank(void) const;
		int getRecvRank(void) const;
	private:
		int sendRank;
		int recvRank;
		int tagBase;
};

#endif // CMR_MPI_COMMUNICATOR_H
