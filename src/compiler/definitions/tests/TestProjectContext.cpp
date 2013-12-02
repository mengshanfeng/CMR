/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <ProjectContext.h>
#include <../parsor/LatexFormula.h>
#include <sstream>
#include "MockProjectEntity.h"

#define EXPECT_NULL(ptr) EXPECT_EQ((void*)NULL,(ptr))

/**********************  USING  *********************/
using namespace testing;
using namespace std;
using namespace CMRCompiler;

/*******************  FUNCTION  *********************/
TEST(TestProjectContext,testConstructor)
{
	ProjectContext context1;
	ProjectContext context2(&context1);
}

/*******************  FUNCTION  *********************/
TEST(TestProjectContext,testCountTotalEntries_none)
{
	ProjectContext context;
	EXPECT_EQ(0,context.countTotalEntries());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectContext,testCountTotalEntries_no_parent)
{
	ProjectContext context;
	MockProjectEntity entry1("A_i","testA");
	MockProjectEntity entry2("B_i","testB");
	context.addEntry(&entry1);
	context.addEntry(&entry2);
	EXPECT_EQ(2,context.countTotalEntries());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectContext,testCountTotalEntries_parent)
{
	ProjectContext context;
	ProjectContext context2(&context);
	MockProjectEntity entry1("A_i","testA");
	MockProjectEntity entry2("B_i","testB");
	context.addEntry(&entry1);
	context2.addEntry(&entry2);
	EXPECT_EQ(1,context.countTotalEntries());
	EXPECT_EQ(2,context2.countTotalEntries());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectContext,testAddEntry_ok)
{
	ProjectContext context;
	MockProjectEntity entry1("A_i","testA");
	MockProjectEntity entry2("B_i","testB");
	MockProjectEntity entry3("B","testC");
	MockProjectEntity entry4("A_j","testDs");
	EXPECT_NO_THROW(context.addEntry(&entry1));
	EXPECT_NO_THROW(context.addEntry(&entry2));
	EXPECT_NO_THROW(context.addEntry(&entry3));
	EXPECT_NO_THROW(context.addEntry(&entry4));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectContext,testAddEntry_conflict)
{
	ProjectContext context;
	MockProjectEntity entry1("A_i","testA");
	MockProjectEntity entry2("A_i","testB");
	MockProjectEntity entry3("C_i","testA");
	EXPECT_NO_THROW( context.addEntry(&entry1));
	EXPECT_THROW(context.addEntry(&entry2),LatexException);
	EXPECT_THROW(context.addEntry(&entry3),LatexException);
}

/*******************  FUNCTION  *********************/
TEST(TestProjectContext,testAddEntry_conflict_reverse)
{
	ProjectContext context;
	MockProjectEntity entry1("A_i","testA");
	entry1.changeCaptureType("i",CAPTURE_REQUIRED);
	MockProjectEntity entry2("A_j","testB");
	EXPECT_NO_THROW(context.addEntry(&entry1));
	EXPECT_THROW(context.addEntry(&entry2),LatexException);
}

/*******************  FUNCTION  *********************/
TEST(TestProjectContext,testFind_ok_local)
{
	ProjectContext context;
	ProjectContext context2(&context);
	MockProjectEntity entry1("A_i","testA");
	MockProjectEntity entry2("B_i","testB");
	context.addEntry(&entry1);
	context2.addEntry(&entry2);
	
	LatexEntity e("A_i");
	EXPECT_EQ(&entry1,context.find(e));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectContext,testFind_ok_parent)
{
	ProjectContext context;
	ProjectContext context2(&context);
	MockProjectEntity entry1("A_i","testA");
	MockProjectEntity entry2("B_i","testB");
	context.addEntry(&entry1);
	context2.addEntry(&entry2);
	
	LatexEntity e("A_i");
	EXPECT_EQ(&entry1,context2.find(e));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectContext,testFind_not_found)
{
	ProjectContext context;
	ProjectContext context2(&context);
	MockProjectEntity entry1("A_i","testA");
	MockProjectEntity entry2("B_i","testB");
	context.addEntry(&entry1);
	context2.addEntry(&entry2);
	
	LatexEntity e("C_i");
	EXPECT_NULL(context2.find(e));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectContext,testPrintDebug)
{
	ProjectContext context;
	ProjectContext context2(&context);
	MockProjectEntity entry1("A_i","testA");
	MockProjectEntity entry2("B_i","testB");
	context.addEntry(&entry1);
	context2.addEntry(&entry2);
	
	stringstream tmp;
	context2.printDebug(tmp);
	EXPECT_FALSE(tmp.str().empty());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectContext,testGetDeph)
{
	ProjectContext context;
	ProjectContext context2(&context);
	
	EXPECT_EQ(0,context.getDepth());
	EXPECT_EQ(1,context2.getDepth());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectContext,testGenTempNameLongName)
{
	ProjectContext context;
	ProjectContext context2(&context);
	ProjectContext context3(&context);
	
	EXPECT_EQ("temp_0_0",context.genTempName().longName);
	EXPECT_EQ("temp_0_1",context.genTempName().longName);
	EXPECT_EQ("temp_0_2",context.genTempName().longName);
	
	EXPECT_EQ("temp_1_3",context2.genTempName().longName);
	EXPECT_EQ("temp_1_4",context2.genTempName().longName);
	EXPECT_EQ("temp_1_5",context2.genTempName().longName);
	
	EXPECT_EQ("temp_1_6",context3.genTempName().longName);
	EXPECT_EQ("temp_1_7",context3.genTempName().longName);
	EXPECT_EQ("temp_1_8",context3.genTempName().longName);
}
