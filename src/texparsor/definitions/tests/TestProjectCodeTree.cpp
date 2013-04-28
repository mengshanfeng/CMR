/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <svUnitTest.h>
#include <CMRProjectCodeTree.h>
#include <sstream>
#include "MockProjectEntity.h"

/**********************  USING  *********************/
using namespace svUnitTest;
using namespace std;

/*********************  STRUCT  *********************/
struct TestEntry
{
	TestEntry(int value) {this->value = value;};
	int value;
};

/*********************  TYPES  **********************/
class TestCodeTree : public CMRProjectCodeTree<TestCodeTree> 
{
	public:
		TestCodeTree(int value) : entry(value) {localParent = NULL;};
		virtual void onParentChange ( TestCodeTree* newParent ) {this->localParent = newParent;};
		TestEntry entry;
		TestCodeTree * localParent;
};

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectCodeTree,testConstructor)
{
	TestCodeTree tree(0);
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectCodeTree,testInsert_default)
{
	TestCodeTree tree(0);
	TestCodeTree * tree1 = new TestCodeTree(1);
	TestCodeTree * tree2 = new TestCodeTree(2);
	
	tree.insert(tree1);
	tree.insert(tree2);
	
	SVUT_ASSERT_SAME(&tree,tree1->getParent());
	SVUT_ASSERT_SAME(&tree,tree2->getParent());
	SVUT_ASSERT_SAME(tree1,tree.getFirstChild());
	SVUT_ASSERT_SAME(tree2,tree.getLastChild());
	SVUT_ASSERT_SAME(tree2,tree1->getNext());
	SVUT_ASSERT_SAME(tree1,tree2->getPrev());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectCodeTree,testInsert_after)
{
	TestCodeTree tree(0);
	TestCodeTree * tree1 = new TestCodeTree(1);
	TestCodeTree * tree2 = new TestCodeTree(2);
	
	tree.insert(tree1,CMR_INSERT_AFTER);
	tree.insert(tree2,CMR_INSERT_AFTER);
	
	SVUT_ASSERT_NULL(tree.getParent());
	SVUT_ASSERT_NULL(tree1->getParent());
	SVUT_ASSERT_NULL(tree2->getParent());
	SVUT_ASSERT_SAME(&tree,tree2->getPrev());
	SVUT_ASSERT_SAME(tree2,tree1->getPrev());
	SVUT_ASSERT_SAME(tree2,tree.getNext());
	SVUT_ASSERT_SAME(tree1,tree2->getNext());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectCodeTree,testInsert_before)
{
	TestCodeTree tree(0);
	TestCodeTree * tree1 = new TestCodeTree(1);
	TestCodeTree * tree2 = new TestCodeTree(2);
	
	tree.insert(tree1,CMR_INSERT_BEFORE);
	tree.insert(tree2,CMR_INSERT_BEFORE);
	
	SVUT_ASSERT_NULL(tree.getParent());
	SVUT_ASSERT_NULL(tree1->getParent());
	SVUT_ASSERT_NULL(tree2->getParent());
	SVUT_ASSERT_SAME(tree2,tree.getPrev());
	SVUT_ASSERT_SAME(tree1,tree2->getPrev());
	SVUT_ASSERT_SAME(tree2,tree1->getNext());
	SVUT_ASSERT_SAME(&tree,tree2->getNext());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectCodeTree,testInsert_inplace)
{
	TestCodeTree tree(0);
	TestCodeTree * tree1 = new TestCodeTree(1);
	TestCodeTree * tree2 = new TestCodeTree(2);
	
	tree.insert(tree1);
	tree1->insert(tree2,CMR_INSERT_INPLACE);
	
	SVUT_ASSERT_NULL(tree1->getParent());
	SVUT_ASSERT_SAME(&tree,tree2->getParent());
	SVUT_ASSERT_SAME(tree2,tree.getFirstChild());
	SVUT_ASSERT_SAME(tree2,tree.getLastChild());
	SVUT_ASSERT_NULL(tree2->getNext());
	SVUT_ASSERT_NULL(tree1->getNext());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectCodeTree,testInsert_last)
{
	TestCodeTree tree(0);
	TestCodeTree * tree1 = new TestCodeTree(1);
	TestCodeTree * tree2 = new TestCodeTree(2);
	
	tree.insert(tree1);
	tree1->insert(tree2,CMR_INSERT_LAST);
	
	SVUT_ASSERT_NULL(tree2->getNext());
	SVUT_ASSERT_SAME(&tree,tree2->getParent());
	SVUT_ASSERT_SAME(tree2,tree.getLastChild());
	SVUT_ASSERT_SAME(tree2,tree1->getNext());
	SVUT_ASSERT_SAME(tree1,tree2->getPrev());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectCodeTree,testInsert_first)
{
	TestCodeTree tree(0);
	TestCodeTree * tree1 = new TestCodeTree(1);
	TestCodeTree * tree2 = new TestCodeTree(2);
	
	tree.insert(tree1);
	tree1->insert(tree2,CMR_INSERT_FIRST);
	
	SVUT_ASSERT_NULL(tree2->getPrev());
	SVUT_ASSERT_SAME(&tree,tree2->getParent());
	SVUT_ASSERT_SAME(tree2,tree.getFirstChild());
	SVUT_ASSERT_SAME(tree2,tree1->getPrev());
	SVUT_ASSERT_SAME(tree1,tree2->getNext());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectCodeTree,testInsert_lastchild)
{
	TestCodeTree tree(0);
	TestCodeTree * tree1 = new TestCodeTree(1);
	TestCodeTree * tree2 = new TestCodeTree(2);
	
	tree.insert(tree1);
	tree.insert(tree2,CMR_INSERT_LAST_CHILD);
	
	SVUT_ASSERT_NULL(tree2->getNext());
	SVUT_ASSERT_SAME(&tree,tree2->getParent());
	SVUT_ASSERT_SAME(tree2,tree.getLastChild());
	SVUT_ASSERT_SAME(tree2,tree1->getNext());
	SVUT_ASSERT_SAME(tree1,tree2->getPrev());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectCodeTree,testInsert_firstchild)
{
	TestCodeTree tree(0);
	TestCodeTree * tree1 = new TestCodeTree(1);
	TestCodeTree * tree2 = new TestCodeTree(2);
	
	tree.insert(tree1);
	tree.insert(tree2,CMR_INSERT_FIRST_CHILD);
	
	SVUT_ASSERT_NULL(tree2->getPrev());
	SVUT_ASSERT_SAME(&tree,tree2->getParent());
	SVUT_ASSERT_SAME(tree2,tree.getFirstChild());
	SVUT_ASSERT_SAME(tree2,tree1->getPrev());
	SVUT_ASSERT_SAME(tree1,tree2->getNext());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectCodeTree,testDetach)
{
	TestCodeTree tree(0);
	TestCodeTree * tree1 = new TestCodeTree(1);
	TestCodeTree * tree2 = new TestCodeTree(1);
	
	tree.insert(tree1);
	tree.insert(tree2);
	tree1->detach();
	
	SVUT_ASSERT_SAME(tree2,tree.getFirstChild());
	SVUT_ASSERT_SAME(tree2,tree.getLastChild());
	SVUT_ASSERT_NULL(tree1->getParent());
	SVUT_ASSERT_NULL(tree1->getNext());
	SVUT_ASSERT_NULL(tree1->getPrev());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectCodeTree,testClearChild)
{
	TestCodeTree tree(0);
	
	tree.insert(new TestCodeTree(1));
	tree.clearChilds();
	
	SVUT_ASSERT_NULL(tree.getFirstChild());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectCodeTree,testHasChild)
{
	TestCodeTree tree(0);
	SVUT_ASSERT_FALSE(tree.hasChild());
	
	tree.insert(new TestCodeTree(1));
	SVUT_ASSERT_TRUE(tree.hasChild());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectCodeTree,testGetDepth)
{
	TestCodeTree tree(0);
	TestCodeTree * tree1 = tree.insert(new TestCodeTree(1));
	TestCodeTree * tree2 = tree1->insert(new TestCodeTree(1));
	
	SVUT_ASSERT_EQUAL(0,tree.getDepth());
	SVUT_ASSERT_EQUAL(1,tree1->getDepth());
	SVUT_ASSERT_EQUAL(2,tree2->getDepth());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectCodeTree,testOnChangeParent)
{
	TestCodeTree tree(0);
	TestCodeTree * tree1 = new TestCodeTree(1);
	
	SVUT_ASSERT_NULL(tree1->localParent);
	
	tree.insert(tree1);
	
	SVUT_ASSERT_EQUAL(&tree,tree1->localParent);
	
	tree1->detach();
	
	SVUT_ASSERT_NULL(tree1->localParent);
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
