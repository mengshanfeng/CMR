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
#include <CMRProjectIterator.h>
#include <CMRProjectDefinition.h>

/**********************  USING  *********************/
using namespace svUnitTest;
using namespace std;

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectDefinition,testConstructor)
{
	CMRProjectDefinition def("E","energy");
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectDefinition,testAddBasicActions)
{
	CMRProjectDefinition def("E","energy");
	def.addEquation("E","4*5");
	def.addIteratorLoop("k");
	def.addLocalVariable("t","tmp","int","0");
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectDefinition,testPrintDebug)
{
	CMRProjectDefinition def("E","energy");
	def.addLocalVariable("t","tmp","int","0");
	def.addEquation("E","4*5");
	def.addIteratorLoop("k").addEquation("t","3*k","+=");
	def.addEquation("E","t*3","+=");
	
	stringstream out;
	def.printDebug(out);
	
	SVUT_ASSERT_EQUAL("",out.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectDefinition,testGenDefinitionCCode)
{
	SVUT_ASSERT_TODO("todo");
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectDefinition,testGenUsageCCode)
{
	SVUT_ASSERT_TODO("todo");
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectDefinition,testRunTransformation)
{
	SVUT_ASSERT_TODO("todo");
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
