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
#include <domain/CMRDomainBuilder.h>
#include <domain/CMRDomainStorage.h>

/*********************  CLASS  **********************/
class MockDomainBuilder : public CMRDomainBuilder
{
	public:
		MOCK_METHOD1(buildDomain,CMRDomainStorage*( const CMRVariable& variable ));
		MOCK_METHOD0(getLocalId,int(void));
};

#endif //TEST_MOCK_COMM_FACTORY_H
