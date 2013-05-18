/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <svUnitTest.h>
#include "../CMRTransformationMarkNoTransf.h"
#include <CMRTransformationReplace.h>
#include <CMRTransformationImplicitMul.h>
#include <CMRTransformationExpandExpo.h>
#include <../definitions/CMRProjectCode.h>
#include <../definitions/CMRProjectIterator.h>
#include <../definitions/CMRProjectConstant.h>

/**********************  USING  *********************/
using namespace svUnitTest;

/*********************  CONSTS  *********************/

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestExpandExpo,testConstructor)
{
	CMRTransformationExpandExpo transf;
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestExpandExpo,testRun_simple_exp2)
{
	CMRTransformationExpandExpo transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	CMRProjectCodeEquation & eq = root.addEquation("a","a^2");
	
	transf.run(root);
	
	SVUT_ASSERT_EQUAL("(a*a)",eq.getFormulas().getString());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestExpandExpo,testRun_complex_exp2)
{
	CMRTransformationExpandExpo transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	CMRProjectCodeEquation & eq = root.addEquation("a","(a+5)^2");
	
	transf.run(root);
	
	SVUT_ASSERT_EQUAL("((a+5)*(a+5))",eq.getFormulas().getString());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestExpandExpo,testRun_simple_exp3)
{
	CMRTransformationExpandExpo transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	CMRProjectCodeEquation & eq = root.addEquation("a","a^3");
	
	transf.run(root);
	
	SVUT_ASSERT_EQUAL("(a*a*a)",eq.getFormulas().getString());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestExpandExpo,testRun_simple_exp55)
{
	CMRTransformationExpandExpo transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	CMRProjectCodeEquation & eq = root.addEquation("a","a^5.5");
	
	transf.run(root);
	
	SVUT_ASSERT_EQUAL("pow{a,5.5}",eq.getFormulas().getString());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestExpandExpo,testRun_complex_exp55)
{
	CMRTransformationExpandExpo transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	CMRProjectCodeEquation & eq = root.addEquation("a","(a+4)^5.5");
	
	transf.run(root);
	
	SVUT_ASSERT_EQUAL("pow{(a+4),5.5}",eq.getFormulas().getString());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestExpandExpo,testRun_simple_sqrt)
{
	CMRTransformationExpandExpo transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	CMRProjectCodeEquation & eq = root.addEquation("a","a^{1/2}");
	
	transf.run(root);
	
	SVUT_ASSERT_EQUAL("sqrt{a}",eq.getFormulas().getString());
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
