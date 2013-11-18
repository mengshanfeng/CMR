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
#include "ProjectIterator.h"
#include "ProjectDefinition.h"

/**********************  USING  *********************/
using namespace testing;
using namespace std;
using namespace CMRCompiler;

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
static const char * CST_VALUE_3 = "//Definition : E_{i,j,h} : energy\n\
double compute_energy(const VarSystem::CellAccessor & in,VarSystem::CellAccessor & out,int x,int y, int param_0_0)\n\
{\n\
	double result = 0 ;\n\
	int tmp = 0 ;\n\
	result = 4 * 5 + param_0_0 ;\n\
	for(int k = 1 ; k <= 10 ; k++ )\n\
	{\n\
		tmp += 3 * k ;\n\
	}\n\
	result += tmp * 3 ;\n\
	return result;\n\
}\n\n";

/*******************  FUNCTION  *********************/
TEST(TestProjectDefinition,testConstructor)
{
	ProjectDefinition def("E","energy");
}

/*******************  FUNCTION  *********************/
TEST(TestProjectDefinition,testAddBasicActions)
{
	ProjectDefinition def("E","energy");
	def.addEquation("E","4*5");
	def.addIteratorLoop("k");
	def.addLocalVariable("t","tmp","int","0");
}

/*******************  FUNCTION  *********************/
TEST(TestProjectDefinition,testPrintDebug)
{
	FAIL();
	ProjectDefinition def("E","energy");
	def.addLocalVariable("t","tmp","int","0");
	def.addEquation("E","4*5");
	def.addIteratorLoop("k").addEquation("t","3*k","+=");
	def.addEquation("E","t*3","+=");
	
	stringstream out;
	def.printDebug(out);
	
	EXPECT_EQ("",out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectDefinition,testGenDefinitionCCode)
{
	ProjectDefinition def("E","energy");
	def.addIterator("k","k",1,10);
	def.addLocalVariable("t","tmp","int","0");
	def.addEquation("E","4*5");
	def.addIteratorLoop("k").addEquation("t","3*k","+=");
	def.addEquation("E","t*3","+=");
	ProjectContext context;
	
	stringstream out;
	def.genDefinitionCCode(out,&context,0);
	
	EXPECT_EQ(CST_VALUE_1,out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectDefinition,testGenDefinitionCCodeIndent)
{
	ProjectDefinition def("E","energy");
	def.addIterator("k","k",1,10);
	def.addLocalVariable("t","tmp","int","0");
	def.addEquation("E","4*5");
	def.addIteratorLoop("k").addEquation("t","3*k","+=");
	def.addEquation("E","t*3","+=");
	ProjectContext context;
	
	stringstream out;
	def.genDefinitionCCode(out,&context,1);
	
	EXPECT_EQ(CST_VALUE_2,out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectDefinition,testGenDefinitionCCodeParams)
{
	ProjectDefinition def("E_{i,j,h}","energy");
	def.changeCaptureType("h",CAPTURE_REQUIRED);
	def.addIterator("k","k",1,10);
	def.addLocalVariable("t","tmp","int","0");
	def.addEquation("E_{i,j,h}","4*5+h");
	def.addIteratorLoop("k").addEquation("t","3*k","+=");
	def.addEquation("E_{i,j,h}","t*3","+=");
	ProjectContext context;
	
	stringstream out;
	def.genDefinitionCCode(out,&context,0);
	
	EXPECT_EQ(CST_VALUE_3,out.str());
}


/*******************  FUNCTION  *********************/
TEST(TestProjectDefinition,testGenUsageCCodeRead)
{
	ProjectDefinition def("E","energy");
	def.addIterator("k","k",1,10);
	def.addLocalVariable("t","tmp","int","0");
	def.addEquation("E","4*5");
	def.addIteratorLoop("k").addEquation("t","3*k","+=");
	def.addEquation("E","t*3","+=");
	ProjectContext context;
	
	stringstream out;
	LatexEntity entity("E");
	def.genUsageCCode(out,&context,entity);
	
	EXPECT_EQ("compute_energy(in,out,x,y)",out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectDefinition,testGenUsageCCodeParamsRead)
{
	ProjectDefinition def("E_{i,j,h}","energy");
	def.changeCaptureType("h",CAPTURE_REQUIRED);
	def.addIterator("k","k",1,10);
	def.addLocalVariable("t","tmp","int","0");
	def.addEquation("E_{i,j,h}","4*5+h");
	def.addIteratorLoop("k").addEquation("t","3*k","+=");
	def.addEquation("E_{i,j,h}","t*3","+=");
	ProjectContext context;
	
	stringstream out;
	LatexEntity entity("E_{i,j,3}");
	def.genUsageCCode(out,&context,entity);
	
	EXPECT_EQ("compute_energy(in,out,x,y, (3))",out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectDefinition,testVectorParameter)
{
	ProjectDefinition def("E_{i,j,h}","energy");
	def.setCaptureSize("h","double",4);
	def.changeCaptureType("h",CAPTURE_REQUIRED);
	def.addEquation("E_{i,j,h} = h_* + h_4");
	ProjectContext context;
	
	stringstream out;
	def.genDefinitionCCode(out,&context);
	
	EXPECT_EQ("",out.str());
}
