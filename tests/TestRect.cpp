/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <svUnitTest.h>
#include <common/CMRGeometry.h>

/**********************  USING  *********************/
using namespace svUnitTest;

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestRect,testConstructor)
{
	const CMRRect rect(1,1,10,10);
	SVUT_ASSERT_EQUAL(1,rect.x);
	SVUT_ASSERT_EQUAL(1,rect.y);
	SVUT_ASSERT_EQUAL(10,rect.width);
	SVUT_ASSERT_EQUAL(10,rect.height);
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestRect,testSet)
{
	CMRRect rect;
	rect.set(1,1,10,10);
	SVUT_ASSERT_EQUAL(1,rect.x);
	SVUT_ASSERT_EQUAL(1,rect.y);
	SVUT_ASSERT_EQUAL(10,rect.width);
	SVUT_ASSERT_EQUAL(10,rect.height);
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestRect,testOperatorEqualTrue)
{
	const CMRRect rect1(1,1,10,10);
	const CMRRect rect2(1,1,10,10);
	SVUT_ASSERT_TRUE(rect1 == rect2);
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestRect,testOperatorEqualFalse)
{
	const CMRRect rect1(1,1,10,10);
	const CMRRect rect2(2,1,10,10);
	const CMRRect rect3(1,2,10,10);
	const CMRRect rect4(1,0,11,10);
	const CMRRect rect5(1,0,10,11);
	SVUT_ASSERT_FALSE(rect1 == rect2);
	SVUT_ASSERT_FALSE(rect1 == rect3);
	SVUT_ASSERT_FALSE(rect1 == rect4);
	SVUT_ASSERT_FALSE(rect1 == rect5);
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestRect,testContainsFully)
{
	const CMRRect rect1(1,1,10,10);
	const CMRRect rect2(2,2,9,9);
	const CMRRect rect3(0,0,11,11);
	const CMRRect rect4(11,11,10,10);
	SVUT_ASSERT_TRUE(rect1.contains(rect1));
	SVUT_ASSERT_TRUE(rect1.contains(rect2));
	SVUT_ASSERT_FALSE(rect1.contains(rect3));
	SVUT_ASSERT_FALSE(rect1.contains(rect4));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestRect,testContainsPoint)
{
	const CMRRect rect(1,1,10,10);
	SVUT_ASSERT_TRUE(rect.contains(CMRVect2D(1,1)));
	SVUT_ASSERT_TRUE(rect.contains(CMRVect2D(2,2)));
	SVUT_ASSERT_TRUE(rect.contains(CMRVect2D(10,10)));
	SVUT_ASSERT_FALSE(rect.contains(CMRVect2D(11,11)));
	SVUT_ASSERT_TRUE(rect.contains(CMRVect2D(9,9)));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestRect,testContainsPartially)
{
	const CMRRect rect1(1,1,10,10);
	const CMRRect rect2(2,2,9,9);
	const CMRRect rect3(0,0,11,11);
	const CMRRect rect4(11,11,10,10);
	SVUT_ASSERT_TRUE(rect1.contains(rect1,true));
	SVUT_ASSERT_TRUE(rect1.contains(rect2,true));
	SVUT_ASSERT_TRUE(rect1.contains(rect3,true));
	SVUT_ASSERT_FALSE(rect1.contains(rect4,true));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestRect,testGetPoint)
{
	const CMRRect rect(1,1,10,10);
	SVUT_ASSERT_EQUAL(CMRVect2D(1,1),rect.getPoint1());
	SVUT_ASSERT_EQUAL(CMRVect2D(11,11),rect.getPoint2());
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
