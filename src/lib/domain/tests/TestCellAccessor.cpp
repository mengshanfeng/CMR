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
#include <CMRCellAccessor.h>

/**********************  USING  *********************/
using namespace testing;

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
TEST(TestCellAccessor,testTypeSize)
{
	CMRCellAccessorFRM mm(gblDomainMemory,0,0);
	EXPECT_EQ(sizeof(float),mm.getTypeSize());
}

/*******************  FUNCTION  *********************/
TEST(TestCellAccessor,testConstructor1)
{
	const CMRCellAccessorFRM mm(gblDomainMemory,0,0);
	EXPECT_EQ((const float*)gblBuffer,mm(0,0));
}

/*******************  FUNCTION  *********************/
TEST(TestCellAccessor,testConstructor2)
{
	const CMRCellAccessorFRM mm(gblDomainMemory,1,0);
	EXPECT_EQ((const float*)gblBuffer,mm(-1,0));
}

/*******************  FUNCTION  *********************/
TEST(TestCellAccessor,testConstructor3)
{
	const CMRCellAccessorFRM mm(gblDomainMemory,0,1);
	EXPECT_EQ((const float*)gblBuffer,mm(0,-1));
}

/*******************  FUNCTION  *********************/
TEST(TestCellAccessor,testConstructor4)
{
	CMRCellAccessorFRM mm1(gblDomainMemory,1,2);
	const CMRCellAccessorFRM mm2(mm1,1,2);
	EXPECT_EQ((const float*)gblBuffer,mm2(-2,-4));
}

/*******************  FUNCTION  *********************/
TEST(TestCellAccessor,testGetCell)
{
	CMRCellAccessorFRM mm(gblDomainMemory,0,0);
	EXPECT_EQ(&gblBuffer[3][2],mm(2,3));
	EXPECT_EQ(&gblBuffer[2][3],mm(3,2));
	
	//try write
	*mm(2,3) = 2;
	EXPECT_EQ(2,gblBuffer[3][2]);
	*mm(2,3) = 4;
	EXPECT_EQ(4,gblBuffer[3][2]);
}

/*******************  FUNCTION  *********************/
TEST(TestCellAccessor,testGetCellConst1)
{
	const CMRCellAccessorFRM mm(gblDomainMemory,0,0);
	EXPECT_EQ(&gblBuffer[3][2],mm(2,3));
	EXPECT_EQ(&gblBuffer[2][3],mm(3,2));
}

/*******************  FUNCTION  *********************/
TEST(TestCellAccessor,testGetCellConst2)
{
	CMRDomainMemory domain(gblBuffer,CMRRect(-2,-2,CST_WIDTH,CST_HEIGHT));
	const CMRCellAccessorFRM mm(domain,0,0,true);
	EXPECT_EQ(&gblBuffer[3][2],mm(0,1));
	EXPECT_EQ(&gblBuffer[2][3],mm(1,0));
}

/*******************  FUNCTION  *********************/
TEST(TestCellAccessor,testGetCellNotOrig1)
{
	const CMRCellAccessorFRM mm(gblDomainMemory,2,3);
	EXPECT_EQ(&gblBuffer[6][4],mm(2,3));
	EXPECT_EQ(&gblBuffer[5][5],mm(3,2));
}

/*******************  FUNCTION  *********************/
TEST(TestCellAccessor,testGetCellNotOrig2)
{
	static CMRDomainMemory domainMemory(gblBuffer,CMRRect(10,10,CST_WIDTH,CST_HEIGHT));
	CMRCellAccessorFRM mm(domainMemory,0,0);
	EXPECT_EQ(&gblBuffer[3][2],mm(2,3));
	EXPECT_EQ(&gblBuffer[2][3],mm(3,2));
}

/*******************  FUNCTION  *********************/
TEST(TestCellAccessor,testGetCellNotOrig3)
{
	static CMRDomainMemory domainMemory(gblBuffer,CMRRect(10,10,CST_WIDTH,CST_HEIGHT));
	CMRCellAccessorFRM mm(domainMemory,2,3);
	EXPECT_EQ(&gblBuffer[6][4],mm(2,3));
	EXPECT_EQ(&gblBuffer[5][5],mm(3,2));
}


/*******************  FUNCTION  *********************/
TEST(TestCellAccessor,testIsContiguous)
{
	const CMRCellAccessorFRM mm(gblDomainMemory,0,0);
	EXPECT_TRUE(mm.isContiguous(CMRRect(0,0,CST_WIDTH,1)));
	EXPECT_TRUE(mm.isContiguous(CMRRect(0,0,CST_WIDTH,2)));
	EXPECT_FALSE(mm.isContiguous(CMRRect(0,0,CST_WIDTH/2,2)));
	EXPECT_FALSE(mm.isContiguous(CMRRect(0,0,1,CST_HEIGHT)));
}

/*******************  FUNCTION  *********************/
TEST(TestCellAccessor,testGetAbsPosition)
{
	const CMRCellAccessorFRM mm(gblDomainMemory,3,5);
	EXPECT_EQ(CMRVect2D(3,5), mm.getAbsPosition());
	EXPECT_EQ(CMRVect2D(4,7), mm.getAbsPosition(1,2));
}
