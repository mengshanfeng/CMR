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

/*********************  CONSTS  *********************/
static const char * CST_VALUE_1 = "//Definition : E : energy\n\
double compute_energy(const VarSystem::CellAccessor & in,VarSystem::CellAccessor & out,int x,int y)\n\
{\n\
	double result = 0 ;\n\
	int tmp = 0 ;\n\
	result = 4 * 5 ;\n\
	for(int k = 1 ; k <= 10 ; k++ )\n\
	{\n\
		tmp += 3 * k ;\n\
	}\n\
	result += tmp * 3 ;\n\
	return result;\n\
}\n\n";
static const char * CST_VALUE_2 = "	//Definition : E : energy\n\
	double compute_energy(const VarSystem::CellAccessor & in,VarSystem::CellAccessor & out,int x,int y)\n\
	{\n\
		double result = 0 ;\n\
		int tmp = 0 ;\n\
		result = 4 * 5 ;\n\
		for(int k = 1 ; k <= 10 ; k++ )\n\
		{\n\
			tmp += 3 * k ;\n\
		}\n\
		result += tmp * 3 ;\n\
		return result;\n\
	}\n\n";

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
	SVUT_ASSERT_TODO("todo");
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
	CMRProjectDefinition def("E","energy");
	def.addIterator("k","k",1,10);
	def.addLocalVariable("t","tmp","int","0");
	def.addEquation("E","4*5");
	def.addIteratorLoop("k").addEquation("t","3*k","+=");
	def.addEquation("E","t*3","+=");
	CMRProjectContext context;
	
	stringstream out;
	def.genDefinitionCCode(out,&context,0);
	
	SVUT_ASSERT_EQUAL(CST_VALUE_1,out.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectDefinition,testGenDefinitionCCodeIndent)
{
	CMRProjectDefinition def("E","energy");
	def.addIterator("k","k",1,10);
	def.addLocalVariable("t","tmp","int","0");
	def.addEquation("E","4*5");
	def.addIteratorLoop("k").addEquation("t","3*k","+=");
	def.addEquation("E","t*3","+=");
	CMRProjectContext context;
	
	stringstream out;
	def.genDefinitionCCode(out,&context,1);
	
	SVUT_ASSERT_EQUAL(CST_VALUE_2,out.str());
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
