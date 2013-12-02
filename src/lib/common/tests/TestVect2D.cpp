/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <CMRGeometry.h>

/**********************  USING  *********************/
using namespace testing;

/*******************  FUNCTION  *********************/
TEST(TestVect2D,testConstructor)
{
	const CMRVect2D vect(10,20);
	EXPECT_EQ(10,vect.x);
	EXPECT_EQ(20,vect.y);
}

/*******************  FUNCTION  *********************/
TEST(TestVect2D,testSet)
{
	CMRVect2D vect;
	vect.set(10,20);
	EXPECT_EQ(10,vect.x);
	EXPECT_EQ(20,vect.y);
}

/*******************  FUNCTION  *********************/
TEST(TestVect2D,testOperatorEqual)
{
	const CMRVect2D v1(10,20);
	const CMRVect2D v2(10,10);
	EXPECT_TRUE(v1 == v1);
	EXPECT_FALSE(v1 == v2);
}

/*******************  FUNCTION  *********************/
TEST(TestVect2D,testOperatorPlus)
{
	const CMRVect2D v1(10,20);
	const CMRVect2D v2(5,10);
	const CMRVect2D v3(15,30);
	CMRVect2D v4(v1);
	EXPECT_EQ(v3,v1+v2);
	EXPECT_EQ(v3,v2+v1);
	EXPECT_EQ(v3,v4+=v2);
	EXPECT_EQ(v3,v4);
}

/*******************  FUNCTION  *********************/
TEST(TestVect2D,testOperatorMinus)
{
	const CMRVect2D v1(10,20);
	const CMRVect2D v2(5,10);
	const CMRVect2D v3(5,10);
	const CMRVect2D v5(-5,-10);
	CMRVect2D v4(v1);
	EXPECT_EQ(v3,v1-v2);
	EXPECT_EQ(v5,v2-v1);
	EXPECT_EQ(v3,v4-=v2);
	EXPECT_EQ(v3,v4);
}
