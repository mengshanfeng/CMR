/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <ProjectConstant.h>
#include <../parsor/LatexFormula.h>
#include <sstream>
#include "MockProjectEntity.h"
#include <ProjectContext.h>

/**********************  USING  *********************/
using namespace testing;
using namespace std;
using namespace CMRCompiler;

/*********************  CONSTS  *********************/
static const char TEST_CST_1[] = "Entity :\n\
    - latexName  : C_{\\cmr{cstid}{i},\\cmr{cstid}{j}}\n\
    - shortName  : C\n\
    - longName   : testC\n\
    - exponent   : \n\
    - indices    : [\\cmr{cstid}{i}], [\\cmr{cstid}{j}]\n\
    - parameters : \n\
    - dims       : 2 : [ 3, 2 ]\n\
    - values     :1.1, 1.2, 1.3, 2.1, 2.2, 2.3\n";

/*********************  CONSTS  *********************/
static const char TEST_CST_2[] = "//Definition of constant C\n\
static const double testC[3]={1 ,2 ,3 ,};\n";

/*********************  CONSTS  *********************/
static const char TEST_CST_3[] = "//Definition of constant C\n\
static const double testC[2][3]={{1 ,2 ,3 ,},{4 ,5 ,6 ,}};\n";

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testConstructor)
{
	ProjectConstant constant("C","testC","double");
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testLoadValue_scalar)
{
	ProjectConstant constant("C","testC","double");
	constant.loadValues("55",0);

	stringstream out;
	constant.printValues(out);

	EXPECT_EQ("55",out.str());
	EXPECT_FALSE(constant.haveCapture("\\cmr{cstid}{i}"));
	EXPECT_FALSE(constant.haveCapture("\\cmr{cstid}{j}"));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testLoadValue_scalar_invalid)
{
	ProjectConstant constant("C","testC","double");
	EXPECT_THROW(constant.loadValues("55;5",0),LatexException);
	EXPECT_THROW(constant.loadValues("55\\\\5",0),LatexException);
	EXPECT_THROW(constant.loadValues("",0),LatexException);
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testLoadValue_vector)
{
	ProjectConstant constant("C","testC","double");
	constant.loadValues("1 & 2 & 3 & 4 & 5 & 6",1);

	stringstream out;
	constant.printValues(out);

	EXPECT_EQ("1 2 3 4 5 6 ",out.str());
	EXPECT_TRUE(constant.haveCapture("\\cmr{cstid}{i}"));
	EXPECT_FALSE(constant.haveCapture("\\cmr{cstid}{j}"));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testLoadValue_vector_invalid)
{
	ProjectConstant constant("C","testC","double");
	EXPECT_THROW(constant.loadValues("55\\\\5",1),LatexException);
	EXPECT_THROW(constant.loadValues("",1),LatexException);
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testLoadValue_matrix)
{
	ProjectConstant constant("C","testC","double");
	constant.loadValues("1.1 & 1.2 & 1.3\\\\2.1 & 2.2 & 2.3\\\\3.1 & 3.2 & 3.3\\\\4.1 & 4.2 & 4.3\\\\5.1 & 5.2 & 5.3\\\\6.1 & 6.2 & 6.3",2);

	stringstream out;
	constant.printValues(out);

	EXPECT_EQ("1.1 1.2 1.3 \n2.1 2.2 2.3 \n3.1 3.2 3.3 \n4.1 4.2 4.3 \n5.1 5.2 5.3 \n6.1 6.2 6.3 \n",out.str());
	EXPECT_TRUE(constant.haveCapture("\\cmr{cstid}{i}"));
	EXPECT_TRUE(constant.haveCapture("\\cmr{cstid}{j}"));
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testLoadValue_scalar_matrix_invalid)
{
	ProjectConstant constant("C","testC","double");
	EXPECT_THROW(constant.loadValues("",2),LatexException);
	EXPECT_THROW(constant.loadValues("1.1 & 1.2 & 1.3\\\\2.1 & 2.2\\\\3.1 & 3.2 & 3.3",2),LatexException);
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testPrintDebug)
{
	ProjectConstant constant("C","testC","double");
	constant.loadValues("1.1 & 1.2 & 1.3\\\\2.1 & 2.2 & 2.3",2);
	
	stringstream out;
	constant.printDebug(out);
	
	EXPECT_EQ(TEST_CST_1,out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testGenDefinitionCCode_scalar)
{
	ProjectConstant constant("C","testC","double");
	constant.loadValues("5.5",0);
	
	ProjectContext context;
	stringstream out;
	constant.genDefinitionCCode(out,context);
	
	EXPECT_EQ("const double testC = 5.5 ;\n",out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testGenDefinitionCCode_vector)
{
	ProjectConstant constant("C","testC","double");
	constant.loadValues("1 & 2 & 3",1);
	
	ProjectContext context;
	stringstream out;
	constant.genDefinitionCCode(out,context);
	
	EXPECT_EQ(TEST_CST_2,out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testGenDefinitionCCode_matrix)
{
	ProjectConstant constant("C","testC","double");
	constant.loadValues("1 & 2 & 3\\\\4 & 5 & 6",2);
	
	ProjectContext context;
	stringstream out;
	constant.genDefinitionCCode(out,context);
	
	EXPECT_EQ(TEST_CST_3,out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testGenUsageCCode_scalar)
{
	ProjectConstant constant("C","testC","double");
	constant.loadValues("1",0);
	
	ProjectContext context;
	LatexEntity entity("C");
	stringstream out;
	constant.genUsageCCode(out,context,entity);
	
	EXPECT_EQ("testC",out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testGenUsageCCode_vector)
{
	ProjectConstant constant("C","testC","double");
	constant.loadValues("1 & 10",1);
	
	ProjectContext context;
	MockProjectEntity var("m","testM");
	context.addEntry(&var);
	LatexEntity entity("C_{m +5}");
	stringstream out;
	constant.genUsageCCode(out,context,entity);
	
	EXPECT_EQ("testC[ testM + 5 ]",out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectConstant,testGenUsageCCode_matrix)
{
	ProjectConstant constant("C","testC","double");
	constant.loadValues("1 & 10\\\\2 & 5",2);
	
	ProjectContext context;
	MockProjectEntity var("m","testM");
	context.addEntry(&var);
	LatexEntity entity("C_{m +5,m*2}");
	stringstream out;
	constant.genUsageCCode(out,context,entity);
	
	EXPECT_EQ("testC[ testM + 5 ][ testM * 2 ]",out.str());
}
