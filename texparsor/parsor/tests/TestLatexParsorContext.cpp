/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <svUnitTest.h>
#include <CMRLatexParsorContext.h>

/**********************  USING  *********************/
using namespace svUnitTest;

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexParsorContext,testConstructor1)
{
	CMRLatexParsorContext context1("test");
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexParsorContext,testConstructor2)
{
	CMRLatexParsorContext context1("test");
	CMRLatexParsorContext context2(&context1,1,3);
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexParsorContext,testGetPosition)
{
	CMRLatexParsorContext context("test");
	SVUT_ASSERT_EQUAL(0,context.getPosition());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexParsorContext,testMove)
{
	CMRLatexParsorContext context("test");
	SVUT_ASSERT_EQUAL(0,context.getPosition());
	context.move(1);
	SVUT_ASSERT_EQUAL(1,context.getPosition());
	context.move(2);
	SVUT_ASSERT_EQUAL(3,context.getPosition());
	context.move(-1);
	SVUT_ASSERT_EQUAL(2,context.getPosition());
	context.move(5);
	SVUT_ASSERT_EQUAL(7,context.getPosition());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexParsorContext,testSkipWhiteSpace)
{
	CMRLatexParsorContext context("  	test");
	SVUT_ASSERT_EQUAL(0,context.getPosition());
	context.skipWhiteSpace();
	SVUT_ASSERT_EQUAL(3,context.getPosition());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexParsorContext,testIsEnd)
{
	CMRLatexParsorContext context("test");
	SVUT_ASSERT_EQUAL(0,context.getPosition());
	SVUT_ASSERT_FALSE(context.isEnd());
	context.move(8);
	SVUT_ASSERT_EQUAL(8,context.getPosition());
	SVUT_ASSERT_TRUE(context.isEnd());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexParsorContext,testOperatorPlusPlus)
{
	CMRLatexParsorContext context("test");
	SVUT_ASSERT_EQUAL(0,context.getPosition());
	++context;
	SVUT_ASSERT_EQUAL(1,context.getPosition());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexParsorContext,getCurrent)
{
	CMRLatexParsorContext context("test");
	SVUT_ASSERT_EQUAL('t',context.getCurrent());
	++context;
	SVUT_ASSERT_EQUAL('e',context.getCurrent());
	++context;
	SVUT_ASSERT_EQUAL('s',context.getCurrent());
	++context;
	SVUT_ASSERT_EQUAL('t',context.getCurrent());
	++context;
	SVUT_ASSERT_EQUAL('\0',context.getCurrent());
	++context;
	SVUT_ASSERT_EQUAL('\0',context.getCurrent());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexParsorContext,testGetCurAndMove)
{
	CMRLatexParsorContext context("test");
	SVUT_ASSERT_EQUAL(0,context.getPosition());
	SVUT_ASSERT_EQUAL('t',context.getCurAndMove());
	SVUT_ASSERT_EQUAL(1,context.getPosition());
	SVUT_ASSERT_EQUAL('e',context.getCurAndMove());
	SVUT_ASSERT_EQUAL(2,context.getPosition());
	SVUT_ASSERT_EQUAL('s',context.getCurAndMove());
	SVUT_ASSERT_EQUAL(3,context.getPosition());
	SVUT_ASSERT_EQUAL('t',context.getCurAndMove());
	SVUT_ASSERT_EQUAL(4,context.getPosition());
	SVUT_ASSERT_EQUAL('\0',context.getCurAndMove());
	SVUT_ASSERT_EQUAL(4,context.getPosition());
	SVUT_ASSERT_EQUAL('\0',context.getCurAndMove());
	SVUT_ASSERT_EQUAL(4,context.getPosition());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexParsorContext,testGetValue)
{
	CMRLatexParsorContext context("test");
	SVUT_ASSERT_EQUAL("test",context.getValue());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexParsorContext,testStartBy_string)
{
	CMRLatexParsorContext context("test");
	SVUT_ASSERT_TRUE(context.startBy("te"));
	SVUT_ASSERT_FALSE(context.startBy("es"));
	
	context.move(8);
	SVUT_ASSERT_FALSE(context.startBy("te"));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexParsorContext,testStartBy_char)
{
	CMRLatexParsorContext context("test");
	SVUT_ASSERT_TRUE(context.startBy('t'));
	SVUT_ASSERT_FALSE(context.startBy('e'));
	
	context.move(8);
	SVUT_ASSERT_FALSE(context.startBy('e'));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexParsorContext,testExtractSubZone_ok)
{
	CMRLatexParsorContext context("test(ok)");
	context.move(4);
	CMRLatexParsorContext res = context.extractSubZone('(',')');
	SVUT_ASSERT_EQUAL("ok",res.getValue());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexParsorContext,testExtractSubZone_error)
{
	CMRLatexParsorContext context("test(ok");
	context.move(4);
	SVUT_ASSERT_THROW(CMRLatexException,context.extractSubZone('(',')'));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexParsorContext,testFatal)
{
	CMRLatexParsorContext context("test");
	SVUT_ASSERT_THROW(CMRLatexException,context.fatal("short message"));
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
