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
#include <LatexEntity.h>
#include <LatexParsorContext.h>

/**********************  USING  *********************/
using namespace std;
using namespace testing;
using namespace CMRCompiler;

/*********************  CONSTS  *********************/
static const char TEST_CST_1[] = "<entity>\n\
  <name>A</name>\n\
  <indices>\n\
    <formula>\n\
      <entity>\n\
        <name>2</name>\n\
      </entity>\n\
    </formula>\n\
  </indices>\n\
  <exponents>\n\
    <formula>\n\
      <entity>\n\
        <name>3</name>\n\
      </entity>\n\
    </formula>\n\
  </exponents>\n\
</entity>\n\
";
static const char TEST_CST_2[] = "entity: A\n\
  indices: 2\n\
  exponents: 3\n\
";

/*******************  FUNCTION  *********************/
TEST(TestLatexEntity,testConstructor1)
{
	LatexEntity entity;
	EXPECT_EQ("",entity.getName());
	EXPECT_EQ(0,entity.countExponents());
	EXPECT_EQ(0,entity.countIndices());
	EXPECT_EQ(0,entity.countParameters());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexEntity,testConstructor2)
{
	LatexEntity entity("A_2");
	EXPECT_EQ("A",entity.getName());
	EXPECT_EQ(0,entity.countExponents());
	EXPECT_EQ(1,entity.countIndices());
	EXPECT_EQ(0,entity.countParameters());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexEntity,testGetString_1)
{
	LatexEntity entity(" A_2^3 ");
	EXPECT_EQ("A_2^3",entity.getString());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexEntity,testGetString_2)
{
	LatexEntity entity(" \\frac{ a } { b } ");
	EXPECT_EQ("\\frac{a}{b}",entity.getString());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexEntity,testGetString_3)
{
	LatexEntity entity(" ( 4 * d ) ");
	EXPECT_EQ("(4*d)",entity.getString());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexEntity,testGetString_4)
{
	LatexEntity entity(" A_{ 2 }^{ 3 } ");
	EXPECT_EQ("A_2^3",entity.getString());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexEntity,testGetString_5)
{
	LatexEntity entity("a^{ab}");
	EXPECT_EQ("a^{ab}",entity.getString());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexEntity,testGetString_6)
{
	LatexEntity entity("a_{ab}");
	EXPECT_EQ("a_{ab}",entity.getString());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexEntity,testCountIndices_1)
{
	LatexEntity entity(" A_{ 2 }^{ 3 , a } ");
	EXPECT_EQ(1,entity.countIndices());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexEntity,testCountIndices_2)
{
	LatexEntity entity(" A_{ 2 , a , 5}^{ 3 , a } ");
	EXPECT_EQ(3,entity.countIndices());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexEntity,testCountExponents_1)
{
	LatexEntity entity(" A^{ 2 }_{ 3 , a } ");
	EXPECT_EQ(1,entity.countExponents());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexEntity,testCountExponents_2)
{
	LatexEntity entity(" A^{ 2 , a , 5}_{ 3 , a } ");
	EXPECT_EQ(3,entity.countExponents());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexEntity,testParameters)
{
	LatexEntity entity("\\frac{a}{b}");
	EXPECT_EQ(2,entity.countParameters());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexEntity,testParse_simple_ok)
{
	LatexEntity entity;
	entity.parse("A_2");
	EXPECT_EQ("A_2",entity.getString());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexEntity,testParse_simple_not_complete)
{
	LatexEntity entity;
	EXPECT_THROW(entity.parse("A_2 B_3"),LatexException);
}
/*******************  FUNCTION  *********************/
TEST(TestLatexEntity,testParse_complex_ok)
{
	LatexEntity entity1;
	LatexEntity entity2;
	LatexParsorContext context("A_2 B_3");
	EXPECT_EQ(0,context.getPosition());
	
	entity1.parse(context);
	EXPECT_EQ(4,context.getPosition());
	EXPECT_EQ("A_2",entity1.getString());
	
	entity2.parse(context);
	EXPECT_TRUE(context.isEnd());
	EXPECT_EQ("B_3",entity2.getString());
}


/*******************  FUNCTION  *********************/
TEST(TestLatexEntity,testClear)
{
	LatexEntity entity("A_2");
	EXPECT_EQ("A",entity.getName());
	EXPECT_EQ(0,entity.countExponents());
	EXPECT_EQ(1,entity.countIndices());
	EXPECT_EQ(0,entity.countParameters());
	entity.clear();
	EXPECT_EQ("",entity.getName());
	EXPECT_EQ(0,entity.countExponents());
	EXPECT_EQ(0,entity.countIndices());
	EXPECT_EQ(0,entity.countParameters());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexEntity,testDumpAsXml)
{
	LatexEntity entity("A_{2}^{3}");
	stringstream tmp;
	entity.dumpAsXml(tmp);
	EXPECT_EQ(TEST_CST_1,tmp.str());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexEntity,testDumpAsTree)
{
	LatexEntity entity("A_{2}^{3}");
	stringstream tmp;
	entity.dumpAsTree(tmp);
	EXPECT_EQ(TEST_CST_2,tmp.str());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexEntity,testExpression_ExponentOnParenthesis)
{
	LatexEntity entity("( a + 5 )^2");
	EXPECT_EQ("(a+5)^2",entity.getString());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexEntity,testCmrSubactionNameEscapte)
{
	LatexEntity entity("\\cmrsubaction{bounce\\_back}");
	EXPECT_EQ("\\cmrsubaction{bounce_back}",entity.getString());
}