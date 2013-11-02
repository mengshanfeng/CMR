/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <CMRLatexParsorContext.h>

/**********************  USING  *********************/
using namespace testing;

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testConstructor1)
{
	CMRLatexParsorContext context1("test");
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testConstructor2)
{
	CMRLatexParsorContext context1("test");
	CMRLatexParsorContext context2(&context1,1,3);
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testGetPosition)
{
	CMRLatexParsorContext context("test");
	EXPECT_EQ(0,context.getPosition());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testMove)
{
	CMRLatexParsorContext context("test");
	EXPECT_EQ(0,context.getPosition());
	context.move(1);
	EXPECT_EQ(1,context.getPosition());
	context.move(2);
	EXPECT_EQ(3,context.getPosition());
	context.move(-1);
	EXPECT_EQ(2,context.getPosition());
	context.move(5);
	EXPECT_EQ(7,context.getPosition());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testSkipWhiteSpace)
{
	CMRLatexParsorContext context("  	test");
	EXPECT_EQ(0,context.getPosition());
	context.skipWhiteSpace();
	EXPECT_EQ(3,context.getPosition());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testIsEnd)
{
	CMRLatexParsorContext context("test");
	EXPECT_EQ(0,context.getPosition());
	EXPECT_FALSE(context.isEnd());
	context.move(8);
	EXPECT_EQ(8,context.getPosition());
	EXPECT_TRUE(context.isEnd());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testOperatorPlusPlus)
{
	CMRLatexParsorContext context("test");
	EXPECT_EQ(0,context.getPosition());
	++context;
	EXPECT_EQ(1,context.getPosition());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,getCurrent)
{
	CMRLatexParsorContext context("test");
	EXPECT_EQ('t',context.getCurrent());
	++context;
	EXPECT_EQ('e',context.getCurrent());
	++context;
	EXPECT_EQ('s',context.getCurrent());
	++context;
	EXPECT_EQ('t',context.getCurrent());
	++context;
	EXPECT_EQ('\0',context.getCurrent());
	++context;
	EXPECT_EQ('\0',context.getCurrent());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testGetCurAndMove)
{
	CMRLatexParsorContext context("test");
	EXPECT_EQ(0,context.getPosition());
	EXPECT_EQ('t',context.getCurAndMove());
	EXPECT_EQ(1,context.getPosition());
	EXPECT_EQ('e',context.getCurAndMove());
	EXPECT_EQ(2,context.getPosition());
	EXPECT_EQ('s',context.getCurAndMove());
	EXPECT_EQ(3,context.getPosition());
	EXPECT_EQ('t',context.getCurAndMove());
	EXPECT_EQ(4,context.getPosition());
	EXPECT_EQ('\0',context.getCurAndMove());
	EXPECT_EQ(4,context.getPosition());
	EXPECT_EQ('\0',context.getCurAndMove());
	EXPECT_EQ(4,context.getPosition());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testGetValue)
{
	CMRLatexParsorContext context("test");
	EXPECT_EQ("test",context.getValue());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testStartBy_string)
{
	CMRLatexParsorContext context("test");
	EXPECT_TRUE(context.startBy("te"));
	EXPECT_FALSE(context.startBy("es"));
	
	context.move(8);
	EXPECT_FALSE(context.startBy("te"));
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testStartBy_char)
{
	CMRLatexParsorContext context("test");
	EXPECT_TRUE(context.startBy('t'));
	EXPECT_FALSE(context.startBy('e'));
	
	context.move(8);
	EXPECT_FALSE(context.startBy('e'));
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testExtractSubZone_ok)
{
	CMRLatexParsorContext context("test(ok)");
	context.move(4);
	CMRLatexParsorContext res = context.extractSubZone('(',')');
	EXPECT_EQ("ok",res.getValue());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testExtractSubZone_error)
{
	CMRLatexParsorContext context("test(ok");
	context.move(4);
	EXPECT_THROW(context.extractSubZone('(',')'),CMRLatexException);
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testFatal)
{
	CMRLatexParsorContext context("test");
	EXPECT_THROW(context.fatal("short message"),CMRLatexException);
}
