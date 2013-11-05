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
#include <CMRTransformationReplaceAlias.h>
#include <../definitions/CMRProjectCode.h>
#include <../definitions/CMRProjectIterator.h>
#include <../definitions/CMRProjectConstant.h>
#include <../definitions/CMRProjectAlias.h>

/**********************  USING  *********************/
using namespace testing;

/*********************  CONSTS  *********************/

/*******************  FUNCTION  *********************/
TEST(TestReplaceAlias,testConstructor)
{
	CMRTransformationReplaceAlias transf;
}

/*******************  FUNCTION  *********************/
TEST(TestReplaceAlias,testRun_simple_1)
{
	CMRTransformationReplaceAlias transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	context.addEntry(new CMRProjectAlias("L","a"));
	CMRProjectCodeEquation & eq = root.addEquation("a","4 * b + \\frac{1}{b} + L");
	
	transf.run(root);
	
	EXPECT_EQ("4*b+\\frac{1}{b}+a",eq.getFormulas().getString());
}

/*******************  FUNCTION  *********************/
TEST(TestReplaceAlias,testRun_simple_2)
{
	CMRTransformationReplaceAlias transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	context.addEntry(new CMRProjectAlias("L^2","a",false));
	CMRProjectCodeEquation & eq = root.addEquation("a","4 * b + \\frac{1}{b} + L^2");
	
	transf.run(root);
	
	EXPECT_EQ("4*b+\\frac{1}{b}+a",eq.getFormulas().getString());
}

/*******************  FUNCTION  *********************/
TEST(TestReplaceAlias,testRun_composed_1)
{
	CMRTransformationReplaceAlias transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	context.addEntry(new CMRProjectAlias("L","abc+4"));
	CMRProjectCodeEquation & eq = root.addEquation("a","4 * b + \\frac{1}{b} + L");
	
	transf.run(root);
	
	EXPECT_EQ("4*b+\\frac{1}{b}+(abc+4)",eq.getFormulas().getString());
}

/*******************  FUNCTION  *********************/
TEST(TestReplaceAlias,testRun_composed_2)
{
	CMRTransformationReplaceAlias transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	context.addEntry(new CMRProjectAlias("L_i","abc+4",false));
	CMRProjectCodeEquation & eq = root.addEquation("a","4 * b + \\frac{1}{b} + L_i");
	
	transf.run(root);
	
	EXPECT_EQ("4*b+\\frac{1}{b}+(abc+4)",eq.getFormulas().getString());
}

/*******************  FUNCTION  *********************/
TEST(TestReplaceAlias,testRun_composed_capture_1)
{
	CMRTransformationReplaceAlias transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	context.addEntry(new CMRProjectAlias("L_i","ibc+4"));
	CMRProjectCodeEquation & eq = root.addEquation("a","4 * b + \\frac{1}{b} + L_a");
	
	transf.run(root);
	
	EXPECT_EQ("4*b+\\frac{1}{b}+(abc+4)",eq.getFormulas().getString());
}

/*******************  FUNCTION  *********************/
TEST(TestReplaceAlias,testRun_composed_capture_2)
{
	CMRTransformationReplaceAlias transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	context.addEntry(new CMRProjectAlias("L_i","i+i+i"));
	CMRProjectCodeEquation & eq = root.addEquation("a","4 * b + \\frac{1}{b} + L_a");
	
	transf.run(root);
	
	EXPECT_EQ("4*b+\\frac{1}{b}+(a+a+a)",eq.getFormulas().getString());
}

/*******************  FUNCTION  *********************/
TEST(TestReplaceAlias,testRun_composed_capture_3)
{
	CMRTransformationReplaceAlias transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	context.addEntry(new CMRProjectAlias("L_{i,j}","i+j+i"));
	CMRProjectCodeEquation & eq = root.addEquation("a","4 * b + \\frac{1}{b} + L_{a,b}");
	
	transf.run(root);
	
	EXPECT_EQ("4*b+\\frac{1}{b}+(a+b+a)",eq.getFormulas().getString());
}

/*******************  FUNCTION  *********************/
TEST(TestReplaceAlias,testRun_frac)
{
	CMRTransformationReplaceAlias transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	context.addEntry(new CMRProjectAlias("\\frac{a}{b}","a/b"));
	CMRProjectCodeEquation & eq = root.addEquation("a","4 * b + \\frac{1}{b}");
	
	transf.run(root);
	
	EXPECT_EQ("4*b+(1/b)",eq.getFormulas().getString());
}

/*******************  FUNCTION  *********************/
TEST(TestReplaceAlias,testRun_partial_capture)
{
	CMRTransformationReplaceAlias transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	CMRProjectLocalVariable & var = root.addLocalVariable("d","testd","int","0");
	var.addIndice("i");
	var.addIndice("j");
	var.addIndice("k");
	context.addEntry(new CMRProjectAlias("L_k","d_{i,j,k}"));
	CMRProjectCodeEquation & eq = root.addEquation("a","4 * b + \\frac{1}{b} + L_b");
	
	transf.run(root);
	
	EXPECT_EQ("4*b+\\frac{1}{b}+d_{i,j,b}",eq.getFormulas().getString());
}

/*******************  FUNCTION  *********************/
TEST(TestReplaceAlias,testRun_wildcard_alias_expo_1)
{
	CMRTransformationReplaceAlias transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	context.addEntry(new CMRProjectAlias("x^4","x*x*x*x",false)).captureName();
	CMRProjectCodeEquation & eq = root.addEquation("a","a^4");
	
	transf.run(root);
	
	EXPECT_EQ("(a*a*a*a)",eq.getFormulas().getString());
}

/*******************  FUNCTION  *********************/
TEST(TestReplaceAlias,testRun_wildcard_alias_expo_2)
{
	CMRTransformationReplaceAlias transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	context.addEntry(new CMRProjectAlias("x^4","x*x*x*x",false)).captureName();
	CMRProjectCodeEquation & eq = root.addEquation("a","(a+b)^4");
	
	transf.run(root);
	
	EXPECT_EQ("((a+b)*(a+b)*(a+b)*(a+b))",eq.getFormulas().getString());
}

/*******************  FUNCTION  *********************/
TEST(TestReplaceAlias,testRun_wildcard_alias_expo_3)
{
	CMRTransformationReplaceAlias transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	context.addEntry(new CMRProjectAlias("x^4","x*x*x*x",false)).captureName();
	CMRProjectCodeEquation & eq = root.addEquation("a","(\\frac{a}{2})^4");
	
	transf.run(root);
	
	EXPECT_EQ("(\\frac{a}{2}*\\frac{a}{2}*\\frac{a}{2}*\\frac{a}{2})",eq.getFormulas().getString());
}

// /*******************  FUNCTION  *********************/
// TEST(TestReplaceAlias,testRun_wildcard_alias_convar_contravar)
// {
// 	CMRTransformationReplaceAlias transf;
// 	
// 	CMRProjectContext context;
// 	CMRProjectCodeNode root(&context);
// 	root.addLocalVariable("a","testA","int","0");
// 	root.addLocalVariable("b","testB","int","0");
// 	root.addLocalVariable("c","testC","int","0");
// 	context.addEntry(new CMRProjectAlias("x^i_j","x_i^j",true)).captureName();
// 	CMRProjectCodeEquation & eq = root.addEquation("a","a^b_c");
// 	
// 	transf.run(root);
// 	
// 	EXPECT_EQ("a^c_b",eq.getFormulas().getString());
// }
