/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <mpi.h>
#include <cstring>
#include <gtest/gtest.h>
#include <CMRGenericMemoryAccessor.h>
#include <CMRMemoryModels.h>

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
typedef CMRGenericMemoryAccessor<float,CMRMemoryModelRowMajor> CMRMemoryAccessorFRM;

/*******************  FUNCTION  *********************/
TEST(TestGenericMemoryAccessor,testConstructor)
{
	const CMRMemoryAccessorFRM mm;
	EXPECT_EQ(CMRVect2D(0,0),mm.getAbsPosition());
}

/*******************  FUNCTION  *********************/
TEST(TestGenericMemoryAccessor,testGetTypeSize)
{
	const CMRMemoryAccessorFRM mm;
	EXPECT_EQ(sizeof(float),mm.getTypeSize());
}

/*******************  FUNCTION  *********************/
TEST(TestGenericMemoryAccessor,testIsContiguous)
{
	const CMRMemoryAccessorFRM mm(gblDomainMemory);
	EXPECT_TRUE(mm.isContiguous(CMRRect(0,0,CST_WIDTH,1)));
	EXPECT_TRUE(mm.isContiguous(CMRRect(0,0,CST_WIDTH,2)));
	EXPECT_FALSE(mm.isContiguous(CMRRect(0,0,CST_WIDTH/2,2)));
	EXPECT_FALSE(mm.isContiguous(CMRRect(0,0,1,CST_HEIGHT)));
}

/*******************  FUNCTION  *********************/
TEST(TestGenericMemoryAccessor,testCopyFromBuffer1)
{
	CMRMemoryAccessorFRM mm(gblDomainMemory);

	//reset
	memset(gblBuffer,0,sizeof(gblBuffer));
	
	//copy
	float buffer[20];
	
	//setup buffer
	for (int i = 0 ; i < 10 ; i++)
	{
		buffer[i] = i + 10 + (10 * CST_WIDTH);
		buffer[i+10] = i + 10 + (11 * CST_WIDTH);
	}
	
	//push
	size_t res = mm.copyFromBuffer(buffer,sizeof(buffer),CMRRect(10,10,10,2));
	EXPECT_EQ(sizeof(buffer),res);
	
	//check
	for (int i = 0 ; i < 10 ; i++)
	{
		RecordProperty("iteration",i);
		EXPECT_EQ(i + 10 + (10 * CST_WIDTH),gblBuffer[10][10+i]);
		EXPECT_EQ(i + 10 + (11 * CST_WIDTH),gblBuffer[11][10+i]);
	}
}

/*******************  FUNCTION  *********************/
TEST(TestGenericMemoryAccessor,testCopyFromBuffer2)
{
	CMRDomainMemory domain(gblBuffer,CMRRect(-2,-2,CST_WIDTH,CST_HEIGHT));
	CMRMemoryAccessorFRM mm(domain);

	//reset
	memset(gblBuffer,0,sizeof(gblBuffer));
	
	//copy
	float buffer[20];
	
	//setup buffer
	for (int i = 0 ; i < 10 ; i++)
	{
		buffer[i] = i + 10 + (10 * CST_WIDTH);
		buffer[i+10] = i + 10 + (11 * CST_WIDTH);
	}
	
	//push
	size_t res = mm.copyFromBuffer(buffer,sizeof(buffer),CMRRect(8,8,10,2));
	EXPECT_EQ(sizeof(buffer),res);
	
	//check
	for (int i = 0 ; i < 10 ; i++)
	{
		RecordProperty("iteration",i);
		EXPECT_EQ(i + 10 + (10 * CST_WIDTH),gblBuffer[10][10+i]);
		EXPECT_EQ(i + 10 + (11 * CST_WIDTH),gblBuffer[11][10+i]);
	}
}

/*******************  FUNCTION  *********************/
TEST(TestGenericMemoryAccessor,testCopyToBuffer)
{
	const CMRMemoryAccessorFRM mm(gblDomainMemory);

	//init buffer
	for (int x = 0 ; x < CST_WIDTH ; x++)
		for (int y = 0 ; y < CST_WIDTH ; y++)
			gblBuffer[y][x] = y * CST_WIDTH + x;
	
	//copy
	float buffer[20];
	memset(buffer,0,sizeof(buffer));
	size_t res = mm.copyToBuffer(buffer,sizeof(buffer),CMRRect(10,10,10,2));
	EXPECT_EQ(sizeof(buffer),res);
	
	//check
	for (int i = 0 ; i < 10 ; i++)
	{
		RecordProperty("iteration",i);
		EXPECT_EQ(i + 10 + (10 * CST_WIDTH),buffer[i]);
		EXPECT_EQ(i + 10 + (11 * CST_WIDTH),buffer[i+10]);
	}
}

/*******************  FUNCTION  *********************/
TEST(TestGenericMemoryAccessor,testGetBufferSize)
{
	const CMRMemoryAccessorFRM mm(gblDomainMemory);
	EXPECT_EQ(10*10*sizeof(float),mm.getBufferSize(CMRRect(10,10,10,10)));
}

/*******************  FUNCTION  *********************/
TEST(TestGenericMemoryAccessor,testGetCell)
{
	CMRMemoryAccessorFRM mm(gblDomainMemory);
	EXPECT_EQ(&gblBuffer[3][2],mm.getCell(2,3));
	EXPECT_EQ(&gblBuffer[2][3],mm.getCell(3,2));
	
	//try write
	*(float*)mm.getCell(2,3) = 2;
	EXPECT_EQ(2,gblBuffer[3][2]);
	*(float*)mm.getCell(2,3) = 4;
	EXPECT_EQ(4,gblBuffer[3][2]);
}

/*******************  FUNCTION  *********************/
TEST(TestGenericMemoryAccessor,testGetCellConst)
{
	const CMRMemoryAccessorFRM mm(gblDomainMemory);
	EXPECT_EQ(&gblBuffer[3][2],mm.getCell(2,3));
	EXPECT_EQ(&gblBuffer[2][3],mm.getCell(3,2));
}

/*******************  FUNCTION  *********************/
TEST(TestGenericMemoryAccessor,testSet)
{
	CMRMemoryAccessorFRM mm;
	mm.set(gblDomainMemory);
	EXPECT_EQ(&gblBuffer[3][2],mm.getCell(2,3));
	EXPECT_EQ(&gblBuffer[2][3],mm.getCell(3,2));
}
