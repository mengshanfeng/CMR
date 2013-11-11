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
#include <../definitions/CMRProjectCode.h>
#include <../definitions/CMRProjectIterator.h>
#include <../definitions/CMRProjectConstant.h>

/**********************  USING  *********************/
using namespace testing;
using namespace CMRCompiler;

/*********************  CONSTS  *********************/

/*******************  FUNCTION  *********************/
TEST(TestReplace,testConstructor)
{
	CMRProjectCaptureMap map;
	CMRTransformationReplace transf(&map);
}

/*******************  FUNCTION  *********************/
TEST(TestReplace,testRun_single)
{
	CMRProjectCaptureMap map;
	LatexFormulas f("c");
	map["b"] = &f;
	CMRTransformationReplace transf(&map);
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	CMRProjectCodeEquation & eq = root.addEquation("a","4 * b + \\frac{1+b}{b}");
	
	transf.run(root);
	
	EXPECT_EQ("4*c+\\frac{1+c}{c}",eq.getFormulas().getString());
}

/*******************  FUNCTION  *********************/
TEST(TestReplace,testRun_complex)
{
	CMRProjectCaptureMap map;
	LatexFormulas f("c*c");
	map["b"] = &f;
	CMRTransformationReplace transf(&map);
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	CMRProjectCodeEquation & eq = root.addEquation("a","4 * b + \\frac{1+b}{b}");
	
	transf.run(root);
	
	EXPECT_EQ("4*(c*c)+\\frac{1+(c*c)}{(c*c)}",eq.getFormulas().getString());
}

/*******************  FUNCTION  *********************/
TEST(TestReplace,testRun_recurse)
{
	CMRProjectCaptureMap map;
	LatexFormulas f1("c*c");
	LatexFormulas f2("d*a");
	map["a"] = &f1;
	map["b"] = &f2;
	CMRTransformationReplace transf(&map);
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	root.addLocalVariable("d","testD","int","0");
	CMRProjectCodeEquation & eq = root.addEquation("a","b");
	
	transf.run(root);
	
	EXPECT_EQ("(d*(c*c))",eq.getFormulas().getString());
}