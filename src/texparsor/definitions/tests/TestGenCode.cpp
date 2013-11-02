/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <CMRProjectContext.h>
#include "../../parsor/CMRLatexFormula.h"
#include <sstream>
#include "MockProjectEntity.h"
#include "../CMRGenCode.h"

/**********************  USING  *********************/
using namespace testing;
using namespace std;

/*******************  FUNCTION  *********************/
TEST(TestGenCode,testCmrGenEqCCode_entity_1)
{
	MockProjectEntity entity1("A_i","testAi");
	MockProjectEntity entity2("A_{i,j}","testAij");
	MockProjectEntity entity3("B_i","testB");
	entity3.changeCaptureType("i",CMR_CAPTURE_REQUIRED);
	CMRProjectContext context;
	context.addEntry(&entity1);
	context.addEntry(&entity2);
	context.addEntry(&entity3);
	
	stringstream out;
	
	CMRLatexEntity2 e("A_i");
	cmrGenEqCCode(out,context,e);
	EXPECT_EQ("testAi ",out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestGenCode,testCmrGenEqCCode_entity_2)
{
	MockProjectEntity entity1("A_i","testAi");
	MockProjectEntity entity2("A_{i,j}","testAij");
	MockProjectEntity entity3("B_i","testB");
	entity3.changeCaptureType("i",CMR_CAPTURE_REQUIRED);
	CMRProjectContext context;
	context.addEntry(&entity1);
	context.addEntry(&entity2);
	context.addEntry(&entity3);
	
	stringstream out;
	
	CMRLatexEntity2 e("155");
	cmrGenEqCCode(out,context,e);
	EXPECT_EQ("155 ",out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestGenCode,testCmrGenEqCCode_entity_3)
{
	MockProjectEntity entity1("A_i","testAi");
	MockProjectEntity entity2("A_{i, j}","testAij");
	MockProjectEntity entity3("B_i","testB");
	entity3.changeCaptureType("i",CMR_CAPTURE_REQUIRED);
	CMRProjectContext context;
	context.addEntry(&entity1);
	context.addEntry(&entity2);
	context.addEntry(&entity3);
	
	stringstream out;
	
	CMRLatexEntity2 e("A_{ i , j }");
	cmrGenEqCCode(out,context,e);
	EXPECT_EQ("testAij ",out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestGenCode,testCmrGenEqCCode_entity_4)
{
	MockProjectEntity entity1("A_i","testAi");
	MockProjectEntity entity2("A_{i, j}","testAij");
	MockProjectEntity entity3("B_i","testB");
	entity3.changeCaptureType("i",CMR_CAPTURE_REQUIRED);
	CMRProjectContext context;
	context.addEntry(&entity1);
	context.addEntry(&entity2);
	context.addEntry(&entity3);
	
	stringstream out;
	
	CMRLatexEntity2 e("B_k");
	cmrGenEqCCode(out,context,e);
	EXPECT_EQ("testB ",out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestGenCode,testCmrGenEqCCode_entity_5)
{
	MockProjectEntity entity1("A_i","testAi");
	MockProjectEntity entity2("A_{i, j}","testAij");
	MockProjectEntity entity3("B_i","testB");
	entity3.changeCaptureType("i",CMR_CAPTURE_REQUIRED);
	CMRProjectContext context;
	context.addEntry(&entity1);
	context.addEntry(&entity2);
	context.addEntry(&entity3);
	
	stringstream out;
	
	CMRLatexEntity2 e("Z_k");
	EXPECT_THROW(cmrGenEqCCode(out,context,e),CMRLatexException);
}

/*******************  FUNCTION  *********************/
TEST(TestGenCode,testCmrGenEqCCode_entity_6)
{
	MockProjectEntity entity1("A_i","testAi");
	MockProjectEntity entity2("A_{i, j}","testAij");
	MockProjectEntity entity3("B_i","testB");
	entity3.changeCaptureType("i",CMR_CAPTURE_REQUIRED);
	CMRProjectContext context;
	context.addEntry(&entity1);
	context.addEntry(&entity2);
	context.addEntry(&entity3);
	
	stringstream out;
	
	CMRLatexEntity2 e("B");
	EXPECT_THROW(cmrGenEqCCode(out,context,e),CMRLatexException);
}

/*******************  FUNCTION  *********************/
TEST(TestGenCode,testCmrGenEqCCode_formula)
{
	MockProjectEntity entity1("A_i","testAi");
	MockProjectEntity entity2("A_{i,j}","testAij");
	MockProjectEntity entity3("B_i","testB");
	entity3.changeCaptureType("i",CMR_CAPTURE_REQUIRED);
	CMRProjectContext context;
	context.addEntry(&entity1);
	context.addEntry(&entity2);
	context.addEntry(&entity3);
	
	stringstream out;
	
	CMRLatexFormulas2 e("A_i + A_{i,j} + B_{A_i * 8} + (55 + A_i)");
	cmrGenEqCCode(out,context,e);
	EXPECT_EQ("testAi + testAij + testB + ( 55 + testAi ) ",out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestGenCode,testCmrLatexEntityIsInteger)
{
	CMRLatexEntity2 entity1("155");
	CMRLatexEntity2 entity2("1");
	CMRLatexEntity2 entity3("A");
	CMRLatexEntity2 entity4("15.3");
	CMRLatexEntity2 entity5("1_2");
	CMRLatexEntity2 entity6("1^2");
	
	EXPECT_TRUE(latexEntityIsInteger(entity1));
	EXPECT_TRUE(latexEntityIsInteger(entity2));
	EXPECT_FALSE(latexEntityIsInteger(entity3));
	EXPECT_FALSE(latexEntityIsInteger(entity4));
	EXPECT_FALSE(latexEntityIsInteger(entity5));
	EXPECT_FALSE(latexEntityIsInteger(entity6));
}
