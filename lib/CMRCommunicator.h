/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMMUNICATOR_H
#define CMR_COMMUNICATOR_H

/********************  HEADERS  *********************/
#include <stdlib.h>
#include "CMRCommon.h"

/*********************  CLASS  **********************/
class CMRCommunication;
class CMRAbstractDomain;
class CMRRect2D;

/*********************  CLASS  **********************/
class CMRCommunicator
{
	public:
		virtual ~CMRCommunicator(void);
		virtual CMRCommunication * createBufferedCommunication(CMRAbstractDomain * domain,const CMRRect2D & rect,CMRCommType commType) = 0;
};

#endif // CMR_COMMUNICATOR_H
