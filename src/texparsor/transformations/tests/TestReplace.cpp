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
#include <../definitions/CMRProjectCode.h>
#include <../definitions/CMRProjectIterator.h>
#include <../definitions/CMRProjectConstant.h>

/**********************  USING  *********************/
using namespace svUnitTest;

/*********************  CONSTS  *********************/

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestReplace,testConstructor)
{
	CMRProjectCaptureMap map;
	CMRTransformationReplace transf(&map);
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestReplace,testRun_single)
{
	CMRProjectCaptureMap map;
	CMRLatexFormulas2 f("c");
	map["b"] = &f;
	CMRTransformationReplace transf(&map);
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	CMRProjectCodeEquation & eq = root.addEquation("a","4 * b + \\frac{1+b}{b}");
	
	transf.run(root);
	
	SVUT_ASSERT_EQUAL("4*c+\\frac{1+c}{c}",eq.getFormulas().getString());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestReplace,testRun_complex)
{
	CMRProjectCaptureMap map;
	CMRLatexFormulas2 f("c*c");
	map["b"] = &f;
	CMRTransformationReplace transf(&map);
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	CMRProjectCodeEquation & eq = root.addEquation("a","4 * b + \\frac{1+b}{b}");
	
	transf.run(root);
	
	SVUT_ASSERT_EQUAL("4*(c*c)+\\frac{1+(c*c)}{(c*c)}",eq.getFormulas().getString());
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
