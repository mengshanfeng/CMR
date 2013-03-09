/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef MOCK_DOMAIN_BUILDER_H
#define MOCK_DOMAIN_BUILDER_H

/*********************  CLASS  **********************/
#include <domain/CMRVarSystem.h>
#include <domain/CMRDomainBuilder.h>

/*********************  CLASS  **********************/
class MockDomainStorage : public CMRDomainStorage
{
	public:
		MockDomainStorage ( size_t typeSize, const CMRRect& localDomain, int ghostDepth, int globalWidth = -1, int globalHeight = -1 );
		int id;
};

/*********************  CLASS  **********************/
class MockDomainBuilder : public CMRDomainBuilder
{
	public:
		virtual CMRDomainStorage* buildDomain ( const CMRVariable& variable );
		int cnt;
};

/*******************  FUNCTION  *********************/
MockDomainStorage::MockDomainStorage ( size_t typeSize, const CMRRect& localDomain, int ghostDepth, int globalWidth, int globalHeight )
	: CMRDomainStorage ( typeSize, localDomain, ghostDepth, globalWidth, globalHeight )
{
	this->id = 0;
}

/*******************  FUNCTION  *********************/
CMRDomainStorage* MockDomainBuilder::buildDomain ( const CMRVariable& variable )
{
	MockDomainStorage * res = new MockDomainStorage(sizeof(float),CMRRect(0,0,800,600),1);
	res->id = cnt++;
	return res;
}

#endif // MOCKCOMMUNICATOR_H
