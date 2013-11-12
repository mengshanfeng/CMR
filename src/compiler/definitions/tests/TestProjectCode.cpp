/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <CMRProjectCode.h>
#include <../parsor/LatexFormula.h>
#include <sstream>
#include "MockProjectEntity.h"
#include "ProjectIterator.h"

/**********************  USING  *********************/
using namespace testing;
using namespace std;
using namespace CMRCompiler;

/*******************  FUNCTION  *********************/
TEST(TestProjectCodeNode,testConstructor)
{
	CMRProjectCodeNode root;
}

/*******************  FUNCTION  *********************/
TEST(TestProjectCodeNode,testAddSubBlock)
{
	CMRProjectCodeNode root;
	
	CMRProjectCodeNode & child = root.addSubBlock();
	
	EXPECT_EQ(&root,child.getParent());
	EXPECT_EQ(CMR_PROJECT_CODE_NODE,child.getType());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectCodeNode,testAddEquation)
{
	CMRProjectCodeNode root;
	
	CMRProjectCodeEquation & eq = root.addEquation("A_i","b * c + 5");
	
	EXPECT_EQ(&root,eq.getParent());
	EXPECT_EQ(CMR_PROJECT_CODE_EQUATION,eq.getType());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectCodeNode,testIteratorLoop_ok)
{
	CMRProjectContext context;	
	CMRProjectCodeNode root(&context);
	
	ProjectIterator it("k","itK",0,9);
	context.addEntry(&it);
	
	CMRProjectCodeIteratorLoop & loop = root.addIteratorLoop("k");

	EXPECT_EQ(&root,loop.getParent());
	ASSERT_NO_THROW(loop.getIterator());
	EXPECT_EQ(CMR_PROJECT_CODE_ITERATOR,loop.getType());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectCodeNode,testIteratorLoop_error)
{
	CMRProjectCodeNode root;
	
	CMRProjectCodeIteratorLoop & loop = root.addIteratorLoop("k");
	
	EXPECT_EQ(&root,loop.getParent());
	EXPECT_THROW(loop.getIterator(),LatexException);
}

TEST(TestProjectCodeNode,testAddLocalVariable)
{
	CMRProjectCodeNode root;
	
	root.addLocalVariable("k","testK","int","1");
}
