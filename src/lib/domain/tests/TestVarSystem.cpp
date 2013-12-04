/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <domain/CMRVarSystem.h>
#include <CMRDomainBuilder.h>
#include <common/CMRGeometry.h>

/**********************  USING  *********************/
using namespace testing;

/********************  MACRO  ***********************/
#define EXPECT_NO_NULL(x) EXPECT_NE((void*)NULL,(void*)(x))

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
		virtual int getLocalId ( void );
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

/*******************  FUNCTION  *********************/
int MockDomainBuilder::getLocalId ( void )
{
	return 0;
}

/********************  GLOBALS  *********************/
class TestVarSystem : public Test
{
	public:
		void SetUp(void);
		void TearDown(void);
	protected:
		CMRVarSystem * system;
		MockDomainBuilder  * domainBuilder;
};

/*******************  FUNCTION  *********************/
void TestVarSystem::SetUp(void)
{
	domainBuilder = new MockDomainBuilder();
	system = new CMRVarSystem(domainBuilder);
	system->addVariable("x",sizeof(float),1);
	system->addVariable("y",sizeof(float),1);
}

/*******************  FUNCTION  *********************/
void TestVarSystem::TearDown(void)
{
	delete domainBuilder;
	domainBuilder = NULL;
	delete system;
	system = NULL;
}

/*******************  FUNCTION  *********************/
TEST_F(TestVarSystem,testConstructorDestructor)
{
	//do nothing, only check except steup/teardown
}

/*******************  FUNCTION  *********************/
TEST_F(TestVarSystem,testAddVariable)
{
	CMRVariableId res = system->addVariable("z",sizeof(float),1);
	EXPECT_EQ(2,res);
	EXPECT_NO_NULL(system->getDomain(res,0));
};

/*******************  FUNCTION  *********************/
TEST_F(TestVarSystem,testGetDomain1)
{
	CMRDomainStorage * d1 = system->getDomain(0,0);
	EXPECT_NO_NULL(dynamic_cast<MockDomainStorage*>(d1));
	EXPECT_EQ(CMRRect(-1,-1,802,602),d1->getMemoryRect());
	EXPECT_EQ(CMRRect(0,0,800,600),d1->getGlobalRect());
	EXPECT_EQ(CMRRect(0,0,800,600),d1->getLocalDomainRect());
	EXPECT_EQ(1,d1->getGhostDepth());
	EXPECT_EQ(sizeof(float),d1->getTypeSize());
	EXPECT_FALSE(d1->hasMemoryAccessor());
};

/*******************  FUNCTION  *********************/
TEST_F(TestVarSystem,testGetDomain2)
{
	EXPECT_NO_NULL(dynamic_cast<MockDomainStorage*>(system->getDomain(0,0)));

	MockDomainStorage * d00 = (MockDomainStorage*)system->getDomain(0,0);
	MockDomainStorage * d01 = (MockDomainStorage*)system->getDomain(0,1);
	MockDomainStorage * d10 = (MockDomainStorage*)system->getDomain(1,0);
	MockDomainStorage * d11 = (MockDomainStorage*)system->getDomain(1,1);
	int id00 = d00->id;
	int id01 = d01->id;
	int id10 = d10->id;
	int id11 = d11->id;
	
	EXPECT_NE(d00,d01);
	EXPECT_NE(d00,d10);
	EXPECT_NE(d00,d11);
	
	MockDomainStorage * D00 = (MockDomainStorage*)system->getDomain(0,0);
	MockDomainStorage * D01 = (MockDomainStorage*)system->getDomain(0,1);
	MockDomainStorage * D10 = (MockDomainStorage*)system->getDomain(1,0);
	MockDomainStorage * D11 = (MockDomainStorage*)system->getDomain(1,1);
	
	EXPECT_NE(D00,D01);
	EXPECT_NE(D00,D10);
	EXPECT_NE(D00,D11);
	
	EXPECT_EQ(id00,D00->id);
	EXPECT_EQ(id01,D01->id);
	EXPECT_EQ(id10,D10->id);
	EXPECT_EQ(id11,D11->id);
};

