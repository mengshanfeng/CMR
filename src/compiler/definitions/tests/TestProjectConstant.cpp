/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <CMRProjectConstant.h>
#include <../parsor/CMRLatexFormula.h>
#include <sstream>
#include "MockProjectEntity.h"
#include <CMRProjectContext.h>

/**********************  USING  *********************/
using namespace testing;
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
TEST(TestProjectConstant,testConstructor)
{
	CMRProjectConstant constant("C","testC");
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testLoadValue_scalar)
{
	CMRProjectConstant constant("C","testC");
	constant.loadValues("55",0);

	stringstream out;
	constant.printValues(out);

	EXPECT_EQ("55",out.str());
	EXPECT_FALSE(constant.haveCapture("\\cmr{cstid}{i}"));
	EXPECT_FALSE(constant.haveCapture("\\cmr{cstid}{j}"));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testLoadValue_scalar_invalid)
{
	CMRProjectConstant constant("C","testC");
	EXPECT_THROW(constant.loadValues("55;5",0),CMRLatexException);
	EXPECT_THROW(constant.loadValues("55\\\\5",0),CMRLatexException);
	EXPECT_THROW(constant.loadValues("",0),CMRLatexException);
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testLoadValue_vector)
{
	CMRProjectConstant constant("C","testC");
	constant.loadValues("1;2;3;4;5;6",1);

	stringstream out;
	constant.printValues(out);

	EXPECT_EQ("1 2 3 4 5 6 ",out.str());
	EXPECT_TRUE(constant.haveCapture("\\cmr{cstid}{i}"));
	EXPECT_FALSE(constant.haveCapture("\\cmr{cstid}{j}"));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testLoadValue_vector_invalid)
{
	CMRProjectConstant constant("C","testC");
	EXPECT_THROW(constant.loadValues("55\\\\5",1),CMRLatexException);
	EXPECT_THROW(constant.loadValues("",1),CMRLatexException);
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testLoadValue_matrix)
{
	CMRProjectConstant constant("C","testC");
	constant.loadValues("1.1;1.2;1.3\\\\2.1;2.2;2.3\\\\3.1;3.2;3.3\\\\4.1;4.2;4.3\\\\5.1;5.2;5.3\\\\6.1;6.2;6.3",2);

	stringstream out;
	constant.printValues(out);

	EXPECT_EQ("1.1 1.2 1.3 \n2.1 2.2 2.3 \n3.1 3.2 3.3 \n4.1 4.2 4.3 \n5.1 5.2 5.3 \n6.1 6.2 6.3 \n",out.str());
	EXPECT_TRUE(constant.haveCapture("\\cmr{cstid}{i}"));
	EXPECT_TRUE(constant.haveCapture("\\cmr{cstid}{j}"));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testLoadValue_scalar_matrix_invalid)
{
	CMRProjectConstant constant("C","testC");
	EXPECT_THROW(constant.loadValues("",2),CMRLatexException);
	EXPECT_THROW(constant.loadValues("1.1;1.2;1.3\\\\2.1;2.2\\\\3.1;3.2;3.3",2),CMRLatexException);
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testPrintDebug)
{
	CMRProjectConstant constant("C","testC");
	constant.loadValues("1.1;1.2;1.3\\\\2.1;2.2;2.3",2);
	
	stringstream out;
	constant.printDebug(out);
	
	EXPECT_EQ(TEST_CST_1,out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testGenDefinitionCCode_scalar)
{
	CMRProjectConstant constant("C","testC");
	constant.loadValues("5.5",0);
	
	CMRProjectContext context;
	stringstream out;
	constant.genDefinitionCCode(out,context);
	
	EXPECT_EQ("const float testC = 5.5;\n",out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testGenDefinitionCCode_vector)
{
	CMRProjectConstant constant("C","testC");
	constant.loadValues("1;2;3",1);
	
	CMRProjectContext context;
	stringstream out;
	constant.genDefinitionCCode(out,context);
	
	EXPECT_EQ(TEST_CST_2,out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testGenDefinitionCCode_matrix)
{
	CMRProjectConstant constant("C","testC");
	constant.loadValues("1;2;3\\\\4;5;6",2);
	
	CMRProjectContext context;
	stringstream out;
	constant.genDefinitionCCode(out,context);
	
	EXPECT_EQ(TEST_CST_3,out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testGenUsageCCode_scalar)
{
	CMRProjectConstant constant("C","testC");
	constant.loadValues("1",0);
	
	CMRProjectContext context;
	CMRLatexEntity2 entity("C");
	stringstream out;
	constant.genUsageCCode(out,context,entity);
	
	EXPECT_EQ("testC",out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testGenUsageCCode_vector)
{
	CMRProjectConstant constant("C","testC");
	constant.loadValues("1;10",1);
	
	CMRProjectContext context;
	MockProjectEntity var("m","testM");
	context.addEntry(&var);
	CMRLatexEntity2 entity("C_{m +5}");
	stringstream out;
	constant.genUsageCCode(out,context,entity);
	
	EXPECT_EQ("testC[ testM + 5 ]",out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testGenUsageCCode_matrix)
{
	CMRProjectConstant constant("C","testC");
	constant.loadValues("1;10\\2;5",2);
	
	CMRProjectContext context;
	MockProjectEntity var("m","testM");
	context.addEntry(&var);
	CMRLatexEntity2 entity("C_{m +5,m*2}");
	stringstream out;
	constant.genUsageCCode(out,context,entity);
	
	EXPECT_EQ("testC[ testM * 2 ][ testM + 5 ]",out.str());
}
