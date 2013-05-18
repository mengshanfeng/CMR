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
#include <../definitions/CMRProjectCode.h>
#include <../definitions/CMRProjectIterator.h>
#include <../definitions/CMRProjectConstant.h>

/**********************  USING  *********************/
using namespace svUnitTest;

/*********************  CONSTS  *********************/

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestImplicitMul,testConstructor)
{
	CMRTransformationImplicitMul transf;
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestImplicitMul,testRun_simple)
{
	CMRTransformationImplicitMul transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	CMRProjectCodeEquation & eq = root.addEquation("a","ab");
	
	transf.run(root);
	
	SVUT_ASSERT_EQUAL("a*b",eq.getFormulas().getString());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestImplicitMul,testRun_complex)
{
	CMRTransformationImplicitMul transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	CMRProjectCodeEquation & eq = root.addEquation("a","ab^{ab}+8a*5e\\frac{af}{5e}");

	transf.run(root);
	
	SVUT_ASSERT_EQUAL("a*b^{a*b}+8*a*5*e*\\frac{a*f}{5*e}",eq.getFormulas().getString());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestImplicitMul,testRun_start_minus)
{
	CMRTransformationImplicitMul transf;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	CMRProjectCodeEquation & eq = root.addEquation("a","-abc+5");

	transf.run(root);
	
	SVUT_ASSERT_EQUAL("-a*b*c+5",eq.getFormulas().getString());
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
