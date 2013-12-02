/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <CMRMemoryModels.h>

/**********************  USING  *********************/
using namespace testing;

static const int CST_WIDTH = 20;
static const int CST_HEIGHT = 20;
static const CMRRect CST_MM_RECT(0,0,CST_WIDTH,CST_HEIGHT);

/*******************  FUNCTION  *********************/
TEST(TestMemoryModelRowMajor,testGetCellId)
{
	EXPECT_EQ(0,CMRMemoryModelRowMajor::getCellId(0,0,CST_WIDTH,CST_HEIGHT));
	EXPECT_EQ(1,CMRMemoryModelRowMajor::getCellId(1,0,CST_WIDTH,CST_HEIGHT));
	EXPECT_EQ(CST_WIDTH,CMRMemoryModelRowMajor::getCellId(0,1,CST_WIDTH,CST_HEIGHT));
}

/*******************  FUNCTION  *********************/
TEST(TestMemoryModelRowMajor,testIsContiguous)
{
	EXPECT_TRUE(CMRMemoryModelRowMajor::isContiguous(CST_MM_RECT,CMRRect(0,0,CST_WIDTH,1)));
	EXPECT_TRUE(CMRMemoryModelRowMajor::isContiguous(CST_MM_RECT,CMRRect(0,0,CST_WIDTH,2)));
	EXPECT_FALSE(CMRMemoryModelRowMajor::isContiguous(CST_MM_RECT,CMRRect(0,0,CST_WIDTH/2,2)));
	EXPECT_FALSE(CMRMemoryModelRowMajor::isContiguous(CST_MM_RECT,CMRRect(0,0,1,CST_HEIGHT)));
}

/*******************  FUNCTION  *********************/
TEST(TestMemoryModelRowMajor,testGetName)
{
	EXPECT_EQ("RowMajor",CMRMemoryModelRowMajor::getName());
}
