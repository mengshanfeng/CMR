/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <CMRProjectCode.h>
#include <../parsor/LatexFormula.h>
#include <sstream>
#include "MockProjectEntity.h"

/**********************  USING  *********************/
using namespace testing;
using namespace std;
using namespace CMRCompiler;

/*******************  FUNCTION  *********************/
TEST(TestProjectLocalVariable,testConstructor)
{
	CMRProjectLocalVariable var("A_i","toto","int","1");
}

/*******************  FUNCTION  *********************/
TEST(TestProjectLocalVariable,testGenUsageCCode)
{
	CMRProjectLocalVariable var("A_i","toto","int","1");
	
	ProjectContext context;
	LatexEntity entity("A_i");
	
	stringstream out1;
	var.genUsageCCode(out1,context,entity,true);
	EXPECT_EQ("toto",out1.str());
	
	stringstream out2;
	var.genUsageCCode(out2,context,entity,false);
	EXPECT_EQ("toto",out2.str());
	
	stringstream out3;
	var.genUsageCCode(out3,context,entity);
	EXPECT_EQ("toto",out3.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectLocalVariable,testGenDefinitionCCode)
{
	CMRProjectLocalVariable var("A_i","toto","int","1");
	
	ProjectContext context;
	LatexEntity entity("A_i");
	
	stringstream out1;
	var.genDefinitionCCode(out1,context);
	EXPECT_EQ("int toto = 1 ;\n",out1.str());
}
