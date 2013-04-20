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
#include "../common/CMRCommon.h"

/*********************  CLASS  **********************/
class CMRComm;
class CMRAbstractDomain;
struct CMRRect;

/*********************  CLASS  **********************/
class CMRCommFactory
{
	public:
		virtual ~CMRCommFactory(void);
		virtual CMRComm * createComm(CMRAbstractDomain * domain,const CMRRect & rect,CMRCommType commType) = 0;
};

#endif // CMR_COMMUNICATOR_H