/*******************  FUNCTION  *********************/
TEST_F(TestVarSystem,testFreeDomain1)
{
	EXPECT_NO_NULL(dynamic_cast<MockDomainStorage*>(system->getDomain(0,0)));

	MockDomainStorage * d00 = (MockDomainStorage*)system->getDomain(0,0);
	MockDomainStorage * d01 = (MockDomainStorage*)system->getDomain(0,1);
	MockDomainStorage * d10 = (MockDomainStorage*)system->getDomain(1,0);
	MockDomainStorage * d11 = (MockDomainStorage*)system->getDomain(1,1);
	int id00 = d00->id;
	int id01 = d01->id;
	int id10 = d10->id;
	int id11 = d11->id;
	
	EXPECT_NE(d00,d01);
	EXPECT_NE(d00,d10);
	EXPECT_NE(d00,d11);
	
	system->freeDomain(0,0);
	
	MockDomainStorage * D00 = (MockDomainStorage*)system->getDomain(0,0);
	MockDomainStorage * D01 = (MockDomainStorage*)system->getDomain(0,1);
	MockDomainStorage * D10 = (MockDomainStorage*)system->getDomain(1,0);
	MockDomainStorage * D11 = (MockDomainStorage*)system->getDomain(1,1);
	
	EXPECT_NE(D00,D01);
	EXPECT_NE(D00,D10);
	EXPECT_NE(D00,D11);
	
	EXPECT_NE(id00,D00->id);
	EXPECT_EQ(id01,D01->id);
	EXPECT_EQ(id10,D10->id);
	EXPECT_EQ(id11,D11->id);
};

/*******************  FUNCTION  *********************/
TEST_F(TestVarSystem,testFreeDomain2)
{
	EXPECT_NO_NULL(dynamic_cast<MockDomainStorage*>(system->getDomain(0,0)));

	MockDomainStorage * d00 = (MockDomainStorage*)system->getDomain(0,0);
	MockDomainStorage * d01 = (MockDomainStorage*)system->getDomain(0,1);
	MockDomainStorage * d10 = (MockDomainStorage*)system->getDomain(1,0);
	MockDomainStorage * d11 = (MockDomainStorage*)system->getDomain(1,1);
	int id00 = d00->id;
	int id01 = d01->id;
	int id10 = d10->id;
	int id11 = d11->id;
	
	EXPECT_NE(d00,d01);
	EXPECT_NE(d00,d10);
	EXPECT_NE(d00,d11);
	
	system->freeDomain(0,CMR_ALL);
	
	MockDomainStorage * D00 = (MockDomainStorage*)system->getDomain(0,0);
	MockDomainStorage * D01 = (MockDomainStorage*)system->getDomain(0,1);
	MockDomainStorage * D10 = (MockDomainStorage*)system->getDomain(1,0);
	MockDomainStorage * D11 = (MockDomainStorage*)system->getDomain(1,1);
	
	EXPECT_NE(D00,D01);
	EXPECT_NE(D00,D10);
	EXPECT_NE(D00,D11);
	
	EXPECT_NE(id00,D00->id);
	EXPECT_NE(id01,D01->id);
	EXPECT_EQ(id10,D10->id);
	EXPECT_EQ(id11,D11->id);
};

/*******************  FUNCTION  *********************/
TEST_F(TestVarSystem,testFreeDomain3)
{
	EXPECT_NO_NULL(dynamic_cast<MockDomainStorage*>(system->getDomain(0,0)));

	MockDomainStorage * d00 = (MockDomainStorage*)system->getDomain(0,0);
	MockDomainStorage * d01 = (MockDomainStorage*)system->getDomain(0,1);
	MockDomainStorage * d10 = (MockDomainStorage*)system->getDomain(1,0);
	MockDomainStorage * d11 = (MockDomainStorage*)system->getDomain(1,1);
	int id00 = d00->id;
	int id01 = d01->id;
	int id10 = d10->id;
	int id11 = d11->id;
	
	EXPECT_NE(d00,d01);
	EXPECT_NE(d00,d10);
	EXPECT_NE(d00,d11);
	
	system->freeDomain(CMR_ALL,0);
	
	MockDomainStorage * D00 = (MockDomainStorage*)system->getDomain(0,0);
	MockDomainStorage * D01 = (MockDomainStorage*)system->getDomain(0,1);
	MockDomainStorage * D10 = (MockDomainStorage*)system->getDomain(1,0);
	MockDomainStorage * D11 = (MockDomainStorage*)system->getDomain(1,1);
	
	EXPECT_NE(D00,D01);
	EXPECT_NE(D00,D10);
	EXPECT_NE(D00,D11);
	
	EXPECT_NE(id00,D00->id);
	EXPECT_EQ(id01,D01->id);
	EXPECT_NE(id10,D10->id);
	EXPECT_EQ(id11,D11->id);
};

