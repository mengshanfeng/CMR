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
#include <CMRCommunicator.h>
#include <CMRCommunication.h>
#include <CMRGeometry.h>

/*********************  CLASS  **********************/
class MockCommunication : public CMRCommunication
{
	public:
		CMRRect2D rect;
		int dest;
		int id;
};

/*********************  CLASS  **********************/
class MockCommunicator : public CMRCommunicator
{
	public:
		virtual CMRCommunication* createComm ( CMRAbstractDomain* domain, const CMRRect2D& rect, CMRCommType commType );
		int dest;
		int id;
};

#endif // MOCKCOMMUNICATOR_H
