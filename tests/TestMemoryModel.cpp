/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cstring>
#include <svUnitTest.h>
#include <domain/CMRMemoryModel.h>

/**********************  USING  *********************/
using namespace svUnitTest;

static const int CST_WIDTH = 20;
static const int CST_HEIGHT = 20;
static float gblBuffer[CST_HEIGHT][CST_WIDTH];
static const CMRRect CST_MM_RECT(0,0,CST_WIDTH,CST_HEIGHT);
static CMRDomainMemory gblDomainMemory(gblBuffer,CST_MM_RECT);
typedef CMRMemoryAccessor<float,CMRMemoryModelRowMajor> CMRMemoryAccessorFRM;

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestMemoryModelRowMajor,testGetCellId)
{
	SVUT_ASSERT_EQUAL(0,CMRMemoryModelRowMajor::getCellId(0,0,CST_WIDTH,CST_HEIGHT));
	SVUT_ASSERT_EQUAL(1,CMRMemoryModelRowMajor::getCellId(1,0,CST_WIDTH,CST_HEIGHT));
	SVUT_ASSERT_EQUAL(CST_WIDTH,CMRMemoryModelRowMajor::getCellId(0,1,CST_WIDTH,CST_HEIGHT));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestMemoryModelRowMajor,testGetCellRelId)
{
	SVUT_ASSERT_EQUAL(0,CMRMemoryModelRowMajor::getRelCellId(0,0,CST_WIDTH,CST_HEIGHT,0,0));
	SVUT_ASSERT_EQUAL(1,CMRMemoryModelRowMajor::getRelCellId(1,0,CST_WIDTH,CST_HEIGHT,0,0));
	SVUT_ASSERT_EQUAL(CST_WIDTH,CMRMemoryModelRowMajor::getRelCellId(0,1,CST_WIDTH,CST_HEIGHT,0,0));
	
	SVUT_ASSERT_EQUAL(0,CMRMemoryModelRowMajor::getRelCellId(0,0,CST_WIDTH,CST_HEIGHT,10,15));
	SVUT_ASSERT_EQUAL(1,CMRMemoryModelRowMajor::getRelCellId(1,0,CST_WIDTH,CST_HEIGHT,10,15));
	SVUT_ASSERT_EQUAL(CST_WIDTH,CMRMemoryModelRowMajor::getRelCellId(0,1,CST_WIDTH,CST_HEIGHT,10,15));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestMemoryModelRowMajor,testIsContiguous)
{
	SVUT_ASSERT_TRUE(CMRMemoryModelRowMajor::isContiguous(CST_MM_RECT,CMRRect(0,0,CST_WIDTH,1)));
	SVUT_ASSERT_TRUE(CMRMemoryModelRowMajor::isContiguous(CST_MM_RECT,CMRRect(0,0,CST_WIDTH,2)));
	SVUT_ASSERT_FALSE(CMRMemoryModelRowMajor::isContiguous(CST_MM_RECT,CMRRect(0,0,CST_WIDTH/2,2)));
	SVUT_ASSERT_FALSE(CMRMemoryModelRowMajor::isContiguous(CST_MM_RECT,CMRRect(0,0,1,CST_HEIGHT)));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestMemoryModel,testTypeSize)
{
	CMRMemoryAccessorFRM mm(gblDomainMemory,0,0);
	SVUT_ASSERT_EQUAL(sizeof(float),mm.getTypeSize());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestMemoryModel,testConstructor1)
{
	const CMRMemoryAccessorFRM mm(gblDomainMemory,0,0);
	SVUT_ASSERT_SAME(gblBuffer,&mm.getCell(0,0));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestMemoryModel,testConstructor2)
{
	const CMRMemoryAccessorFRM mm(gblDomainMemory,1,0);
	SVUT_ASSERT_SAME(gblBuffer,&mm.getCell(-1,0));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestMemoryModel,testConstructor3)
{
	const CMRMemoryAccessorFRM mm(gblDomainMemory,0,1);
	SVUT_ASSERT_SAME(gblBuffer,&mm.getCell(0,-1));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestMemoryModel,testConstructor4)
{
	const CMRMemoryAccessorFRM mm1(gblDomainMemory,1,2);
	const CMRMemoryAccessorFRM mm2(mm1,1,2);
	SVUT_ASSERT_SAME(gblBuffer,&mm2.getCell(-2,-4));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestMemoryModel,testGetCell)
{
	CMRMemoryAccessorFRM mm(gblDomainMemory,0,0);
	SVUT_ASSERT_SAME(&gblBuffer[3][2],&mm.getCell(2,3));
	SVUT_ASSERT_SAME(&gblBuffer[2][3],&mm.getCell(3,2));
	
	//try write
	mm.getCell(2,3) = 2;
	SVUT_ASSERT_EQUAL(2,gblBuffer[3][2]);
	mm.getCell(2,3) = 4;
	SVUT_ASSERT_EQUAL(4,gblBuffer[3][2]);
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestMemoryModel,testGetCellConst)
{
	const CMRMemoryAccessorFRM mm(gblDomainMemory,0,0);
	SVUT_ASSERT_SAME(&gblBuffer[3][2],&mm.getCell(2,3));
	SVUT_ASSERT_SAME(&gblBuffer[2][3],&mm.getCell(3,2));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestMemoryModel,testIsContiguous)
{
	const CMRMemoryAccessorFRM mm(gblDomainMemory,0,0);
	SVUT_ASSERT_TRUE(mm.isContiguous(CMRRect(0,0,CST_WIDTH,1)));
	SVUT_ASSERT_TRUE(mm.isContiguous(CMRRect(0,0,CST_WIDTH,2)));
	SVUT_ASSERT_FALSE(mm.isContiguous(CMRRect(0,0,CST_WIDTH/2,2)));
	SVUT_ASSERT_FALSE(mm.isContiguous(CMRRect(0,0,1,CST_HEIGHT)));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestMemoryModel,testGetAbsPosition)
{
	const CMRMemoryAccessorFRM mm(gblDomainMemory,3,5);
	SVUT_ASSERT_EQUAL(CMRVect2D(3,5), mm.getAbsPosition());
	SVUT_ASSERT_EQUAL(CMRVect2D(4,7), mm.getAbsPosition(1,2));
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
