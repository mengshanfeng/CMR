/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef TEST_MOCK_COMM_FACTORY_H
#define TEST_MOCK_COMM_FACTORY_H

/********************  HEADERS  *********************/
#include <gmock/gmock.h>
#include <communication/CMRCommFactory.h>

/*********************  CLASS  **********************/
class MockCommFactory : public CMRCommFactory
{
	public:
		MOCK_METHOD3(createComm, CMRComm* ( CMRAbstractDomain* domain, const CMRRect& rect, CMRCommType commType ));
};

#endif //TEST_MOCK_COMM_FACTORY_H
