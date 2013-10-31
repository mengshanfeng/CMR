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
#include <../definitions/CMRProjectIterator.h>
#include <../definitions/CMRProjectConstant.h>

/**********************  USING  *********************/
using namespace testing;

/*********************  CONSTS  *********************/

/*******************  FUNCTION  *********************/
TEST(TestExandFrac,testConstructor)
{
	CMRTransformationExpandFrac transf;
}

/*******************  FUNCTION  *********************/
TEST(TestReplace,testRun_single)
{
	CMRTransformationExpandFrac transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	CMRProjectCodeEquation & eq = root.addEquation("a","4 * b + \\frac{1}{b}");
	
	transf.run(root);
	
	EXPECT_EQ("4*b+(1/b)",eq.getFormulas().getString());
}

/*******************  FUNCTION  *********************/
TEST(TestReplace,testRun_complex_1)
{
	CMRTransformationExpandFrac transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	CMRProjectCodeEquation & eq = root.addEquation("a","4 * b + \\frac{a+b}{4*c}");
	
	transf.run(root);
	
	EXPECT_EQ("4*b+((a+b)/(4*c))",eq.getFormulas().getString());
}

/*******************  FUNCTION  *********************/
TEST(TestReplace,testRun_complex_2)
{
	CMRTransformationExpandFrac transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	CMRProjectCodeEquation & eq = root.addEquation("a","4 * b + \\frac{a+b}{\\frac{4}{c}}");
	
	transf.run(root);
	
	EXPECT_EQ("4*b+((a+b)/(4/c))",eq.getFormulas().getString());
}
