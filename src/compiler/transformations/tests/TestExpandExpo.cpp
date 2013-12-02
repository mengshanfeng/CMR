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
#include <CMRTransformationExpandExpo.h>
#include <../definitions/CMRProjectCode.h>
#include <../definitions/ProjectIterator.h>
#include <../definitions/ProjectConstant.h>

/**********************  USING  *********************/
using namespace testing;
using namespace CMRCompiler;

/*********************  STRUCT  *********************/
struct TestExpandExpoParam
{
	const char * in;
	const char * out;
};

/*********************  CONSTS  *********************/
static const TestExpandExpoParam CST_TESTED_VALUES[] = { 
	{"a"        , "a"               },
	{"a^2"      , "(a*a)"           },
	{"(a+5)^2"  , "((a+5)*(a+5))"   },
	{"a^3"      , "(a*a*a)"         },
	{"a^5.5"    , "pow{a,5.5}"      },
	{"(a+4)^5.5", "pow{(a+4),5.5}"  },
	{"a^{1/2}"  , "sqrt{a}"         }
};

/*********************  CLASS  **********************/
class TestExpandExpoBase : public TestWithParam<TestExpandExpoParam>
{
};

/*******************  FUNCTION  *********************/
TEST_P(TestExpandExpoBase,simpleTest)
{
	CMRTransformationExpandExpo transf;
	ProjectContext context;
	CMRProjectCodeNode root(&context);

	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	
	const TestExpandExpoParam param = GetParam();
	
	CMRProjectCodeEquation & eq = root.addEquation("a",param.in);
	
	transf.run(root);
	
	EXPECT_EQ(param.out,eq.getFormulas().getString());
}

/********************  MACRO  ***********************/
INSTANTIATE_TEST_CASE_P(TestExpandExpo, TestExpandExpoBase, ValuesIn(CST_TESTED_VALUES));
