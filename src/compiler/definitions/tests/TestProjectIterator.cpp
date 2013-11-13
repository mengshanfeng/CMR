/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include "ProjectIterator.h"
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
    - latexName  : k\n\
    - shortName  : k\n\
    - longName   : testK\n\
    - exponent   : \n\
    - indices    : \n\
    - parameters : \n\
    - values     : 0 .. 9\n";

/*******************  FUNCTION  *********************/
TEST(TestProjectIterator,testConstructor)
{
	ProjectIterator it("k","testK",0,9);
}

/*******************  FUNCTION  *********************/
TEST(TestProjectIterator,testPrintDebug)
{
	ProjectIterator it("k","testK",0,9);
	
	stringstream out;
	it.printDebug(out);
	
	EXPECT_EQ(TEST_CST_1,out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectIterator,testGenDefinitionCCode)
{
	ProjectIterator it("k","testK",0,9);
	ProjectContext context;
	
	stringstream out;
	it.genDefinitionCCode(out,context);
	
	EXPECT_EQ("int testK = 0 ; testK <= 9 ; testK++",out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectIterator,testGenUsageCCode)
{
	ProjectIterator it("k","testK",0,9);
	ProjectContext context;
	LatexEntity entity("k");
	
	stringstream out;
	EXPECT_THROW(it.genUsageCCode(out,context,entity),LatexException);
}
