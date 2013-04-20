/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <svUnitTest.h>
#include <CMRProjectVariable.h>
#include <../parsor/CMRLatexFormula.h>
#include <sstream>
#include "MockProjectEntity.h"
#include <CMRProjectContext.h>

/**********************  USING  *********************/
using namespace svUnitTest;
using namespace std;

/*********************  CONSTS  *********************/
static const char TEST_CST_1[] = "			CMRCellAccessor<int[5],CMRMemoryModelRowMajor> testA;\n";
static const char TEST_CST_2[] = "	//define variable A_{i,j,k}\n\
	this->addVariable(\"testA\",sizeof(int[5]),1);\n";
static const char TEST_CST_3[] = "testA*(sys.getDomain(1,tstep)),x,y,absolute)\n";
static const char TEST_CST_4[] = "testA(acc.testA,x,y,absolute)\n";

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectVariable,testConstructor)
{
	CMRProjectVariable variable("A_{i,j}","testA","int");
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectVariable,testAddDim_1)
{
	CMRProjectVariable variable("A_{i,j,k}","testA","int");
	SVUT_ASSERT_EQUAL(2,variable.getCapturedIndices().size());
	variable.addDim("k",5);
	SVUT_ASSERT_EQUAL("i",variable.getCapturedIndices()[0]);
	SVUT_ASSERT_EQUAL("j",variable.getCapturedIndices()[1]);
	SVUT_ASSERT_EQUAL("k",variable.getCapturedIndices()[2]);
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectVariable,testAddDim_2)
{
	CMRProjectVariable variable("A_{i,j}","testA","int");
	SVUT_ASSERT_EQUAL(2,variable.getCapturedIndices().size());
	SVUT_ASSERT_THROW(CMRLatexException, variable.addDim("k",5));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectVariable,testGenUsageCCode_1)
{
	CMRProjectVariable variable("A_{i,j,k}","testA","int");
	variable.addDim("k",5);

	CMRLatexEntity2 entity("A_{2+1,2+1,55}");
	CMRProjectContext context;
	
	stringstream outr;
	variable.genUsageCCode(outr,context,entity);
	SVUT_ASSERT_EQUAL("in.testA( 2 + 1 , 2 + 1 )[ 55 ]",outr.str());

	stringstream outw;
	variable.genUsageCCode(outw,context,entity,true);
	SVUT_ASSERT_EQUAL("out.testA( 2 + 1 , 2 + 1 )[ 55 ]",outw.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectVariable,testGenUsageCCode_2)
{
	CMRProjectVariable variable("A_{i,j,k}","testA","int");
	variable.addDim("k",5);

	CMRLatexEntity2 entity("A_{m+1,m+1,55}");
	MockProjectEntity entity2("m","testM");
	CMRProjectContext context;
	context.addEntry(&entity2);

	stringstream outr;
	variable.genUsageCCode(outr,context,entity);
	SVUT_ASSERT_EQUAL("in.testA( testM + 1 , testM + 1 )[ 55 ]",outr.str());

	stringstream outw;
	variable.genUsageCCode(outw,context,entity,true);
	SVUT_ASSERT_EQUAL("out.testA( testM + 1 , testM + 1 )[ 55 ]",outw.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectVariable,testGenDefinitionCCode)
{
	CMRProjectVariable variable("A_{i,j,k}","testA","int");
	variable.addDim("k",5);
	
	CMRProjectContext context;
	
	stringstream out;
	SVUT_ASSERT_THROW(CMRLatexException,variable.genDefinitionCCode(out,context));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectVariable,testGetTypeWithDims_0)
{
	CMRProjectVariable variable("A_{i,j}","testA","int");;
	SVUT_ASSERT_EQUAL("int",variable.getTypeWithDims());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectVariable,testGetTypeWithDims_1)
{
	CMRProjectVariable variable("A_{i,j,k}","testA","int");
	variable.addDim("k",5);
	SVUT_ASSERT_EQUAL("int[5]",variable.getTypeWithDims());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectVariable,testGenCPPAccessorDefinition)
{
	CMRProjectVariable variable("A_{i,j,k}","testA","int");
	variable.addDim("k",5);
	
	stringstream out;
	variable.genCPPAccessorDefinition(out);
	SVUT_ASSERT_EQUAL(TEST_CST_1,out.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectVariable,testGenCPPAccessorAddVar)
{
	CMRProjectVariable variable("A_{i,j,k}","testA","int");
	variable.addDim("k",5);
	
	stringstream out;
	variable.genCPPAccessorAddVar(out);
	SVUT_ASSERT_EQUAL(TEST_CST_2,out.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectVariable,testGenCPPAccessorConstrSys)
{
	CMRProjectVariable variable("A_{i,j,k}","testA","int");
	variable.addDim("k",5);
	
	stringstream out;
	variable.genCPPAccessorConstrSys(out,1);
	SVUT_ASSERT_EQUAL(TEST_CST_3,out.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectVariable,testGenCPPAccessorConstrAcc)
{
	CMRProjectVariable variable("A_{i,j,k}","testA","int");
	variable.addDim("k",5);
	
	stringstream out;
	variable.genCPPAccessorConstrAcc(out);
	SVUT_ASSERT_EQUAL(TEST_CST_4,out.str());
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
