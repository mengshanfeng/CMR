/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_MPI_REDUCTION_H
#define CMR_MPI_REDUCTION_H

/********************  HEADERS  *********************/
#include <map>
#include <mpi.h>
#include "../communication/CMRComm.h"
#include "../communication/CMRReductionDescriptor.h"
#include "../common/CMRCommon.h"

/*********************  TYPES  **********************/
typedef std::map<CMRUserReduceOperator*,MPI_Op> CMRMPIOpMap;

/*********************  CLASS  **********************/
class CMRMPIReduction : public CMRComm
{
	public:
		CMRMPIReduction(CMRReductionDescriptor * descriptor,int root = CMR_ALL);
		virtual std::string getDebugString ( void ) const;
		virtual void run ( void );
		MPI_Op & getMpiOperation(void);
	private:
		void runCustom(void);
		void runNative(void);
		bool isNative(void);
	private:
		CMRReductionDescriptor * descriptor;
		int root;
		static CMRMPIOpMap mpiOps;
};

#endif // CMR_MPI_REDUCTION_H