/*******************  FUNCTION  *********************/
TEST_F(TestVarSystem,testFreeDomain4)
{
	EXPECT_NO_NULL(dynamic_cast<MockDomainStorage*>(system->getDomain(0,0)));

	MockDomainStorage * d00 = (MockDomainStorage*)system->getDomain(0,0);
	MockDomainStorage * d01 = (MockDomainStorage*)system->getDomain(0,1);
	MockDomainStorage * d10 = (MockDomainStorage*)system->getDomain(1,0);
	MockDomainStorage * d11 = (MockDomainStorage*)system->getDomain(1,1);
	int id00 = d00->id;
	int id01 = d01->id;
	int id10 = d10->id;
	int id11 = d11->id;
	
	EXPECT_NE(d00,d01);
	EXPECT_NE(d00,d10);
	EXPECT_NE(d00,d11);
	
	system->freeDomain(CMR_ALL,CMR_ALL);
	
	MockDomainStorage * D00 = (MockDomainStorage*)system->getDomain(0,0);
	MockDomainStorage * D01 = (MockDomainStorage*)system->getDomain(0,1);
	MockDomainStorage * D10 = (MockDomainStorage*)system->getDomain(1,0);
	MockDomainStorage * D11 = (MockDomainStorage*)system->getDomain(1,1);
	
	EXPECT_NE(D00,D01);
	EXPECT_NE(D00,D10);
	EXPECT_NE(D00,D11);
	
	EXPECT_NE(id00,D00->id);
	EXPECT_NE(id01,D01->id);
	EXPECT_NE(id10,D10->id);
	EXPECT_NE(id11,D11->id);
};

/*******************  FUNCTION  *********************/
TEST_F(TestVarSystem,testPermut1)
{
	MockDomainStorage * d00 = (MockDomainStorage*)system->getDomain(0,0);
	MockDomainStorage * d01 = (MockDomainStorage*)system->getDomain(0,1);
	MockDomainStorage * d10 = (MockDomainStorage*)system->getDomain(1,0);
	MockDomainStorage * d11 = (MockDomainStorage*)system->getDomain(1,1);
	
	EXPECT_NE(d00,d01);
	EXPECT_NE(d00,d10);
	EXPECT_NE(d00,d11);
	
	system->permutVar(0);
	
	MockDomainStorage * D00 = (MockDomainStorage*)system->getDomain(0,0);
	MockDomainStorage * D01 = (MockDomainStorage*)system->getDomain(0,1);
	MockDomainStorage * D10 = (MockDomainStorage*)system->getDomain(1,0);
	MockDomainStorage * D11 = (MockDomainStorage*)system->getDomain(1,1);
	
	EXPECT_EQ(d01,D00);
	EXPECT_EQ(d00,D01);
	EXPECT_EQ(d10,D10);
	EXPECT_EQ(d11,D11);
}

/*******************  FUNCTION  *********************/
TEST_F(TestVarSystem,testPermut2)
{
	MockDomainStorage * d00 = (MockDomainStorage*)system->getDomain(0,0);
	MockDomainStorage * d01 = (MockDomainStorage*)system->getDomain(0,1);
	MockDomainStorage * d10 = (MockDomainStorage*)system->getDomain(1,0);
	MockDomainStorage * d11 = (MockDomainStorage*)system->getDomain(1,1);
	
	EXPECT_NE(d00,d01);
	EXPECT_NE(d00,d10);
	EXPECT_NE(d00,d11);
	
	system->permutVar(CMR_ALL);
	
	MockDomainStorage * D00 = (MockDomainStorage*)system->getDomain(0,0);
	MockDomainStorage * D01 = (MockDomainStorage*)system->getDomain(0,1);
	MockDomainStorage * D10 = (MockDomainStorage*)system->getDomain(1,0);
	MockDomainStorage * D11 = (MockDomainStorage*)system->getDomain(1,1);
	
	EXPECT_EQ(d01,D00);
	EXPECT_EQ(d00,D01);
	EXPECT_EQ(d11,D10);
	EXPECT_EQ(d10,D11);
}
