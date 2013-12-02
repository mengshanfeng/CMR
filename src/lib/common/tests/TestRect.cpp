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
TEST(TestRect,testConstructor)
{
	const CMRRect rect(1,1,10,10);
	EXPECT_EQ(1,rect.x);
	EXPECT_EQ(1,rect.y);
	EXPECT_EQ(10,rect.width);
	EXPECT_EQ(10,rect.height);
}

/*******************  FUNCTION  *********************/
TEST(TestRect,testSet)
{
	CMRRect rect;
	rect.set(1,1,10,10);
	EXPECT_EQ(1,rect.x);
	EXPECT_EQ(1,rect.y);
	EXPECT_EQ(10,rect.width);
	EXPECT_EQ(10,rect.height);
}

/*******************  FUNCTION  *********************/
TEST(TestRect,testOperatorEqualTrue)
{
	const CMRRect rect1(1,1,10,10);
	const CMRRect rect2(1,1,10,10);
	EXPECT_TRUE(rect1 == rect2);
}

/*******************  FUNCTION  *********************/
TEST(TestRect,testOperatorEqualFalse)
{
	const CMRRect rect1(1,1,10,10);
	const CMRRect rect2(2,1,10,10);
	const CMRRect rect3(1,2,10,10);
	const CMRRect rect4(1,0,11,10);
	const CMRRect rect5(1,0,10,11);
	EXPECT_FALSE(rect1 == rect2);
	EXPECT_FALSE(rect1 == rect3);
	EXPECT_FALSE(rect1 == rect4);
	EXPECT_FALSE(rect1 == rect5);
}

/*******************  FUNCTION  *********************/
TEST(TestRect,testContainsFully)
{
	const CMRRect rect1(1,1,10,10);
	const CMRRect rect2(2,2,9,9);
	const CMRRect rect3(0,0,11,11);
	const CMRRect rect4(11,11,10,10);
	EXPECT_TRUE(rect1.contains(rect1));
	EXPECT_TRUE(rect1.contains(rect2));
	EXPECT_FALSE(rect1.contains(rect3));
	EXPECT_FALSE(rect1.contains(rect4));
}

/*******************  FUNCTION  *********************/
TEST(TestRect,testContainsPoint)
{
	const CMRRect rect(1,1,10,10);
	EXPECT_TRUE(rect.contains(CMRVect2D(1,1)));
	EXPECT_TRUE(rect.contains(CMRVect2D(2,2)));
	EXPECT_TRUE(rect.contains(CMRVect2D(10,10)));
	EXPECT_FALSE(rect.contains(CMRVect2D(11,11)));
	EXPECT_TRUE(rect.contains(CMRVect2D(9,9)));
}

/*******************  FUNCTION  *********************/
TEST(TestRect,testContainsPartially)
{
	const CMRRect rect1(1,1,10,10);
	const CMRRect rect2(2,2,9,9);
	const CMRRect rect3(0,0,11,11);
	const CMRRect rect4(11,11,10,10);
	EXPECT_TRUE(rect1.contains(rect1,true));
	EXPECT_TRUE(rect1.contains(rect2,true));
	EXPECT_TRUE(rect1.contains(rect3,true));
	EXPECT_FALSE(rect1.contains(rect4,true));
}

/*******************  FUNCTION  *********************/
TEST(TestRect,testPoint)
{
	const CMRRect rect(1,1,10,10);
	EXPECT_EQ(CMRVect2D(1,1),rect.point1());
	EXPECT_EQ(CMRVect2D(11,11),rect.point2());
}

/*******************  FUNCTION  *********************/
TEST(TestRect,testSurface)
{
	const CMRRect rect(1,1,10,10);
	EXPECT_EQ(100,rect.surface());
}

/*******************  FUNCTION  *********************/
TEST(TestRect,testOperatorPlus)
{
	const CMRRect rect1(1,1,10,10);
	const CMRRect rect2(11,21,10,10);
	const CMRVect2D v(10,20);
	EXPECT_EQ(rect2,rect1+v);
}
