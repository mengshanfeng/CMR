/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <ProjectCodeTree.h>
#include <sstream>
#include "MockProjectEntity.h"

#define EXPECT_NULL(ptr) EXPECT_EQ((void*)NULL,(ptr))

/**********************  USING  *********************/
using namespace testing;
using namespace std;

/*********************  STRUCT  *********************/
struct TestEntry
{
	TestEntry(int value) {this->value = value;};
	int value;
};

/*********************  TYPES  **********************/
class TestCodeTree : public ProjectCodeTree<TestCodeTree> 
{
	public:
		TestCodeTree(int value) : entry(value) {localParent = NULL;};
		virtual void onParentChange ( TestCodeTree* newParent ) {this->localParent = newParent;};
		TestEntry entry;
		TestCodeTree * localParent;
};

/*******************  FUNCTION  *********************/
TEST(TestProjectCodeTree,testConstructor)
{
	TestCodeTree tree(0);
}

/*******************  FUNCTION  *********************/
TEST(TestProjectCodeTree,testInsert_default)
{
	TestCodeTree tree(0);
	TestCodeTree * tree1 = new TestCodeTree(1);
	TestCodeTree * tree2 = new TestCodeTree(2);
	
	tree.insert(tree1);
	tree.insert(tree2);
	
	EXPECT_EQ(&tree,tree1->getParent());
	EXPECT_EQ(&tree,tree2->getParent());
	EXPECT_EQ(tree1,tree.getFirstChild());
	EXPECT_EQ(tree2,tree.getLastChild());
	EXPECT_EQ(tree2,tree1->getNext());
	EXPECT_EQ(tree1,tree2->getPrev());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectCodeTree,testInsert_after)
{
	TestCodeTree tree(0);
	TestCodeTree * tree1 = new TestCodeTree(1);
	TestCodeTree * tree2 = new TestCodeTree(2);
	
	tree.insert(tree1,CMR_INSERT_AFTER);
	tree.insert(tree2,CMR_INSERT_AFTER);
	
	EXPECT_NULL(tree.getParent());
	EXPECT_NULL(tree1->getParent());
	EXPECT_NULL(tree2->getParent());
	EXPECT_EQ(&tree,tree2->getPrev());
	EXPECT_EQ(tree2,tree1->getPrev());
	EXPECT_EQ(tree2,tree.getNext());
	EXPECT_EQ(tree1,tree2->getNext());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectCodeTree,testInsert_before)
{
	TestCodeTree tree(0);
	TestCodeTree * tree1 = new TestCodeTree(1);
	TestCodeTree * tree2 = new TestCodeTree(2);
	
	tree.insert(tree1,CMR_INSERT_BEFORE);
	tree.insert(tree2,CMR_INSERT_BEFORE);
	
	EXPECT_NULL(tree.getParent());
	EXPECT_NULL(tree1->getParent());
	EXPECT_NULL(tree2->getParent());
	EXPECT_EQ(tree2,tree.getPrev());
	EXPECT_EQ(tree1,tree2->getPrev());
	EXPECT_EQ(tree2,tree1->getNext());
	EXPECT_EQ(&tree,tree2->getNext());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectCodeTree,testInsert_inplace)
{
	TestCodeTree tree(0);
	TestCodeTree * tree1 = new TestCodeTree(1);
	TestCodeTree * tree2 = new TestCodeTree(2);
	
	tree.insert(tree1);
	tree1->insert(tree2,CMR_INSERT_INPLACE);
	
	EXPECT_NULL(tree1->getParent());
	EXPECT_EQ(&tree,tree2->getParent());
	EXPECT_EQ(tree2,tree.getFirstChild());
	EXPECT_EQ(tree2,tree.getLastChild());
	EXPECT_NULL(tree2->getNext());
	EXPECT_NULL(tree1->getNext());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectCodeTree,testInsert_last)
{
	TestCodeTree tree(0);
	TestCodeTree * tree1 = new TestCodeTree(1);
	TestCodeTree * tree2 = new TestCodeTree(2);
	
	tree.insert(tree1);
	tree1->insert(tree2,CMR_INSERT_LAST);
	
	EXPECT_NULL(tree2->getNext());
	EXPECT_EQ(&tree,tree2->getParent());
	EXPECT_EQ(tree2,tree.getLastChild());
	EXPECT_EQ(tree2,tree1->getNext());
	EXPECT_EQ(tree1,tree2->getPrev());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectCodeTree,testInsert_first)
{
	TestCodeTree tree(0);
	TestCodeTree * tree1 = new TestCodeTree(1);
	TestCodeTree * tree2 = new TestCodeTree(2);
	
	tree.insert(tree1);
	tree1->insert(tree2,CMR_INSERT_FIRST);
	
	EXPECT_NULL(tree2->getPrev());
	EXPECT_EQ(&tree,tree2->getParent());
	EXPECT_EQ(tree2,tree.getFirstChild());
	EXPECT_EQ(tree2,tree1->getPrev());
	EXPECT_EQ(tree1,tree2->getNext());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectCodeTree,testInsert_lastchild)
{
	TestCodeTree tree(0);
	TestCodeTree * tree1 = new TestCodeTree(1);
	TestCodeTree * tree2 = new TestCodeTree(2);
	
	tree.insert(tree1);
	tree.insert(tree2,CMR_INSERT_LAST_CHILD);
	
	EXPECT_NULL(tree2->getNext());
	EXPECT_EQ(&tree,tree2->getParent());
	EXPECT_EQ(tree2,tree.getLastChild());
	EXPECT_EQ(tree2,tree1->getNext());
	EXPECT_EQ(tree1,tree2->getPrev());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectCodeTree,testInsert_firstchild)
{
	TestCodeTree tree(0);
	TestCodeTree * tree1 = new TestCodeTree(1);
	TestCodeTree * tree2 = new TestCodeTree(2);
	
	tree.insert(tree1);
	tree.insert(tree2,CMR_INSERT_FIRST_CHILD);
	
	EXPECT_NULL(tree2->getPrev());
	EXPECT_EQ(&tree,tree2->getParent());
	EXPECT_EQ(tree2,tree.getFirstChild());
	EXPECT_EQ(tree2,tree1->getPrev());
	EXPECT_EQ(tree1,tree2->getNext());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectCodeTree,testDetach)
{
	TestCodeTree tree(0);
	TestCodeTree * tree1 = new TestCodeTree(1);
	TestCodeTree * tree2 = new TestCodeTree(1);
	
	tree.insert(tree1);
	tree.insert(tree2);
	tree1->detach();
	
	EXPECT_EQ(tree2,tree.getFirstChild());
	EXPECT_EQ(tree2,tree.getLastChild());
	EXPECT_NULL(tree1->getParent());
	EXPECT_NULL(tree1->getNext());
	EXPECT_NULL(tree1->getPrev());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectCodeTree,testClearChild)
{
	TestCodeTree tree(0);
	
	tree.insert(new TestCodeTree(1));
	tree.clearChilds();
	
	EXPECT_NULL(tree.getFirstChild());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectCodeTree,testHasChild)
{
	TestCodeTree tree(0);
	EXPECT_FALSE(tree.hasChild());
	
	tree.insert(new TestCodeTree(1));
	EXPECT_TRUE(tree.hasChild());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectCodeTree,testGetDepth)
{
	TestCodeTree tree(0);
	TestCodeTree * tree1 = tree.insert(new TestCodeTree(1));
	TestCodeTree * tree2 = tree1->insert(new TestCodeTree(1));
	
	EXPECT_EQ(0,tree.getDepth());
	EXPECT_EQ(1,tree1->getDepth());
	EXPECT_EQ(2,tree2->getDepth());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectCodeTree,testOnChangeParent)
{
	TestCodeTree tree(0);
	TestCodeTree * tree1 = new TestCodeTree(1);
	
	EXPECT_NULL(tree1->localParent);
	
	tree.insert(tree1);
	
	EXPECT_EQ(&tree,tree1->localParent);
	
	tree1->detach();
	
	EXPECT_NULL(tree1->localParent);
}
