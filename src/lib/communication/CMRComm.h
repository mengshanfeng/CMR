/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMMUNICATION_H
#define CMR_COMMUNICATION_H

/********************  HEADERS  *********************/
#include <string>

/********************  ENUM  ************************/
enum CMRCommStatus
{
	CMR_COMM_STATUS_INIT    = 0,
	CMR_COMM_STATUS_STARTED = 1,
	CMR_COMM_STATUS_DONE    = 2
};

/*********************  CLASS  **********************/
class CMRComm
{
	public:
		virtual ~CMRComm(void);
		virtual std::string getDebugString(void) const;
		virtual void run(void) = 0;
};

#endif // CMR_COMMUNICATION_H
