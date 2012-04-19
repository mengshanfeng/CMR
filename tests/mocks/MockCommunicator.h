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
#include <CMRCommFactory.h>
#include <CMRComm.h>
#include <CMRGeometry.h>

/*********************  CLASS  **********************/
class MockCommunication : public CMRComm
{
	public:
    virtual void run(void );
		CMRRect2D rect;
		int dest;
		int id;
		bool hasRun;
};

/*********************  CLASS  **********************/
class MockCommunicator : public CMRCommFactory
{
	public:
		virtual CMRComm* createComm ( CMRAbstractDomain* domain, const CMRRect2D& rect, CMRCommType commType );
		int dest;
		int id;
};

#endif // MOCKCOMMUNICATOR_H
