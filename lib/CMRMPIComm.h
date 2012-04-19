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
#include "CMRCommon.h"
#include "CMRGeometry.h"
#include "CMRComm.h"

/*********************  CLASS  **********************/
class CMRAbstractDomain;
class CMRMPICommFactory;

/*********************  CLASS  **********************/
class CMRMPIComm : public CMRComm
{
	public:
		CMRMPIComm(CMRMPICommFactory * communicator,CMRAbstractDomain* domain, const CMRRect2D& rect, CMRCommType commType);
		virtual void run(void );
	protected:
		void runSend(void);
		void runReceive(void);
	private:
		CMRMPICommFactory * communicator;
		CMRAbstractDomain* domain;
		CMRRect2D rect;
		CMRCommType commType;
		enum CMRCommStatus commStatus;
};

#endif // CMR_MPI_COMMUNICATION_H
