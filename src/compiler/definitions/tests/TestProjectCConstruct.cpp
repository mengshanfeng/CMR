/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include "CMRProjectCode.h"
#include "ProjectMeshVar.h"

/**********************  USING  *********************/
using namespace testing;
using namespace std;
using namespace CMRCompiler;

/*******************  FUNCTION  *********************/
TEST(TestCConstruct,testConstructor)
{
	CMRProjectCConstruct construct("abort()");
}

/*******************  FUNCTION  *********************/
TEST(TestCConstruct,testSimpleGenCode)
{
	//setup construct
	CMRProjectContext context;
	CMRProjectCConstruct construct("abort();");
	
	//gen
	stringstream out;
	construct.genCCode(out,context);
	
	//check
	EXPECT_EQ("abort();",out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestCConstruct,testUserArgs1)
{
	//setup context
	CMRProjectContext context;
	ProjectMeshVar var("v_{i,j}","var","double");
	context.addEntry(new CMRProjectLocalVariable("i","x","int"));
	context.addEntry(new CMRProjectLocalVariable("j","y","int"));
	context.addEntry(&var);

	//setup construct
	CMRProjectCConstruct construct("pow(%1,2);");
	construct.arg("v_{i,j}");

	//gen
	stringstream out;
	construct.genCCode(out,context);
	
	//check
	EXPECT_EQ("pow(in.var( x , y ) ,2);",out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestCConstruct,testUserArgs2)
{
	//setup context
	CMRProjectContext context;
	ProjectMeshVar var("v_{i,j}","var","double");
	context.addEntry(new CMRProjectLocalVariable("i","x","int"));
	context.addEntry(new CMRProjectLocalVariable("j","y","int"));
	context.addEntry(&var);

	//setup construct
	CMRProjectCConstruct construct("pow(%1,%2);");
	construct.arg("v_{i,j}");
	construct.arg("2");

	//gen
	stringstream out;
	construct.genCCode(out,context);
	
	//check
	EXPECT_EQ("pow(in.var( x , y ) ,2 );",out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestCConstruct,testUserArgs3)
{
	//setup context
	CMRProjectContext context;
	ProjectMeshVar var("v_{i,j}","var","double");
	context.addEntry(new CMRProjectLocalVariable("i","x","int"));
	context.addEntry(new CMRProjectLocalVariable("j","y","int"));
	context.addEntry(&var);

	//setup construct
	CMRProjectCConstruct construct("pow(%2,%1);");
	construct.arg("v_{i,j}");
	construct.arg("2");

	//gen
	stringstream out;
	construct.genCCode(out,context);
	
	//check
	EXPECT_EQ("pow(2 ,in.var( x , y ) );",out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestCConstruct,testAutoArgs1)
{
	//setup context
	CMRProjectContext context;
	ProjectMeshVar var("v_{i,j}","var","double");
	context.addEntry(new CMRProjectLocalVariable("i","x","int"));
	context.addEntry(new CMRProjectLocalVariable("j","y","int"));
	context.addEntry(&var);

	//setup construct
	CMRProjectCConstruct construct("pow($v_{i,j}$,$v_{i+1,j+1}$);");

	//gen
	stringstream out;
	construct.genCCode(out,context);
	
	//check
	EXPECT_EQ("pow(in.var( x , y ) ,in.var( x + 1 , y + 1 ) );",out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestCConstruct,testMixArgs)
{
	//setup context
	CMRProjectContext context;
	ProjectMeshVar var("v_{i,j}","var","double");
	context.addEntry(new CMRProjectLocalVariable("i","x","int"));
	context.addEntry(new CMRProjectLocalVariable("j","y","int"));
	context.addEntry(&var);

	//setup construct
	CMRProjectCConstruct construct("pow($v_{i,j}$,%1);");
	construct.arg("v_{i+1,j+1}");

	//gen
	stringstream out;
	construct.genCCode(out,context);
	
	//check
	EXPECT_EQ("pow(in.var( x , y ) ,in.var( x + 1 , y + 1 ) );",out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestCConstruct,testEscape)
{
	//setup context
	CMRProjectContext context;

	//setup construct
	CMRProjectCConstruct construct("pow(\\$1,\\%1,\\%a1);");

	//gen
	stringstream out;
	construct.genCCode(out,context);
	
	//check
	EXPECT_EQ("pow(\\$1,\\%1,\\%a1);",out.str());
}
