/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <svUnitTest.h>
#include <CMRProjectContext.h>
#include <../parsor/CMRLatexFormula.h>
#include <sstream>
#include "MockProjectEntity.h"

/**********************  USING  *********************/
using namespace svUnitTest;
using namespace std;

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectContext,testConstructor)
{
	CMRProjectContext context1;
	CMRProjectContext context2(&context1);
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectContext,testCountTotalEntries_none)
{
	CMRProjectContext context;
	SVUT_ASSERT_EQUAL(0,context.countTotalEntries());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectContext,testCountTotalEntries_no_parent)
{
	CMRProjectContext context;
	MockProjectEntity entry1("A_i","testA");
	MockProjectEntity entry2("B_i","testB");
	context.addEntry(&entry1);
	context.addEntry(&entry2);
	SVUT_ASSERT_EQUAL(2,context.countTotalEntries());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectContext,testCountTotalEntries_parent)
{
	CMRProjectContext context;
	CMRProjectContext context2(&context);
	MockProjectEntity entry1("A_i","testA");
	MockProjectEntity entry2("B_i","testB");
	context.addEntry(&entry1);
	context2.addEntry(&entry2);
	SVUT_ASSERT_EQUAL(1,context.countTotalEntries());
	SVUT_ASSERT_EQUAL(2,context2.countTotalEntries());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectContext,testAddEntry_ok)
{
	CMRProjectContext context;
	MockProjectEntity entry1("A_i","testA");
	MockProjectEntity entry2("B_i","testB");
	MockProjectEntity entry3("B","testC");
	MockProjectEntity entry4("A_j","testDs");
	SVUT_ASSERT_NOT_THROW(CMRLatexException,context.addEntry(&entry1));
	SVUT_ASSERT_NOT_THROW(CMRLatexException,context.addEntry(&entry2));
	SVUT_ASSERT_NOT_THROW(CMRLatexException,context.addEntry(&entry3));
	SVUT_ASSERT_NOT_THROW(CMRLatexException,context.addEntry(&entry4));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectContext,testAddEntry_conflict)
{
	CMRProjectContext context;
	MockProjectEntity entry1("A_i","testA");
	MockProjectEntity entry2("A_i","testB");
	MockProjectEntity entry3("C_i","testA");
	SVUT_ASSERT_NOT_THROW(CMRLatexException, context.addEntry(&entry1));
	SVUT_ASSERT_THROW(CMRLatexException, context.addEntry(&entry2));
	SVUT_ASSERT_THROW(CMRLatexException, context.addEntry(&entry3));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectContext,testAddEntry_conflict_reverse)
{
	CMRProjectContext context;
	MockProjectEntity entry1("A_i","testA");
	entry1.changeCaptureType("i",CMR_CAPTURE_REQUIRED);
	MockProjectEntity entry2("A_j","testB");
	SVUT_ASSERT_NOT_THROW(CMRLatexException, context.addEntry(&entry1));
	SVUT_ASSERT_THROW(CMRLatexException, context.addEntry(&entry2));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectContext,testFind_ok_local)
{
	CMRProjectContext context;
	CMRProjectContext context2(&context);
	MockProjectEntity entry1("A_i","testA");
	MockProjectEntity entry2("B_i","testB");
	context.addEntry(&entry1);
	context2.addEntry(&entry2);
	
	CMRLatexEntity2 e("A_i");
	SVUT_ASSERT_SAME(&entry1,context.find(e));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectContext,testFind_ok_parent)
{
	CMRProjectContext context;
	CMRProjectContext context2(&context);
	MockProjectEntity entry1("A_i","testA");
	MockProjectEntity entry2("B_i","testB");
	context.addEntry(&entry1);
	context2.addEntry(&entry2);
	
	CMRLatexEntity2 e("A_i");
	SVUT_ASSERT_SAME(&entry1,context2.find(e));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectContext,testFind_not_found)
{
	CMRProjectContext context;
	CMRProjectContext context2(&context);
	MockProjectEntity entry1("A_i","testA");
	MockProjectEntity entry2("B_i","testB");
	context.addEntry(&entry1);
	context2.addEntry(&entry2);
	
	CMRLatexEntity2 e("C_i");
	SVUT_ASSERT_NULL(context2.find(e));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectContext,testPrintDebug)
{
	CMRProjectContext context;
	CMRProjectContext context2(&context);
	MockProjectEntity entry1("A_i","testA");
	MockProjectEntity entry2("B_i","testB");
	context.addEntry(&entry1);
	context2.addEntry(&entry2);
	
	stringstream tmp;
	context2.printDebug(tmp);
	SVUT_ASSERT_FALSE(tmp.str().empty());
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
