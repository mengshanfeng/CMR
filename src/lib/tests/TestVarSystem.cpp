/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <svUnitTest.h>
#include <domain/CMRVarSystem.h>
#include "mocks/MockDomainBuilder.h"

/**********************  USING  *********************/
using namespace svUnitTest;

/********************  GLOBALS  *********************/
static CMRVarSystem * gblSystem = NULL;
static MockDomainBuilder gblDomainBuilder;

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_SETUP(TestVarSystem)
{
	gblSystem = new CMRVarSystem(&gblDomainBuilder);
	gblSystem->addVariable("x",sizeof(float),1);
	gblSystem->addVariable("y",sizeof(float),1);
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEAR_DOWN(TestVarSystem)
{
	delete gblSystem;
	gblSystem = NULL;
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestVarSystem,testConstructorDestructor)
{
	//do nothing, only check except steup/teardown
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestVarSystem,testAddVariable)
{
	CMRVariableId res = gblSystem->addVariable("z",sizeof(float),1);
	SVUT_ASSERT_EQUAL(2,res);
	SVUT_ASSERT_NOT_NULL(gblSystem->getDomain(res,0));
};

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestVarSystem,testGetDomain1)
{
	CMRDomainStorage * d1 = gblSystem->getDomain(0,0);
	SVUT_ASSERT_NOT_NULL(dynamic_cast<MockDomainStorage*>(d1));
	SVUT_ASSERT_EQUAL(CMRRect(-1,-1,802,602),d1->getMemoryRect());
	SVUT_ASSERT_EQUAL(CMRRect(0,0,800,600),d1->getGlobalRect());
	SVUT_ASSERT_EQUAL(CMRRect(0,0,800,600),d1->getLocalDomainRect());
	SVUT_ASSERT_EQUAL(1,d1->getGhostDepth());
	SVUT_ASSERT_EQUAL(sizeof(float),d1->getTypeSize());
	SVUT_ASSERT_FALSE(d1->hasMemoryAccessor());
};

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestVarSystem,testGetDomain2)
{
	SVUT_ASSERT_NOT_NULL(dynamic_cast<MockDomainStorage*>(gblSystem->getDomain(0,0)));

	MockDomainStorage * d00 = (MockDomainStorage*)gblSystem->getDomain(0,0);
	MockDomainStorage * d01 = (MockDomainStorage*)gblSystem->getDomain(0,1);
	MockDomainStorage * d10 = (MockDomainStorage*)gblSystem->getDomain(1,0);
	MockDomainStorage * d11 = (MockDomainStorage*)gblSystem->getDomain(1,1);
	int id00 = d00->id;
	int id01 = d01->id;
	int id10 = d10->id;
	int id11 = d11->id;
	
	SVUT_ASSERT_NOT_SAME(d00,d01);
	SVUT_ASSERT_NOT_SAME(d00,d10);
	SVUT_ASSERT_NOT_SAME(d00,d11);
	
	MockDomainStorage * D00 = (MockDomainStorage*)gblSystem->getDomain(0,0);
	MockDomainStorage * D01 = (MockDomainStorage*)gblSystem->getDomain(0,1);
	MockDomainStorage * D10 = (MockDomainStorage*)gblSystem->getDomain(1,0);
	MockDomainStorage * D11 = (MockDomainStorage*)gblSystem->getDomain(1,1);
	
	SVUT_ASSERT_NOT_SAME(D00,D01);
	SVUT_ASSERT_NOT_SAME(D00,D10);
	SVUT_ASSERT_NOT_SAME(D00,D11);
	
	SVUT_ASSERT_EQUAL(id00,D00->id);
	SVUT_ASSERT_EQUAL(id01,D01->id);
	SVUT_ASSERT_EQUAL(id10,D10->id);
	SVUT_ASSERT_EQUAL(id11,D11->id);
};

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestVarSystem,testFreeDomain1)
{
	SVUT_ASSERT_NOT_NULL(dynamic_cast<MockDomainStorage*>(gblSystem->getDomain(0,0)));

	MockDomainStorage * d00 = (MockDomainStorage*)gblSystem->getDomain(0,0);
	MockDomainStorage * d01 = (MockDomainStorage*)gblSystem->getDomain(0,1);
	MockDomainStorage * d10 = (MockDomainStorage*)gblSystem->getDomain(1,0);
	MockDomainStorage * d11 = (MockDomainStorage*)gblSystem->getDomain(1,1);
	int id00 = d00->id;
	int id01 = d01->id;
	int id10 = d10->id;
	int id11 = d11->id;
	
	SVUT_ASSERT_NOT_SAME(d00,d01);
	SVUT_ASSERT_NOT_SAME(d00,d10);
	SVUT_ASSERT_NOT_SAME(d00,d11);
	
	gblSystem->freeDomain(0,0);
	
	MockDomainStorage * D00 = (MockDomainStorage*)gblSystem->getDomain(0,0);
	MockDomainStorage * D01 = (MockDomainStorage*)gblSystem->getDomain(0,1);
	MockDomainStorage * D10 = (MockDomainStorage*)gblSystem->getDomain(1,0);
	MockDomainStorage * D11 = (MockDomainStorage*)gblSystem->getDomain(1,1);
	
	SVUT_ASSERT_NOT_SAME(D00,D01);
	SVUT_ASSERT_NOT_SAME(D00,D10);
	SVUT_ASSERT_NOT_SAME(D00,D11);
	
	SVUT_ASSERT_NOT_EQUAL(id00,D00->id);
	SVUT_ASSERT_EQUAL(id01,D01->id);
	SVUT_ASSERT_EQUAL(id10,D10->id);
	SVUT_ASSERT_EQUAL(id11,D11->id);
};

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestVarSystem,testFreeDomain2)
{
	SVUT_ASSERT_NOT_NULL(dynamic_cast<MockDomainStorage*>(gblSystem->getDomain(0,0)));

	MockDomainStorage * d00 = (MockDomainStorage*)gblSystem->getDomain(0,0);
	MockDomainStorage * d01 = (MockDomainStorage*)gblSystem->getDomain(0,1);
	MockDomainStorage * d10 = (MockDomainStorage*)gblSystem->getDomain(1,0);
	MockDomainStorage * d11 = (MockDomainStorage*)gblSystem->getDomain(1,1);
	int id00 = d00->id;
	int id01 = d01->id;
	int id10 = d10->id;
	int id11 = d11->id;
	
	SVUT_ASSERT_NOT_SAME(d00,d01);
	SVUT_ASSERT_NOT_SAME(d00,d10);
	SVUT_ASSERT_NOT_SAME(d00,d11);
	
	gblSystem->freeDomain(0,CMR_ALL);
	
	MockDomainStorage * D00 = (MockDomainStorage*)gblSystem->getDomain(0,0);
	MockDomainStorage * D01 = (MockDomainStorage*)gblSystem->getDomain(0,1);
	MockDomainStorage * D10 = (MockDomainStorage*)gblSystem->getDomain(1,0);
	MockDomainStorage * D11 = (MockDomainStorage*)gblSystem->getDomain(1,1);
	
	SVUT_ASSERT_NOT_SAME(D00,D01);
	SVUT_ASSERT_NOT_SAME(D00,D10);
	SVUT_ASSERT_NOT_SAME(D00,D11);
	
	SVUT_ASSERT_NOT_EQUAL(id00,D00->id);
	SVUT_ASSERT_NOT_EQUAL(id01,D01->id);
	SVUT_ASSERT_EQUAL(id10,D10->id);
	SVUT_ASSERT_EQUAL(id11,D11->id);
};

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestVarSystem,testFreeDomain3)
{
	SVUT_ASSERT_NOT_NULL(dynamic_cast<MockDomainStorage*>(gblSystem->getDomain(0,0)));

	MockDomainStorage * d00 = (MockDomainStorage*)gblSystem->getDomain(0,0);
	MockDomainStorage * d01 = (MockDomainStorage*)gblSystem->getDomain(0,1);
	MockDomainStorage * d10 = (MockDomainStorage*)gblSystem->getDomain(1,0);
	MockDomainStorage * d11 = (MockDomainStorage*)gblSystem->getDomain(1,1);
	int id00 = d00->id;
	int id01 = d01->id;
	int id10 = d10->id;
	int id11 = d11->id;
	
	SVUT_ASSERT_NOT_SAME(d00,d01);
	SVUT_ASSERT_NOT_SAME(d00,d10);
	SVUT_ASSERT_NOT_SAME(d00,d11);
	
	gblSystem->freeDomain(CMR_ALL,0);
	
	MockDomainStorage * D00 = (MockDomainStorage*)gblSystem->getDomain(0,0);
	MockDomainStorage * D01 = (MockDomainStorage*)gblSystem->getDomain(0,1);
	MockDomainStorage * D10 = (MockDomainStorage*)gblSystem->getDomain(1,0);
	MockDomainStorage * D11 = (MockDomainStorage*)gblSystem->getDomain(1,1);
	
	SVUT_ASSERT_NOT_SAME(D00,D01);
	SVUT_ASSERT_NOT_SAME(D00,D10);
	SVUT_ASSERT_NOT_SAME(D00,D11);
	
	SVUT_ASSERT_NOT_EQUAL(id00,D00->id);
	SVUT_ASSERT_EQUAL(id01,D01->id);
	SVUT_ASSERT_NOT_EQUAL(id10,D10->id);
	SVUT_ASSERT_EQUAL(id11,D11->id);
};

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestVarSystem,testFreeDomain4)
{
	SVUT_ASSERT_NOT_NULL(dynamic_cast<MockDomainStorage*>(gblSystem->getDomain(0,0)));

	MockDomainStorage * d00 = (MockDomainStorage*)gblSystem->getDomain(0,0);
	MockDomainStorage * d01 = (MockDomainStorage*)gblSystem->getDomain(0,1);
	MockDomainStorage * d10 = (MockDomainStorage*)gblSystem->getDomain(1,0);
	MockDomainStorage * d11 = (MockDomainStorage*)gblSystem->getDomain(1,1);
	int id00 = d00->id;
	int id01 = d01->id;
	int id10 = d10->id;
	int id11 = d11->id;
	
	SVUT_ASSERT_NOT_SAME(d00,d01);
	SVUT_ASSERT_NOT_SAME(d00,d10);
	SVUT_ASSERT_NOT_SAME(d00,d11);
	
	gblSystem->freeDomain(CMR_ALL,CMR_ALL);
	
	MockDomainStorage * D00 = (MockDomainStorage*)gblSystem->getDomain(0,0);
	MockDomainStorage * D01 = (MockDomainStorage*)gblSystem->getDomain(0,1);
	MockDomainStorage * D10 = (MockDomainStorage*)gblSystem->getDomain(1,0);
	MockDomainStorage * D11 = (MockDomainStorage*)gblSystem->getDomain(1,1);
	
	SVUT_ASSERT_NOT_SAME(D00,D01);
	SVUT_ASSERT_NOT_SAME(D00,D10);
	SVUT_ASSERT_NOT_SAME(D00,D11);
	
	SVUT_ASSERT_NOT_EQUAL(id00,D00->id);
	SVUT_ASSERT_NOT_EQUAL(id01,D01->id);
	SVUT_ASSERT_NOT_EQUAL(id10,D10->id);
	SVUT_ASSERT_NOT_EQUAL(id11,D11->id);
};

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestVarSystem,testPermut1)
{
	MockDomainStorage * d00 = (MockDomainStorage*)gblSystem->getDomain(0,0);
	MockDomainStorage * d01 = (MockDomainStorage*)gblSystem->getDomain(0,1);
	MockDomainStorage * d10 = (MockDomainStorage*)gblSystem->getDomain(1,0);
	MockDomainStorage * d11 = (MockDomainStorage*)gblSystem->getDomain(1,1);
	
	SVUT_ASSERT_NOT_SAME(d00,d01);
	SVUT_ASSERT_NOT_SAME(d00,d10);
	SVUT_ASSERT_NOT_SAME(d00,d11);
	
	gblSystem->permutVar(0);
	
	MockDomainStorage * D00 = (MockDomainStorage*)gblSystem->getDomain(0,0);
	MockDomainStorage * D01 = (MockDomainStorage*)gblSystem->getDomain(0,1);
	MockDomainStorage * D10 = (MockDomainStorage*)gblSystem->getDomain(1,0);
	MockDomainStorage * D11 = (MockDomainStorage*)gblSystem->getDomain(1,1);
	
	SVUT_ASSERT_SAME(d01,D00);
	SVUT_ASSERT_SAME(d00,D01);
	SVUT_ASSERT_SAME(d10,D10);
	SVUT_ASSERT_SAME(d11,D11);
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestVarSystem,testPermut2)
{
	MockDomainStorage * d00 = (MockDomainStorage*)gblSystem->getDomain(0,0);
	MockDomainStorage * d01 = (MockDomainStorage*)gblSystem->getDomain(0,1);
	MockDomainStorage * d10 = (MockDomainStorage*)gblSystem->getDomain(1,0);
	MockDomainStorage * d11 = (MockDomainStorage*)gblSystem->getDomain(1,1);
	
	SVUT_ASSERT_NOT_SAME(d00,d01);
	SVUT_ASSERT_NOT_SAME(d00,d10);
	SVUT_ASSERT_NOT_SAME(d00,d11);
	
	gblSystem->permutVar(CMR_ALL);
	
	MockDomainStorage * D00 = (MockDomainStorage*)gblSystem->getDomain(0,0);
	MockDomainStorage * D01 = (MockDomainStorage*)gblSystem->getDomain(0,1);
	MockDomainStorage * D10 = (MockDomainStorage*)gblSystem->getDomain(1,0);
	MockDomainStorage * D11 = (MockDomainStorage*)gblSystem->getDomain(1,1);
	
	SVUT_ASSERT_SAME(d01,D00);
	SVUT_ASSERT_SAME(d00,D01);
	SVUT_ASSERT_SAME(d11,D10);
	SVUT_ASSERT_SAME(d10,D11);
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
