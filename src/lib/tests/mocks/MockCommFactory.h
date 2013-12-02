/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef MOCK_COMMUNICATOR_H
#define MOCK_COMMUNICATOR_H

/*********************  CLASS  **********************/
#include <communication/CMRCommFactory.h>
#include <communication/CMRComm.h>
#include <common/CMRGeometry.h>

/*********************  CLASS  **********************/
class MockComm : public CMRComm
{
	public:
    virtual void run(void );
		CMRRect rect;
		int dest;
		int id;
		bool hasRun;
};

/*********************  CLASS  **********************/
class MockCommFactory : public CMRCommFactory
{
	public:
		virtual CMRComm* createComm ( CMRAbstractDomain* domain, const CMRRect& rect, CMRCommType commType );
		int dest;
		int id;
};

#endif // MOCKCOMMUNICATOR_H
