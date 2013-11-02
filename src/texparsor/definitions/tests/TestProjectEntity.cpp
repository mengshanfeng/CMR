/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <CMRProjectEntity.h>
#include <../parsor/CMRLatexFormula.h>
#include <sstream>
#include "MockProjectEntity.h"

/**********************  USING  *********************/
using namespace testing;
using namespace std;

/*********************  CONSTS  *********************/
static const char TEST_CST_1[] = "Entity :\n\
    - latexName  : A_{i,j}^{5,k}\n\
    - shortName  : A\n\
    - longName   : test\n\
    - exponent   : 5, k\n\
    - indices    : i, [j]\n\
    - parameters : \n";

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testConstructor)
{
	MockProjectEntity entity("A_{i,j}^2","test");
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testGetLatexName)
{
	MockProjectEntity entity("A_{i,j}^2","test");
	EXPECT_EQ("A_{i,j}^{2}",entity.getLatexName());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testAddIndice_ok)
{
	MockProjectEntity entity("A_{i,j}^2","test");
	entity.changeCaptureType("i",CMR_CAPTURE_REQUIRED);
	EXPECT_EQ("A_{i,j}^{2}",entity.getLatexName());
	entity.addIndice("k");
	EXPECT_EQ("A_{i,j,k}^{2}",entity.getLatexName());
	EXPECT_NO_THROW(entity.addIndice("i"));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testAddIndice_duplicate)
{
	MockProjectEntity entity("A_{i,j}^2","test");
	entity.changeCaptureType("i",CMR_CAPTURE_REQUIRED);
	EXPECT_EQ("A_{i,j}^{2}",entity.getLatexName());
	EXPECT_THROW(entity.addIndice("i",CMR_CAPTURE_REQUIRED),CMRLatexException);
	entity.addIndice("2");
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testAddExponent_ok)
{
	MockProjectEntity entity("A_{i,j}^2","test");
	entity.changeCaptureType("i",CMR_CAPTURE_REQUIRED);
	EXPECT_EQ("A_{i,j}^{2}",entity.getLatexName());
	entity.addExponent("k");
	EXPECT_EQ("A_{i,j}^{2,k}",entity.getLatexName());
	EXPECT_NO_THROW(entity.addExponent("i"));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testAddExponent_duplicate)
{
	MockProjectEntity entity("A_{i,j}^2","test");
	entity.changeCaptureType("i",CMR_CAPTURE_REQUIRED);
	EXPECT_EQ("A_{i,j}^{2}",entity.getLatexName());
	EXPECT_THROW(entity.addExponent("i",CMR_CAPTURE_REQUIRED),CMRLatexException);
	entity.addExponent("2");
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testAddParameter_ok)
{
	MockProjectEntity entity("A_{i,j}^2","test");
	entity.changeCaptureType("i",CMR_CAPTURE_REQUIRED);
	EXPECT_EQ("A_{i,j}^{2}",entity.getLatexName());
	entity.addParameter("k");
	EXPECT_EQ("A_{i,j}^{2}{k}",entity.getLatexName());
	entity.addParameter("l");
	EXPECT_EQ("A_{i,j}^{2}{k}{l}",entity.getLatexName());
	EXPECT_NO_THROW(entity.addParameter("i"));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testAddParameter_duplicate)
{
	MockProjectEntity entity("A_{i,j}^2","test");
	entity.changeCaptureType("i",CMR_CAPTURE_REQUIRED);
	EXPECT_EQ("A_{i,j}^{2}",entity.getLatexName());
	EXPECT_THROW(entity.addParameter("i",CMR_CAPTURE_REQUIRED),CMRLatexException);
	entity.addParameter("2");
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testChangeCaptureType_ok)
{
	MockProjectEntity entity("A_{i,j}^2","test");
	entity.changeCaptureType("i",CMR_CAPTURE_REQUIRED);
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testChangeCaptureType_invalid)
{
	MockProjectEntity entity("A_{i,j}^2","test");
	EXPECT_THROW(entity.changeCaptureType("m",CMR_CAPTURE_REQUIRED),CMRLatexException);
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testMatch_1)
{
	MockProjectEntity entity("A_{eq,k}","test");
	entity.changeCaptureType("k",CMR_CAPTURE_REQUIRED);
	CMRLatexEntity2 le("A_{eq,42}");
	EXPECT_TRUE(entity.match(le));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testMatch_2)
{
	MockProjectEntity entity("A_{eq,k}","test");
	entity.changeCaptureType("k",CMR_CAPTURE_REQUIRED);
	CMRLatexEntity2 le("A_{eq2,42}");
	EXPECT_FALSE(entity.match(le));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testMatch_3)
{
	MockProjectEntity entity("A_{eq,k}","test");
	entity.changeCaptureType("k",CMR_CAPTURE_REQUIRED);
	CMRLatexEntity2 le("A_{eq}");
	EXPECT_FALSE(entity.match(le));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testMatch_4)
{
	MockProjectEntity entity("A_{eq,k}","test");
	entity.changeCaptureType("k",CMR_CAPTURE_REQUIRED);
	CMRLatexEntity2 le("A_{eq,42,43}");
	EXPECT_FALSE(entity.match(le));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testMatch_5)
{
	MockProjectEntity entity("A_{eq,k}^{4}","test");
	entity.changeCaptureType("k",CMR_CAPTURE_REQUIRED);
	CMRLatexEntity2 le("A_{eq,42}^4");
	EXPECT_TRUE(entity.match(le));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testMatch_6)
{
	MockProjectEntity entity("A_{eq,k}","test");
	entity.changeCaptureType("k",CMR_CAPTURE_REQUIRED);
	CMRLatexEntity2 le("A_{eq,42}^2");
	EXPECT_TRUE(entity.match(le));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testCapture_1)
{
	MockProjectEntity entity("A_{a,b}^{c,d}","test");
	entity.changeCaptureType("a",CMR_CAPTURE_REQUIRED);
	entity.changeCaptureType("b",CMR_CAPTURE_REQUIRED);
	entity.changeCaptureType("c",CMR_CAPTURE_REQUIRED);
	entity.changeCaptureType("d",CMR_CAPTURE_REQUIRED);

	CMRLatexEntity2 le("A_{1,2}^{3,4}");
	
	EXPECT_TRUE(entity.match(le));
	
	CMRProjectCaptureMap capture;
	entity.capture(le,capture);
	
	EXPECT_EQ(4,capture.size());
	EXPECT_EQ("1",capture["a"]->getString());
	EXPECT_EQ("2",capture["b"]->getString());
	EXPECT_EQ("3",capture["c"]->getString());
	EXPECT_EQ("4",capture["d"]->getString());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testCapture_2)
{
	MockProjectEntity entity("\\frac{a}{b}","test");
	entity.changeCaptureType("a",CMR_CAPTURE_REQUIRED);
	entity.changeCaptureType("b",CMR_CAPTURE_REQUIRED);

	CMRLatexEntity2 le("\\frac{1}{2}");
	
	EXPECT_TRUE(entity.match(le));
	
	CMRProjectCaptureMap capture;
	entity.capture(le,capture);
	
	EXPECT_EQ(2,capture.size());
	EXPECT_EQ("1",capture["a"]->getString());
	EXPECT_EQ("2",capture["b"]->getString());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testCapture_3)
{
	MockProjectEntity entity("A_{a,b}^{c,d}","test");
	entity.changeCaptureType("a",CMR_CAPTURE_REQUIRED);
	entity.changeCaptureType("b",CMR_CAPTURE_REQUIRED);
	entity.changeCaptureType("c",CMR_CAPTURE_REQUIRED);
	entity.changeCaptureType("d",CMR_CAPTURE_REQUIRED);

	CMRLatexEntity2 le("A_{1,2,5}^{3,4}");
	
	EXPECT_FALSE(entity.match(le));
	
	CMRProjectCaptureMap capture;
	EXPECT_THROW(entity.capture(le,capture),CMRLatexException);
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testCapture_4)
{
	MockProjectEntity entity("A_{eq,k}","test");
	entity.changeCaptureType("k",CMR_CAPTURE_REQUIRED);
	CMRLatexEntity2 le("A_{eq,42}^2");
	
	CMRProjectCaptureMap capture;
	entity.capture(le,capture);
	
	EXPECT_EQ(2,capture.size());
	EXPECT_EQ("2",capture["cmrExponent"]->getString());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testGetShortName)
{
	MockProjectEntity entity("A_{i,j}^2","test");
	EXPECT_EQ("A",entity.getShortName());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testGetLongName)
{
	MockProjectEntity entity("A_{i,j}^2","test");
	EXPECT_EQ("test",entity.getLongName());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testOperatorStream)
{
	MockProjectEntity entity("A_{ i , j }^{ 5 , k}","test");
	stringstream out;
	out << entity;
	EXPECT_EQ("A_{i,j}^{5,k}",out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectEntity,testPrintDebug)
{
	MockProjectEntity entity("A_{ i , j }^{ 5 , k}","test");
	entity.changeCaptureType("j",CMR_CAPTURE_REQUIRED);
	
	stringstream out;
	entity.printDebug(out);
	
	EXPECT_EQ(TEST_CST_1,out.str());
}
