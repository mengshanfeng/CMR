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

/**********************  USING  *********************/
using namespace svUnitTest;

static const int CST_WIDTH = 20;
static const int CST_HEIGHT = 20;
static const CMRRect CST_MM_RECT(0,0,CST_WIDTH,CST_HEIGHT);

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestMemoryModelRowMajor,testGetCellId)
{
	SVUT_ASSERT_EQUAL(0,CMRMemoryModelRowMajor::getCellId(0,0,CST_WIDTH,CST_HEIGHT));
	SVUT_ASSERT_EQUAL(1,CMRMemoryModelRowMajor::getCellId(1,0,CST_WIDTH,CST_HEIGHT));
	SVUT_ASSERT_EQUAL(CST_WIDTH,CMRMemoryModelRowMajor::getCellId(0,1,CST_WIDTH,CST_HEIGHT));
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
SVUT_DECLARE_FLAT_TEST(TestMemoryModelRowMajor,testGetName)
{
	SVUT_ASSERT_EQUAL("RowMajor",CMRMemoryModelRowMajor::getName());
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
