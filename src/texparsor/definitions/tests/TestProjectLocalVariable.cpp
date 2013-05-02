/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <svUnitTest.h>
#include <CMRProjectCode.h>
#include <../parsor/CMRLatexFormula.h>
#include <sstream>
#include "MockProjectEntity.h"

/**********************  USING  *********************/
using namespace svUnitTest;
using namespace std;

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectLocalVariable,testConstructor)
{
	CMRProjectLocalVariable var("A_i","toto","int","1");
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectLocalVariable,testGenUsageCCode)
{
	CMRProjectLocalVariable var("A_i","toto","int","1");
	
	CMRProjectContext context;
	CMRLatexEntity2 entity("A_i");
	
	stringstream out1;
	var.genUsageCCode(out1,context,entity,true);
	SVUT_ASSERT_EQUAL("toto",out1.str());
	
	stringstream out2;
	var.genUsageCCode(out2,context,entity,false);
	SVUT_ASSERT_EQUAL("toto",out2.str());
	
	stringstream out3;
	var.genUsageCCode(out3,context,entity);
	SVUT_ASSERT_EQUAL("toto",out3.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectLocalVariable,testGenDefinitionCCode)
{
	CMRProjectLocalVariable var("A_i","toto","int","1");
	
	CMRProjectContext context;
	CMRLatexEntity2 entity("A_i");
	
	stringstream out1;
	var.genDefinitionCCode(out1,context);
	SVUT_ASSERT_EQUAL("int toto = 1 ;\n",out1.str());
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
