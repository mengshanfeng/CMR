/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <LatexParsorContext.h>

/**********************  USING  *********************/
using namespace testing;
using namespace CMRCompiler;

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testConstructor1)
{
	LatexParsorContext context1("test");
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testConstructor2)
{
	LatexParsorContext context1("test");
	LatexParsorContext context2(&context1,1,3);
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testGetPosition)
{
	LatexParsorContext context("test");
	EXPECT_EQ(0,context.getPosition());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testMove)
{
	LatexParsorContext context("test");
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
	LatexParsorContext context("  	test");
	EXPECT_EQ(0,context.getPosition());
	context.skipWhiteSpace();
	EXPECT_EQ(3,context.getPosition());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testIsEnd)
{
	LatexParsorContext context("test");
	EXPECT_EQ(0,context.getPosition());
	EXPECT_FALSE(context.isEnd());
	context.move(8);
	EXPECT_EQ(8,context.getPosition());
	EXPECT_TRUE(context.isEnd());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testOperatorPlusPlus)
{
	LatexParsorContext context("test");
	EXPECT_EQ(0,context.getPosition());
	++context;
	EXPECT_EQ(1,context.getPosition());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,getCurrent)
{
	LatexParsorContext context("test");
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
	LatexParsorContext context("test");
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
	LatexParsorContext context("test");
	EXPECT_EQ("test",context.getValue());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testStartBy_string)
{
	LatexParsorContext context("test");
	EXPECT_TRUE(context.startBy("te"));
	EXPECT_FALSE(context.startBy("es"));
	
	context.move(8);
	EXPECT_FALSE(context.startBy("te"));
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testStartBy_char)
{
	LatexParsorContext context("test");
	EXPECT_TRUE(context.startBy('t'));
	EXPECT_FALSE(context.startBy('e'));
	
	context.move(8);
	EXPECT_FALSE(context.startBy('e'));
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testExtractSubZone_ok)
{
	LatexParsorContext context("test(ok)");
	context.move(4);
	LatexParsorContext res = context.extractSubZone('(',')');
	EXPECT_EQ("ok",res.getValue());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testExtractSubZone_error)
{
	LatexParsorContext context("test(ok");
	context.move(4);
	EXPECT_THROW(context.extractSubZone('(',')'),LatexException);
}

/*******************  FUNCTION  *********************/
TEST(TestLatexParsorContext,testFatal)
{
	LatexParsorContext context("test");
	EXPECT_THROW(context.fatal("short message"),LatexException);
}
