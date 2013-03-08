/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <svUnitTest.h>
#include <domain/CMRMemoryModels.h>
#include <domain/CMRCellAccessor.h>

/**********************  USING  *********************/
using namespace svUnitTest;

/*********************  CONSTS  *********************/
static const int CST_WIDTH = 20;
static const int CST_HEIGHT = 20;
static const CMRRect CST_MM_RECT(0,0,CST_WIDTH,CST_HEIGHT);

/********************  GLOBALS  *********************/
static float gblBuffer[CST_HEIGHT][CST_WIDTH];
static CMRDomainMemory gblDomainMemory(gblBuffer,CST_MM_RECT);

/*********************  TYPES  **********************/
typedef CMRCellAccessor<float,CMRMemoryModelRowMajor> CMRCellAccessorFRM;

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestCellAccessor,testTypeSize)
{
	CMRCellAccessorFRM mm(gblDomainMemory,0,0);
	SVUT_ASSERT_EQUAL(sizeof(float),mm.getTypeSize());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestCellAccessor,testConstructor1)
{
	const CMRCellAccessorFRM mm(gblDomainMemory,0,0);
	SVUT_ASSERT_SAME(gblBuffer,&mm.getCell(0,0));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestCellAccessor,testConstructor2)
{
	const CMRCellAccessorFRM mm(gblDomainMemory,1,0);
	SVUT_ASSERT_SAME(gblBuffer,&mm.getCell(-1,0));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestCellAccessor,testConstructor3)
{
	const CMRCellAccessorFRM mm(gblDomainMemory,0,1);
	SVUT_ASSERT_SAME(gblBuffer,&mm.getCell(0,-1));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestCellAccessor,testConstructor4)
{
	const CMRCellAccessorFRM mm1(gblDomainMemory,1,2);
	const CMRCellAccessorFRM mm2(mm1,1,2);
	SVUT_ASSERT_SAME(gblBuffer,&mm2.getCell(-2,-4));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestCellAccessor,testGetCell)
{
	CMRCellAccessorFRM mm(gblDomainMemory,0,0);
	SVUT_ASSERT_SAME(&gblBuffer[3][2],&mm.getCell(2,3));
	SVUT_ASSERT_SAME(&gblBuffer[2][3],&mm.getCell(3,2));
	
	//try write
	mm.getCell(2,3) = 2;
	SVUT_ASSERT_EQUAL(2,gblBuffer[3][2]);
	mm.getCell(2,3) = 4;
	SVUT_ASSERT_EQUAL(4,gblBuffer[3][2]);
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestCellAccessor,testGetCellConst1)
{
	const CMRCellAccessorFRM mm(gblDomainMemory,0,0);
	SVUT_ASSERT_SAME(&gblBuffer[3][2],&mm.getCell(2,3));
	SVUT_ASSERT_SAME(&gblBuffer[2][3],&mm.getCell(3,2));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestCellAccessor,testGetCellConst2)
{
	CMRDomainMemory domain(gblBuffer,CMRRect(-2,-2,CST_WIDTH,CST_HEIGHT));
	const CMRCellAccessorFRM mm(domain,0,0,true);
	SVUT_ASSERT_SAME(&gblBuffer[3][2],&mm.getCell(0,1));
	SVUT_ASSERT_SAME(&gblBuffer[2][3],&mm.getCell(1,0));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestCellAccessor,testGetCellNotOrig1)
{
	const CMRCellAccessorFRM mm(gblDomainMemory,2,3);
	SVUT_ASSERT_SAME(&gblBuffer[6][4],&mm.getCell(2,3));
	SVUT_ASSERT_SAME(&gblBuffer[5][5],&mm.getCell(3,2));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestCellAccessor,testGetCellNotOrig2)
{
	static CMRDomainMemory domainMemory(gblBuffer,CMRRect(10,10,CST_WIDTH,CST_HEIGHT));
	CMRCellAccessorFRM mm(domainMemory,0,0);
	SVUT_ASSERT_SAME(&gblBuffer[3][2],&mm.getCell(2,3));
	SVUT_ASSERT_SAME(&gblBuffer[2][3],&mm.getCell(3,2));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestCellAccessor,testGetCellNotOrig3)
{
	static CMRDomainMemory domainMemory(gblBuffer,CMRRect(10,10,CST_WIDTH,CST_HEIGHT));
	CMRCellAccessorFRM mm(domainMemory,2,3);
	SVUT_ASSERT_SAME(&gblBuffer[6][4],&mm.getCell(2,3));
	SVUT_ASSERT_SAME(&gblBuffer[5][5],&mm.getCell(3,2));
}


/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestCellAccessor,testIsContiguous)
{
	const CMRCellAccessorFRM mm(gblDomainMemory,0,0);
	SVUT_ASSERT_TRUE(mm.isContiguous(CMRRect(0,0,CST_WIDTH,1)));
	SVUT_ASSERT_TRUE(mm.isContiguous(CMRRect(0,0,CST_WIDTH,2)));
	SVUT_ASSERT_FALSE(mm.isContiguous(CMRRect(0,0,CST_WIDTH/2,2)));
	SVUT_ASSERT_FALSE(mm.isContiguous(CMRRect(0,0,1,CST_HEIGHT)));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestCellAccessor,testGetAbsPosition)
{
	const CMRCellAccessorFRM mm(gblDomainMemory,3,5);
	SVUT_ASSERT_EQUAL(CMRVect2D(3,5), mm.getAbsPosition());
	SVUT_ASSERT_EQUAL(CMRVect2D(4,7), mm.getAbsPosition(1,2));
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
