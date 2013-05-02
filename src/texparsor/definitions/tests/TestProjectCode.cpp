/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <svUnitTest.h>
#include <CMRProjectCode.h>
#include <../parsor/CMRLatexFormula.h>
#include <sstream>
#include "MockProjectEntity.h"
#include <CMRProjectIterator.h>

/**********************  USING  *********************/
using namespace svUnitTest;
using namespace std;

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectCodeNode,testConstructor)
{
	CMRProjectCodeNode root;
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectCodeNode,testAddSubBlock)
{
	CMRProjectCodeNode root;
	
	CMRProjectCodeNode & child = root.addSubBlock();
	
	SVUT_ASSERT_SAME(&root,child.getParent());
	SVUT_ASSERT_EQUAL(CMR_PROJECT_CODE_NODE,child.getType());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectCodeNode,testAddEquation)
{
	CMRProjectCodeNode root;
	
	CMRProjectCodeEquation & eq = root.addEquation("A_i","b * c + 5");
	
	SVUT_ASSERT_SAME(&root,eq.getParent());
	SVUT_ASSERT_EQUAL(CMR_PROJECT_CODE_EQUATION,eq.getType());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectCodeNode,testIteratorLoop_ok)
{
	CMRProjectContext context;	
	CMRProjectCodeNode root(&context);
	
	CMRProjectIterator it("k","itK",0,9);
	context.addEntry(&it);
	
	CMRProjectCodeIteratorLoop & loop = root.addIteratorLoop("k");

	SVUT_ASSERT_SAME(&root,loop.getParent());
	SVUT_ASSERT_NOT_THROW(CMRLatexException,loop.getIterator());
	SVUT_ASSERT_EQUAL(CMR_PROJECT_CODE_ITERATOR,loop.getType());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectCodeNode,testIteratorLoop_error)
{
	CMRProjectCodeNode root;
	
	CMRProjectCodeIteratorLoop & loop = root.addIteratorLoop("k");
	
	SVUT_ASSERT_SAME(&root,loop.getParent());
	SVUT_ASSERT_THROW(CMRLatexException,loop.getIterator());
}

SVUT_DECLARE_FLAT_TEST(TestProjectCodeNode,testAddLocalVariable)
{
	CMRProjectCodeNode root;
	
	root.addLocalVariable("k","testK","int","1");
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
