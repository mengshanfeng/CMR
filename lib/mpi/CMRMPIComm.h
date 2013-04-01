/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_MPI_COMMUNICATION_H
#define CMR_MPI_COMMUNICATION_H

/********************  HEADERS  *********************/
#include "../communication/CMRBufferedComm.h"

/*********************  CLASS  **********************/
class CMRAbstractDomain;
class CMRMPICommFactory;

/*********************  CLASS  **********************/
class CMRMPIComm : public CMRBufferedComm
{
	public:
		CMRMPIComm(CMRMPICommFactory * communicator,CMRAbstractDomain* domain, const CMRRect& rect, CMRCommType commType);
		virtual std::string getDebugString ( void ) const;
	protected:
		virtual void runReceive ( void* buffer, size_t size );
		virtual void runSend ( void* buffer, size_t size );
	private:
		CMRMPICommFactory * communicator;
};

#endif // CMR_MPI_COMMUNICATION_H
