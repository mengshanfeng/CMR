/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <svUnitTest.h>
#include <domain/CMRAbstractType.h>

/**********************  USING  *********************/
using namespace svUnitTest;

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestAbstractType,testGetNameInt)
{
	CMRGenericType<int> type;
	SVUT_ASSERT_EQUAL("i",type.getName());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestAbstractType,testGetNameFloat)
{
	CMRGenericType<float> type;
	SVUT_ASSERT_EQUAL("f",type.getName());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestAbstractType,testGetSizeInt)
{
	CMRGenericType<int> type;
	SVUT_ASSERT_EQUAL(sizeof(int),type.getSize());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestAbstractType,testGetSizeInt8)
{
	CMRGenericType<int[8]> type;
	SVUT_ASSERT_EQUAL(sizeof(int[8]),type.getSize());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestAbstractType,testOperatorEqual)
{
	CMRGenericType<int[8]> type1;
	CMRGenericType<int[8]> type2;
	CMRGenericType<int[9]> type3;
	SVUT_ASSERT_TRUE(type1 == type2);
	SVUT_ASSERT_FALSE(type1 == type3);
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
