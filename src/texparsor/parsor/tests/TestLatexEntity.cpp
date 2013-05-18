/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <svUnitTest.h>
#include <CMRLatexEntity.h>
#include <sstream>
#include <CMRLatexParsorContext.h>

/**********************  USING  *********************/
using namespace svUnitTest;
using namespace std;

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
SVUT_DECLARE_FLAT_TEST(TestLatexEntity,testConstructor1)
{
	CMRLatexEntity2 entity;
	SVUT_ASSERT_EQUAL("",entity.getName());
	SVUT_ASSERT_EQUAL(0,entity.countExponents());
	SVUT_ASSERT_EQUAL(0,entity.countIndices());
	SVUT_ASSERT_EQUAL(0,entity.countParameters());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexEntity,testConstructor2)
{
	CMRLatexEntity2 entity("A_2");
	SVUT_ASSERT_EQUAL("A",entity.getName());
	SVUT_ASSERT_EQUAL(0,entity.countExponents());
	SVUT_ASSERT_EQUAL(1,entity.countIndices());
	SVUT_ASSERT_EQUAL(0,entity.countParameters());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexEntity,testGetString_1)
{
	CMRLatexEntity2 entity(" A_2^3 ");
	SVUT_ASSERT_EQUAL("A_2^3",entity.getString());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexEntity,testGetString_2)
{
	CMRLatexEntity2 entity(" \\frac{ a } { b } ");
	SVUT_ASSERT_EQUAL("\\frac{a}{b}",entity.getString());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexEntity,testGetString_3)
{
	CMRLatexEntity2 entity(" ( 4 * d ) ");
	SVUT_ASSERT_EQUAL("(4*d)",entity.getString());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexEntity,testGetString_4)
{
	CMRLatexEntity2 entity(" A_{ 2 }^{ 3 } ");
	SVUT_ASSERT_EQUAL("A_2^3",entity.getString());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexEntity,testGetString_5)
{
	CMRLatexEntity2 entity("a^{ab}");
	SVUT_ASSERT_EQUAL("a^{ab}",entity.getString());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexEntity,testGetString_6)
{
	CMRLatexEntity2 entity("a_{ab}");
	SVUT_ASSERT_EQUAL("a_{ab}",entity.getString());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexEntity,testCountIndices_1)
{
	CMRLatexEntity2 entity(" A_{ 2 }^{ 3 , a } ");
	SVUT_ASSERT_EQUAL(1,entity.countIndices());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexEntity,testCountIndices_2)
{
	CMRLatexEntity2 entity(" A_{ 2 , a , 5}^{ 3 , a } ");
	SVUT_ASSERT_EQUAL(3,entity.countIndices());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexEntity,testCountExponents_1)
{
	CMRLatexEntity2 entity(" A^{ 2 }_{ 3 , a } ");
	SVUT_ASSERT_EQUAL(1,entity.countExponents());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexEntity,testCountExponents_2)
{
	CMRLatexEntity2 entity(" A^{ 2 , a , 5}_{ 3 , a } ");
	SVUT_ASSERT_EQUAL(3,entity.countExponents());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexEntity,testParameters)
{
	CMRLatexEntity2 entity("\\frac{a}{b}");
	SVUT_ASSERT_EQUAL(2,entity.countParameters());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexEntity,testParse_simple_ok)
{
	CMRLatexEntity2 entity;
	entity.parse("A_2");
	SVUT_ASSERT_EQUAL("A_2",entity.getString());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexEntity,testParse_simple_not_complete)
{
	CMRLatexEntity2 entity;
	SVUT_ASSERT_THROW(CMRLatexException,entity.parse("A_2 B_3"));
}
/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexEntity,testParse_complex_ok)
{
	CMRLatexEntity2 entity1;
	CMRLatexEntity2 entity2;
	CMRLatexParsorContext context("A_2 B_3");
	SVUT_ASSERT_EQUAL(0,context.getPosition());
	
	entity1.parse(context);
	SVUT_ASSERT_EQUAL(4,context.getPosition());
	SVUT_ASSERT_EQUAL("A_2",entity1.getString());
	
	entity2.parse(context);
	SVUT_ASSERT_TRUE(context.isEnd());
	SVUT_ASSERT_EQUAL("B_3",entity2.getString());
}


/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexEntity,testClear)
{
	CMRLatexEntity2 entity("A_2");
	SVUT_ASSERT_EQUAL("A",entity.getName());
	SVUT_ASSERT_EQUAL(0,entity.countExponents());
	SVUT_ASSERT_EQUAL(1,entity.countIndices());
	SVUT_ASSERT_EQUAL(0,entity.countParameters());
	entity.clear();
	SVUT_ASSERT_EQUAL("",entity.getName());
	SVUT_ASSERT_EQUAL(0,entity.countExponents());
	SVUT_ASSERT_EQUAL(0,entity.countIndices());
	SVUT_ASSERT_EQUAL(0,entity.countParameters());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexEntity,testDumpAsXml)
{
	CMRLatexEntity2 entity("A_{2}^{3}");
	stringstream tmp;
	entity.dumpAsXml(tmp);
	SVUT_ASSERT_EQUAL(TEST_CST_1,tmp.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexEntity,testDumpAsTree)
{
	CMRLatexEntity2 entity("A_{2}^{3}");
	stringstream tmp;
	entity.dumpAsTree(tmp);
	SVUT_ASSERT_EQUAL(TEST_CST_2,tmp.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexEntity,testExpreession_ExponentOnParenthesis)
{
	CMRLatexEntity2 entity("( a + 5 )^2");
	SVUT_ASSERT_EQUAL("(a+5)^2",entity.getString());
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
