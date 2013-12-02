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
#include <CMRTransformationImplicitMul.h>
#include <../definitions/CMRProjectCode.h>
#include <../definitions/ProjectIterator.h>
#include <../definitions/ProjectConstant.h>

/**********************  USING  *********************/
using namespace testing;
using namespace CMRCompiler;

/*********************  STRUCT  *********************/
struct TestImplicitMulParam
{
	const char * in;
	const char * out;
};

/*********************  CONSTS  *********************/
static const TestImplicitMulParam CST_TESTED_VALUES[] = { 
	{ "a"                           , "a"                                  },
	{ "ab"                          , "a*b"                                },
	{ "ab^{ab}+8a*5e\\frac{af}{5e}" , "a*b^{a*b}+8*a*5*e*\\frac{a*f}{5*e}" },
	{ "-abc+5"                      , "-a*b*c+5"                           },
};

/*********************  CLASS  **********************/
class TestImplicitMulBase : public TestWithParam<TestImplicitMulParam>
{
};

/*******************  FUNCTION  *********************/
TEST_P(TestImplicitMulBase,simpleTest)
{
	CMRTransformationImplicitMul transf;
	ProjectContext context;
	CMRProjectCodeNode root(&context);

	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	
	const TestImplicitMulParam param = GetParam();
	
	CMRProjectCodeEquation & eq = root.addEquation("a",param.in);
	
	transf.run(root);
	
	EXPECT_EQ(param.out,eq.getFormulas().getString());
}

/********************  MACRO  ***********************/
INSTANTIATE_TEST_CASE_P(TestImplicitMul, TestImplicitMulBase, ValuesIn(CST_TESTED_VALUES));
