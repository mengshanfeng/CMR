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
SVUT_DECLARE_FLAT_TEST(TestVect2D,testConstructor)
{
	const CMRVect2D vect(10,20);
	SVUT_ASSERT_EQUAL(10,vect.x);
	SVUT_ASSERT_EQUAL(20,vect.y);
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestVect2D,testSet)
{
	CMRVect2D vect;
	vect.set(10,20);
	SVUT_ASSERT_EQUAL(10,vect.x);
	SVUT_ASSERT_EQUAL(20,vect.y);
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestVect2D,testOperatorEqual)
{
	const CMRVect2D v1(10,20);
	const CMRVect2D v2(10,10);
	SVUT_ASSERT_TRUE(v1 == v1);
	SVUT_ASSERT_FALSE(v1 == v2);
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestVect2D,testOperatorPlus)
{
	const CMRVect2D v1(10,20);
	const CMRVect2D v2(5,10);
	const CMRVect2D v3(15,30);
	CMRVect2D v4(v1);
	SVUT_ASSERT_EQUAL(v3,v1+v2);
	SVUT_ASSERT_EQUAL(v3,v2+v1);
	SVUT_ASSERT_EQUAL(v3,v4+=v2);
	SVUT_ASSERT_EQUAL(v3,v4);
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestVect2D,testOperatorMinus)
{
	const CMRVect2D v1(10,20);
	const CMRVect2D v2(5,10);
	const CMRVect2D v3(5,10);
	const CMRVect2D v5(-5,-10);
	CMRVect2D v4(v1);
	SVUT_ASSERT_EQUAL(v3,v1-v2);
	SVUT_ASSERT_EQUAL(v5,v2-v1);
	SVUT_ASSERT_EQUAL(v3,v4-=v2);
	SVUT_ASSERT_EQUAL(v3,v4);
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
