/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <CMRAbstractType.h>

/**********************  USING  *********************/
using namespace testing;

/*******************  FUNCTION  *********************/
TEST(TestAbstractType,testGetNameInt)
{
	CMRGenericType<int> type;
	EXPECT_EQ("i",type.getName());
}

/*******************  FUNCTION  *********************/
TEST(TestAbstractType,testGetNameFloat)
{
	CMRGenericType<float> type;
	EXPECT_EQ("f",type.getName());
}

/*******************  FUNCTION  *********************/
TEST(TestAbstractType,testGetSizeInt)
{
	CMRGenericType<int> type;
	EXPECT_EQ(sizeof(int),type.getSize());
}

/*******************  FUNCTION  *********************/
TEST(TestAbstractType,testGetSizeInt8)
{
	CMRGenericType<int[8]> type;
	EXPECT_EQ(sizeof(int[8]),type.getSize());
}

/*******************  FUNCTION  *********************/
TEST(TestAbstractType,testOperatorEqual)
{
	CMRGenericType<int[8]> type1;
	CMRGenericType<int[8]> type2;
	CMRGenericType<int[9]> type3;
	EXPECT_TRUE(type1 == type2);
	EXPECT_FALSE(type1 == type3);
}
