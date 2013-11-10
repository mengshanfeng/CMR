/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <CMRProjectMeshVar.h>
#include <../parsor/CMRLatexFormula.h>
#include <sstream>
#include "MockProjectEntity.h"
#include <CMRProjectContext.h>

/**********************  USING  *********************/
using namespace testing;
using namespace std;

/*********************  CONSTS  *********************/
static const char TEST_CST_1[] = "			CMRCellAccessor<int[5],CMRMemoryModelRowMajor> testA;\n";
static const char TEST_CST_2[] = "	//define variable A_{i,j,k}\n\
	this->addVariable(\"testA\",sizeof(int[5]),1);\n";
static const char TEST_CST_3[] = "testA*(sys.getDomain(1,tstep)),x,y,absolute)\n";
static const char TEST_CST_4[] = "testA(acc.testA,x,y,absolute)\n";

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testConstructor)
{
	CMRProjectMeshVar variable("A_{i,j}","testA","int");
}

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testAddDim_1)
{
	CMRProjectMeshVar variable("A_{i,j,k}","testA","int");
	EXPECT_EQ(2,variable.getCapturedIndices().size());
	variable.addDim("k",5);
	EXPECT_EQ("i",variable.getCapturedIndices()[0]);
	EXPECT_EQ("j",variable.getCapturedIndices()[1]);
	EXPECT_EQ("k",variable.getCapturedIndices()[2]);
}

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testAddDim_2)
{
	CMRProjectMeshVar variable("A_{i,j}","testA","int");
	EXPECT_EQ(2,variable.getCapturedIndices().size());
	EXPECT_THROW(variable.addDim("k",5),CMRLatexException);
}

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testGenUsageCCode_1)
{
	CMRProjectMeshVar variable("A_{i,j,k}","testA","int");
	variable.addDim("k",5);

	CMRLatexEntity2 entity("A_{2+1,2+1,55}");
	CMRProjectContext context;
	
	stringstream outr;
	variable.genUsageCCode(outr,context,entity);
	EXPECT_EQ("in.testA( 2 + 1 , 2 + 1 )[ 55 ]",outr.str());

	stringstream outw;
	variable.genUsageCCode(outw,context,entity,true);
	EXPECT_EQ("out.testA( 2 + 1 , 2 + 1 )[ 55 ]",outw.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testGenUsageCCode_2)
{
	CMRProjectMeshVar variable("A_{i,j,k}","testA","int");
	variable.addDim("k",5);

	CMRLatexEntity2 entity("A_{m+1,m+1,55}");
	MockProjectEntity entity2("m","testM");
	CMRProjectContext context;
	context.addEntry(&entity2);

	stringstream outr;
	variable.genUsageCCode(outr,context,entity);
	EXPECT_EQ("in.testA( testM + 1 , testM + 1 )[ 55 ]",outr.str());

	stringstream outw;
	variable.genUsageCCode(outw,context,entity,true);
	EXPECT_EQ("out.testA( testM + 1 , testM + 1 )[ 55 ]",outw.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testGenDefinitionCCode)
{
	CMRProjectMeshVar variable("A_{i,j,k}","testA","int");
	variable.addDim("k",5);
	
	CMRProjectContext context;
	
	stringstream out;
	EXPECT_THROW(variable.genDefinitionCCode(out,context),CMRLatexException);
}

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testGetTypeWithDims_0)
{
	CMRProjectMeshVar variable("A_{i,j}","testA","int");;
	EXPECT_EQ("int",variable.getTypeWithDims());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testGetTypeWithDims_1)
{
	CMRProjectMeshVar variable("A_{i,j,k}","testA","int");
	variable.addDim("k",5);
	EXPECT_EQ("int[5]",variable.getTypeWithDims());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testGenCPPAccessorDefinition)
{
	CMRProjectMeshVar variable("A_{i,j,k}","testA","int");
	variable.addDim("k",5);
	
	stringstream out;
	variable.genCPPAccessorDefinition(out);
	EXPECT_EQ(TEST_CST_1,out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testGenCPPAccessorAddVar)
{
	CMRProjectMeshVar variable("A_{i,j,k}","testA","int");
	variable.addDim("k",5);
	
	stringstream out;
	variable.genCPPAccessorAddVar(out);
	EXPECT_EQ(TEST_CST_2,out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testGenCPPAccessorConstrSys)
{
	CMRProjectMeshVar variable("A_{i,j,k}","testA","int");
	variable.addDim("k",5);
	
	stringstream out;
	variable.genCPPAccessorConstrSys(out,1);
	EXPECT_EQ(TEST_CST_3,out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectMeshVar,testGenCPPAccessorConstrAcc)
{
	CMRProjectMeshVar variable("A_{i,j,k}","testA","int");
	variable.addDim("k",5);
	
	stringstream out;
	variable.genCPPAccessorConstrAcc(out);
	EXPECT_EQ(TEST_CST_4,out.str());
}
