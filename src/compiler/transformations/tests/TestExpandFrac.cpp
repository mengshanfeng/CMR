/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include "../CMRTransformationMarkNoTransf.h"
#include <CMRTransformationReplace.h>
#include <CMRTransformationExpandFrac.h>
#include <../definitions/CMRProjectCode.h>
#include <../definitions/ProjectIterator.h>
#include <../definitions/ProjectConstant.h>

/**********************  USING  *********************/
using namespace testing;
using namespace CMRCompiler;

/*********************  STRUCT  *********************/
struct TestExandFracParam
{
	const char * in;
	const char * out;
};

/*********************  CONSTS  *********************/
static const TestExandFracParam CST_TESTED_VALUES[] = { 
	{"4 * b + \\frac{1}{b}"              , "4*b+(1/b)"           },
	{"4 * b + \\frac{a+b}{4*c}"          , "4*b+((a+b)/(4*c))"   },
	{"4 * b + \\frac{a+b}{\\frac{4}{c}}" , "4*b+((a+b)/(4/c))"   },
	{"4 * b + \\frac{2^a+b}{4*c}"        , "4*b+((2^a+b)/(4*c))" }
};

/*********************  CLASS  **********************/
class TestExandFracBase : public TestWithParam<TestExandFracParam>
{
};

/*******************  FUNCTION  *********************/
TEST_P(TestExandFracBase,simpleTest)
{
	CMRTransformationExpandFrac transf;
	ProjectContext context;
	CMRProjectCodeNode root(&context);

	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	
	const TestExandFracParam param = GetParam();
	
	CMRProjectCodeEquation & eq = root.addEquation("a",param.in);
	
	transf.run(root);
	
	EXPECT_EQ(param.out,eq.getFormulas().getString());
}

/********************  MACRO  ***********************/
INSTANTIATE_TEST_CASE_P(TestExandFrac, TestExandFracBase, ValuesIn(CST_TESTED_VALUES));
