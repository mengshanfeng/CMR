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
#include <sstream>
#include <ProjectAction.h>
#include <../common/LangDef.h>

/**********************  USING  *********************/
using namespace testing;
using namespace std;
using namespace CMRCompiler;

/*********************  CONSTS  *********************/
static const char * CST_VALUE_1 = "//blablablalbl\n\
struct Actionupdate_enery\n\
{\n\
	void cellAction(const VarSystem::CellAccessor & in,VarSystem::CellAccessor& out,int x,int y) const\n\
	{\n\
		int tmp = 0 ;\n\
		double enegry = 0 ;\n\
		enegry = 4 * 5 ;\n\
		for(int k = 1 ; k <= 10 ; k++ )\n\
		{\n\
			tmp += 3 * k ;\n\
		}\n\
		enegry += tmp * 3 ;\n\n\
	}\n\n\
	typedef CMRMeshOperationSimpleLoop<VarSystem,Actionupdate_enery> LoopType;\n\
};\n\n";

/*******************  FUNCTION  *********************/
TEST(TestProjectAction,testConstructor)
{
	ProjectAction action("update_enery","blablablalbl","CMRMeshOperationSimpleLoop");
}

/*******************  FUNCTION  *********************/
TEST(TestProjectAction,testAddBasicActions)
{
	ProjectAction action("update_enery","blablablalbl","CMRMeshOperationSimpleLoop");
	action.addEquation("E","4*5");
	action.addIteratorLoop("k");
	action.addLocalVariable("t","tmp","int","0");
}

// /*******************  FUNCTION  *********************/
// TEST(TestProjectAction,testPrintDebug)
// {
// 	FAIL();
// 	CMRProjectDefinition def("E","energy");
// 	def.addLocalVariable("t","tmp","int","0");
// 	def.addEquation("E","4*5");
// 	def.addIteratorLoop("k").addEquation("t","3*k","+=");
// 	def.addEquation("E","t*3","+=");
// 	
// 	stringstream out;
// 	def.printDebug(out);
// 	
// 	EXPECT_EQ("",out.str());
// }

/*******************  FUNCTION  *********************/
TEST(TestProjectAction,testGenDefinitionCCode)
{
	LangDef def;
	ProjectAction action("update_enery","blablablalbl","CMRMeshOperationSimpleLoop");
	action.addLocalVariable("E","enegry","double","0");
	action.addIterator("k","k",1,10);
	action.addLocalVariable("t","tmp","int","0");
	action.addEquation("E","4*5");
	action.addIteratorLoop("k").addEquation("t","3*k","+=");
	action.addEquation("E","t*3","+=");
	ProjectContext context;
	
	stringstream out;
	action.genDefinitionCCode(out,def,&context,0);
	
	EXPECT_EQ(CST_VALUE_1,out.str());
}
// 
// /*******************  FUNCTION  *********************/
// TEST(TestProjectAction,testGenDefinitionCCodeIndent)
// {
// 	CMRProjectDefinition def("E","energy");
// 	def.addIterator("k","k",1,10);
// 	def.addLocalVariable("t","tmp","int","0");
// 	def.addEquation("E","4*5");
// 	def.addIteratorLoop("k").addEquation("t","3*k","+=");
// 	def.addEquation("E","t*3","+=");
// 	CMRProjectContext context;
// 	
// 	stringstream out;
// 	def.genDefinitionCCode(out,&context,1);
// 	
// 	EXPECT_EQ(CST_VALUE_2,out.str());
// }
// 
// /*******************  FUNCTION  *********************/
// TEST(TestProjectAction,testGenDefinitionCCodeParams)
// {
// 	CMRProjectDefinition def("E_{i,j,h}","energy");
// 	def.changeCaptureType("h",CMR_CAPTURE_REQUIRED);
// 	def.addIterator("k","k",1,10);
// 	def.addLocalVariable("t","tmp","int","0");
// 	def.addEquation("E_{i,j,h}","4*5+h");
// 	def.addIteratorLoop("k").addEquation("t","3*k","+=");
// 	def.addEquation("E_{i,j,h}","t*3","+=");
// 	CMRProjectContext context;
// 	
// 	stringstream out;
// 	def.genDefinitionCCode(out,&context,0);
// 	
// 	EXPECT_EQ(CST_VALUE_3,out.str());
// }
// 
// 
// /*******************  FUNCTION  *********************/
// TEST(TestProjectAction,testGenUsageCCodeRead)
// {
// 	CMRProjectDefinition def("E","energy");
// 	def.addIterator("k","k",1,10);
// 	def.addLocalVariable("t","tmp","int","0");
// 	def.addEquation("E","4*5");
// 	def.addIteratorLoop("k").addEquation("t","3*k","+=");
// 	def.addEquation("E","t*3","+=");
// 	CMRProjectContext context;
// 	
// 	stringstream out;
// 	CMRLatexEntity2 entity("E");
// 	def.genUsageCCode(out,&context,entity);
// 	
// 	EXPECT_EQ("compute_energy(in,out,x,y)",out.str());
// }
// 
// /*******************  FUNCTION  *********************/
// TEST(TestProjectAction,testGenUsageCCodeParamsRead)
// {
// 	CMRProjectDefinition def("E_{i,j,h}","energy");
// 	def.changeCaptureType("h",CMR_CAPTURE_REQUIRED);
// 	def.addIterator("k","k",1,10);
// 	def.addLocalVariable("t","tmp","int","0");
// 	def.addEquation("E_{i,j,h}","4*5+h");
// 	def.addIteratorLoop("k").addEquation("t","3*k","+=");
// 	def.addEquation("E_{i,j,h}","t*3","+=");
// 	CMRProjectContext context;
// 	
// 	stringstream out;
// 	CMRLatexEntity2 entity("E_{i,j,3}");
// 	def.genUsageCCode(out,&context,entity);
// 	
// 	EXPECT_EQ("compute_energy(in,out,x,y, (3))",out.str());
// }
