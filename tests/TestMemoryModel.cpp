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
typedef CMRMemoryModelRowMajor<float> CMRMemoryModelFloatRM;

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestMemoryModel,testTypeSize)
{
	CMRMemoryModelFloatRM mm;
	SVUT_ASSERT_EQUAL(sizeof(float),mm.getTypeSize());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestMemoryModel,testGetCellId)
{
	CMRMemoryModelFloatRM mm;
	SVUT_ASSERT_EQUAL(0,mm.getCellId(0,0,CST_WIDTH,CST_HEIGHT));
	SVUT_ASSERT_EQUAL(1,mm.getCellId(1,0,CST_WIDTH,CST_HEIGHT));
	SVUT_ASSERT_EQUAL(CST_WIDTH,mm.getCellId(0,1,CST_WIDTH,CST_HEIGHT));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestMemoryModel,testIsContiguous)
{
	CMRMemoryModelFloatRM mm;
	SVUT_ASSERT_TRUE(mm.isContiguous(CST_MM_RECT,CMRRect(0,0,CST_WIDTH,1)));
	SVUT_ASSERT_TRUE(mm.isContiguous(CST_MM_RECT,CMRRect(0,0,CST_WIDTH,2)));
	SVUT_ASSERT_FALSE(mm.isContiguous(CST_MM_RECT,CMRRect(0,0,CST_WIDTH/2,2)));
	SVUT_ASSERT_FALSE(mm.isContiguous(CST_MM_RECT,CMRRect(0,0,1,CST_HEIGHT)));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestMemoryModel,testGetCell)
{
	CMRMemoryModelFloatRM mm;
	SVUT_ASSERT_SAME(&gblBuffer[3][2],mm.getCell(gblBuffer,2,3,CST_WIDTH,CST_HEIGHT));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestMemoryModel,testGetTypedCell)
{
	CMRMemoryModelFloatRM mm;
	memset(gblBuffer,0,sizeof(gblBuffer));
	gblBuffer[4][3] = 3.1f;
	SVUT_ASSERT_EQUAL(3.1f,mm.getTypedCell(gblBuffer,3,4,CST_WIDTH,CST_HEIGHT));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestMemoryModel,testCopyFromBuffer)
{
	SVUT_ASSERT_TODO("todo");
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestMemoryModel,testCopyToBuffer)
{
	SVUT_ASSERT_TODO("todo");
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestMemoryModel,testAccessorGetCell)
{
	CMRMemoryModelFloatRM mm;
	CMRMemoryModelFloatRM::Accessor acc((float*)gblBuffer,CST_MM_RECT,CMRVect2D(1,2));
	SVUT_ASSERT_SAME(&gblBuffer[2][1], &acc.getCell(0,0));
	SVUT_ASSERT_SAME(&gblBuffer[3][2], &acc.getCell(1,1));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestMemoryModel,testAccessorGetAbsPosition)
{
	CMRMemoryModelFloatRM mm;
	CMRMemoryModelFloatRM::Accessor acc((float*)gblBuffer,CST_MM_RECT,CMRVect2D(1,2));
	SVUT_ASSERT_EQUAL(CMRVect2D(1,2), acc.getAbsPosition());
	SVUT_ASSERT_EQUAL(CMRVect2D(2,3), acc.getAbsPosition(1,1));
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
