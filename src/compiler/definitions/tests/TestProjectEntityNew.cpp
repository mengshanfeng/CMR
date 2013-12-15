/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <ProjectEntityNew.h>
#include <parsor/LatexFormula.h>
#include <sstream>
#include "MockProjectEntityNew.h"

/**********************  USING  *********************/
using namespace testing;
using namespace std;
using namespace CMRCompiler;

/*******************  FUNCTION  *********************/
TEST(TestProjectEntityNew,testConstructor)
{
	MockProjectEntityNew entity("A_{i,j}^2","test");
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntityNew,testGetLatexName)
{
	MockProjectEntityNew entity("A_{i,j}^2","test");
	EXPECT_EQ("A_{i,j}^2",entity.getLatexName());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntityNew,testChangeCaptureType_ok)
{
	MockProjectEntityNew entity("A_{i,j}^2","test");
	entity.markForCapture("i");
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntityNew,testChangeCaptureType_invalid)
{
	MockProjectEntityNew entity("A_{i,j}^2","test");
	EXPECT_THROW(entity.markForCapture("m"),LatexException);
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntityNew,testMatch_1)
{
	MockProjectEntityNew entity("A_{eq,k}","test");
	entity.markForCapture("k");
	LatexEntity le("A_{eq,42}");
	EXPECT_TRUE(entity.match(le));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntityNew,testMatch_2)
{
	MockProjectEntityNew entity("A_{eq,k}","test");
	entity.markForCapture("k");
	LatexEntity le("A_{eq2,42}");
	EXPECT_FALSE(entity.match(le));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntityNew,testMatch_3)
{
	MockProjectEntityNew entity("A_{eq,k}","test");
	entity.markForCapture("k");
	LatexEntity le("A_{eq}");
	EXPECT_FALSE(entity.match(le));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntityNew,testMatch_4)
{
	MockProjectEntityNew entity("A_{eq,k}","test");
	entity.markForCapture("k");
	LatexEntity le("A_{eq,42,43}");
	EXPECT_FALSE(entity.match(le));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntityNew,testMatch_5)
{
	MockProjectEntityNew entity("A_{eq,k}^{4}","test");
	entity.markForCapture("k");
	LatexEntity le("A_{eq,42}^4");
	EXPECT_TRUE(entity.match(le));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntityNew,testMatch_6)
{
	MockProjectEntityNew entity("A_{eq,k}","test");
	entity.markForCapture("k");
	LatexEntity le("A_{eq,42}^2");
	EXPECT_TRUE(entity.match(le));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntityNew,testCapture_1)
{
	MockProjectEntityNew entity("A_{a,b}^{c,d}","test");
	entity.markForCapture("a");
	entity.markForCapture("b");
	entity.markForCapture("c");
	entity.markForCapture("d");

	LatexEntity le("A_{1,2}^{3,4}");
	
	EXPECT_TRUE(entity.match(le));
	
	FormulaMatcherResult capture;
	entity.capture(le,capture);
	
	EXPECT_EQ(4,capture.cntCaptureMatch);
	EXPECT_EQ("1",capture.captures["a"]->getString());
	EXPECT_EQ("2",capture.captures["b"]->getString());
	EXPECT_EQ("3",capture.captures["c"]->getString());
	EXPECT_EQ("4",capture.captures["d"]->getString());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntityNew,testCapture_2)
{
	MockProjectEntityNew entity("\\frac{a}{b}","test");
	entity.markForCapture("a");
	entity.markForCapture("b");

	LatexEntity le("\\frac{1}{2}");
	
	EXPECT_TRUE(entity.match(le));
	
	FormulaMatcherResult capture;
	entity.capture(le,capture);
	
	EXPECT_EQ(2,capture.cntCaptureMatch);
	EXPECT_EQ("1",capture.captures["a"]->getString());
	EXPECT_EQ("2",capture.captures["b"]->getString());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntityNew,testCapture_3)
{
	MockProjectEntityNew entity("A_{a,b}^{c,d}","test");
	entity.markForCapture("a");
	entity.markForCapture("b");
	entity.markForCapture("c");
	entity.markForCapture("d");

	LatexEntity le("A_{1,2,5}^{3,4}");
	
	EXPECT_FALSE(entity.match(le));
	
	FormulaMatcherResult capture;
	EXPECT_THROW(entity.capture(le,capture),LatexException);
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntityNew,testCapture_4)
{
	MockProjectEntityNew entity("A_{eq,k}","test");
	entity.markForCapture("k");
	LatexEntity le("A_{eq,42}^2");
	
	FormulaMatcherResult capture;
	entity.capture(le,capture);
	
	EXPECT_EQ(2,capture.cntCaptureMatch);
	ASSERT_TRUE(capture.hasExponent());
	EXPECT_EQ("2",capture.exponent->getString());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntityNew,testCapture_wildcard_1)
{
	MockProjectEntityNew entity("x_{eq,k}","test");
	entity.markForCapture("k");
	entity.markForCapture("x");
	LatexEntity le("A_{eq,42}^2");
	
	FormulaMatcherResult capture;
	entity.capture(le,capture);
	
	EXPECT_EQ(3,capture.cntCaptureMatch);
	ASSERT_TRUE(capture.hasExponent());
	EXPECT_EQ("2",capture.exponent->getString());
	EXPECT_EQ("A",capture.captures["x"]->getString());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntityNew,testCapture_wildcard_2)
{
	MockProjectEntityNew entity("x^2","test");
	entity.markForCapture("x");
	LatexEntity le("A^2");
	
	FormulaMatcherResult capture;
	entity.capture(le,capture);
	
	EXPECT_EQ(1,capture.cntCaptureMatch);
	EXPECT_EQ("A",capture.captures["x"]->getString());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntityNew,testCapture_wildcard_3)
{
	MockProjectEntityNew entity("x^a","test");
	entity.markForCapture("a");
	entity.markForCapture("x");
	LatexEntity le("A^2");
	
	FormulaMatcherResult capture;
	entity.capture(le,capture);
	
	EXPECT_EQ(2,capture.cntCaptureMatch);
	EXPECT_EQ("2",capture.captures["a"]->getString());
	EXPECT_EQ("A",capture.captures["x"]->getString());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntityNew,testCapture_wildcard_4)
{
	MockProjectEntityNew entity("x^a","test");
	entity.markForCapture("a");
	entity.markForCapture("x");
	LatexEntity le("(A+B)^2");
	
	FormulaMatcherResult capture;
	entity.capture(le,capture);
	
	EXPECT_EQ(2,capture.cntCaptureMatch);
	EXPECT_EQ("2",capture.captures["a"]->getString());
	EXPECT_EQ("A+B",capture.captures["x"]->getString());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntityNew,testGetLongName)
{
	MockProjectEntityNew entity("A_{i,j}^2","test");
	EXPECT_EQ("test",entity.getLongName());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntityNew,testOperatorStream)
{
	MockProjectEntityNew entity("A_{ i , j }^{ 5 , k}","test");
	stringstream out;
	out << entity;
	EXPECT_EQ("A_{i,j}^{5,k}",out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntityNew,testPrintDebug)
{
	MockProjectEntityNew entity("A_{ i , j }^{ 5 , k}","test");
	entity.markForCapture("j");
	
	stringstream out;
	entity.printDebug(out);
	
	EXPECT_EQ("A_{i,[j]}^{5,k}",out.str());
}
