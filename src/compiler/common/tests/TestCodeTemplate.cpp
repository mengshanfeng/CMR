/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <sstream>
#include <gtest/gtest.h>
#include "CodeTemplate.h"
#include "CodeTemplateDic.h"

/**********************  USING  *********************/
using namespace std;
using namespace testing;
using namespace CMRCompiler;

/*******************  FUNCTION  *********************/
TEST(TestCodeTemplate,testConstructor1)
{
	CodeTemplate code("constant string, no variables.");
	EXPECT_EQ("",code.strRequiredVars());
}

/*******************  FUNCTION  *********************/
TEST(TestCodeTemplate,testConstructor2)
{
	CodeTemplate code("real string, with variables : @test@.");
	EXPECT_EQ("test",code.strRequiredVars());
}

/*******************  FUNCTION  *********************/
TEST(TestCodeTemplate,testConstructor3)
{
	CodeTemplate code("real string, with variables : @test1@, and escapte one \\@test2");
}

/*******************  FUNCTION  *********************/
TEST(TestCodeTemplate, testApplyOn_1)
{
	CodeTemplate code("real string, with variables : @test1@, @test2@ and escapte one \\@test3");
	CodeTemplateValueDic values;
	values.set("test1","value1");
	values.set("test2","value2");
	values.set("test3","value3");
	
	stringstream buffer;
	code.applyOn(buffer,values);
	
	EXPECT_EQ("real string, with variables : value1, value2 and escapte one \\@test3",buffer.str());
}

/*******************  FUNCTION  *********************/
TEST(TestCodeTemplate, testApplyOn_2)
{
	CodeTemplate code("real string, with variables : @test1@, @test2@ and escapte one \\@test3");
	CodeTemplateValueDic values;
	values.set("test1","value1");
	values.set("test2","value2");
	values.set("test3","value3");
	
	stringstream buffer;
	code.applyOn(buffer,values,2);
	
	EXPECT_EQ("\t\treal string, with variables : value1, value2 and escapte one \\@test3",buffer.str());
}

/*******************  FUNCTION  *********************/
TEST(TestCodeTemplate, testApplyOn_3)
{
	CodeTemplate code("for {\n\tif(@test1@ == @test2@)\n\t\tabort();\n\t}\n");
	CodeTemplateValueDic values;
	values.set("test1","value1");
	values.set("test2","value2");
	values.set("test3","value3");
	
	stringstream buffer;
	code.applyOn(buffer,values,2);
	
	EXPECT_EQ("\t\tfor {\n\t\t\tif(value1 == value2)\n\t\t\t\tabort();\n\t\t\t}\n",buffer.str());
}

/*******************  FUNCTION  *********************/
TEST(TestCodeTemplate, testApplyOn_4)
{
	CodeTemplate code("for {\n\tif(@test1@ == @test2@)\n\t\t@test3@\n\t}\n");
	CodeTemplateValueDic values;
	values.set("test1","value1");
	values.set("test2","value2");
	values.set("test3","abort();\nabort();");
	
	stringstream buffer;
	code.applyOn(buffer,values,2);
	
	EXPECT_EQ("\t\tfor {\n\t\t\tif(value1 == value2)\n\t\t\t\tabort();\n\t\t\t\tabort();\n\t\t\t}\n",buffer.str());
}

