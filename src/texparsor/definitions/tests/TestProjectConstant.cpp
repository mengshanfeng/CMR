/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <svUnitTest.h>
#include <CMRProjectConstant.h>
#include <../parsor/CMRLatexFormula.h>
#include <sstream>
#include "MockProjectEntity.h"
#include <CMRProjectContext.h>

/**********************  USING  *********************/
using namespace svUnitTest;
using namespace std;

/*********************  CONSTS  *********************/
static const char TEST_CST_1[] = "Entity :\n\
    - latexName  : C_{\\cmr{cstid}{i},\\cmr{cstid}{j}}\n\
    - shortName  : C\n\
    - longName   : testC\n\
    - exponent   : \n\
    - indices    : [\\cmr{cstid}{i}], [\\cmr{cstid}{j}]\n\
    - parameters : \n\
    - dims       : 2 : [ 3, 2 ]\n\
    - values     :1.1, 1.2, 1.3, 2.1, 2.2, 2.3\n";

/*********************  CONSTS  *********************/
static const char TEST_CST_2[] = "//Definition of constant C\n\
static const float TMP_VALUE_testC[3]={1,2,3,};\n\
const CMRMathVector testC(TMP_VALUE_testC,3);\n";

/*********************  CONSTS  *********************/
static const char TEST_CST_3[] = "//Definition of constant C\n\
static const float TMP_VALUE_testC[3][2]={{1,2,3,},{4,5,6,}};\n\
const CMRMathMatrix testC(TMP_VALUE_testC,3,2);\n";

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectConstant,testConstructor)
{
	CMRProjectConstant constant("C","testC");
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectConstant,testLoadValue_scalar)
{
	CMRProjectConstant constant("C","testC");
	constant.loadValues("55",0);

	stringstream out;
	constant.printValues(out);

	SVUT_ASSERT_EQUAL("55",out.str());
	SVUT_ASSERT_FALSE(constant.haveCapture("\\cmr{cstid}{i}"));
	SVUT_ASSERT_FALSE(constant.haveCapture("\\cmr{cstid}{j}"));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectConstant,testLoadValue_scalar_invalid)
{
	CMRProjectConstant constant("C","testC");
	SVUT_ASSERT_THROW(CMRLatexException,constant.loadValues("55;5",0));
	SVUT_ASSERT_THROW(CMRLatexException,constant.loadValues("55\\\\5",0));
	SVUT_ASSERT_THROW(CMRLatexException,constant.loadValues("",0));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectConstant,testLoadValue_vector)
{
	CMRProjectConstant constant("C","testC");
	constant.loadValues("1;2;3;4;5;6",1);

	stringstream out;
	constant.printValues(out);

	SVUT_ASSERT_EQUAL("1 2 3 4 5 6 ",out.str());
	SVUT_ASSERT_TRUE(constant.haveCapture("\\cmr{cstid}{i}"));
	SVUT_ASSERT_FALSE(constant.haveCapture("\\cmr{cstid}{j}"));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectConstant,testLoadValue_vector_invalid)
{
	CMRProjectConstant constant("C","testC");
	SVUT_ASSERT_THROW(CMRLatexException,constant.loadValues("55\\\\5",1));
	SVUT_ASSERT_THROW(CMRLatexException,constant.loadValues("",1));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectConstant,testLoadValue_matrix)
{
	CMRProjectConstant constant("C","testC");
	constant.loadValues("1.1;1.2;1.3\\\\2.1;2.2;2.3\\\\3.1;3.2;3.3\\\\4.1;4.2;4.3\\\\5.1;5.2;5.3\\\\6.1;6.2;6.3",2);

	stringstream out;
	constant.printValues(out);

	SVUT_ASSERT_EQUAL("1.1 1.2 1.3 \n2.1 2.2 2.3 \n3.1 3.2 3.3 \n4.1 4.2 4.3 \n5.1 5.2 5.3 \n6.1 6.2 6.3 \n",out.str());
	SVUT_ASSERT_TRUE(constant.haveCapture("\\cmr{cstid}{i}"));
	SVUT_ASSERT_TRUE(constant.haveCapture("\\cmr{cstid}{j}"));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectConstant,testLoadValue_scalar_matrix_invalid)
{
	CMRProjectConstant constant("C","testC");
	SVUT_ASSERT_THROW(CMRLatexException,constant.loadValues("",2));
	SVUT_ASSERT_THROW(CMRLatexException,constant.loadValues("1.1;1.2;1.3\\\\2.1;2.2\\\\3.1;3.2;3.3",2));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectConstant,testPrintDebug)
{
	CMRProjectConstant constant("C","testC");
	constant.loadValues("1.1;1.2;1.3\\\\2.1;2.2;2.3",2);
	
	stringstream out;
	constant.printDebug(out);
	
	SVUT_ASSERT_EQUAL(TEST_CST_1,out.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectConstant,testGenDefinitionCCode_scalar)
{
	CMRProjectConstant constant("C","testC");
	constant.loadValues("5.5",0);
	
	CMRProjectContext context;
	stringstream out;
	constant.genDefinitionCCode(out,context);
	
	SVUT_ASSERT_EQUAL("const float testC = 5.5;\n",out.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectConstant,testGenDefinitionCCode_vector)
{
	CMRProjectConstant constant("C","testC");
	constant.loadValues("1;2;3",1);
	
	CMRProjectContext context;
	stringstream out;
	constant.genDefinitionCCode(out,context);
	
	SVUT_ASSERT_EQUAL(TEST_CST_2,out.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectConstant,testGenDefinitionCCode_matrix)
{
	CMRProjectConstant constant("C","testC");
	constant.loadValues("1;2;3\\\\4;5;6",2);
	
	CMRProjectContext context;
	stringstream out;
	constant.genDefinitionCCode(out,context);
	
	SVUT_ASSERT_EQUAL(TEST_CST_3,out.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectConstant,testGenUsageCCode_scalar)
{
	CMRProjectConstant constant("C","testC");
	constant.loadValues("1",0);
	
	CMRProjectContext context;
	CMRLatexEntity2 entity("C");
	stringstream out;
	constant.genUsageCCode(out,context,entity);
	
	SVUT_ASSERT_EQUAL("testC",out.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectConstant,testGenUsageCCode_vector)
{
	CMRProjectConstant constant("C","testC");
	constant.loadValues("1;10",1);
	
	CMRProjectContext context;
	MockProjectEntity var("m","testM");
	context.addEntry(&var);
	CMRLatexEntity2 entity("C_{m +5}");
	stringstream out;
	constant.genUsageCCode(out,context,entity);
	
	SVUT_ASSERT_EQUAL("testC[ testM + 5 ]",out.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectConstant,testGenUsageCCode_matrix)
{
	CMRProjectConstant constant("C","testC");
	constant.loadValues("1;10\\2;5",2);
	
	CMRProjectContext context;
	MockProjectEntity var("m","testM");
	context.addEntry(&var);
	CMRLatexEntity2 entity("C_{m +5,m*2}");
	stringstream out;
	constant.genUsageCCode(out,context,entity);
	
	SVUT_ASSERT_EQUAL("testC[ testM * 2 ][ testM + 5 ]",out.str());
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
