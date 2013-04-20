/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <svUnitTest.h>
#include <CMRLatexFormula.h>
#include <CMRLatexParsorContext.h>

/**********************  USING  *********************/
using namespace svUnitTest;

/*********************  CONSTS  *********************/
static const char TEST_CST_1[] = "<formula>\n\
  <entity>\n\
    <name>5</name>\n\
  </entity>\n\
  <entity>\n\
    <name>+</name>\n\
  </entity>\n\
  <entity>\n\
    <name>()</name>\n\
    <parameters>\n\
      <formula>\n\
        <entity>\n\
          <name>A</name>\n\
          <indices>\n\
            <formula>\n\
              <entity>\n\
                <name>2</name>\n\
              </entity>\n\
            </formula>\n\
          </indices>\n\
        </entity>\n\
        <entity>\n\
          <name>*</name>\n\
        </entity>\n\
        <entity>\n\
          <name>B</name>\n\
          <indices>\n\
            <formula>\n\
              <entity>\n\
                <name>3</name>\n\
              </entity>\n\
            </formula>\n\
          </indices>\n\
        </entity>\n\
      </formula>\n\
    </parameters>\n\
  </entity>\n\
</formula>\n\
";
static const char TEST_CST_2[] = "formula:\n\
  entity: 5\n\
  entity: +\n\
  entity: ()\n\
    parameters:\n\
      formula:\n\
        entity: A\n\
          indices: 2\n\
        entity: *\n\
        entity: B\n\
          indices: 3\n\
";

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexFormula,testConstructor_1)
{
	CMRLatexFormulas2 f;
	SVUT_ASSERT_TRUE(f.empty());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexFormula,testConstructor_2)
{
	CMRLatexFormulas2 f("A_2 * B_3");
	SVUT_ASSERT_EQUAL(3,f.size());
	SVUT_ASSERT_EQUAL("A_2*B_3",f.getString());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexFormula,testParse_1)
{
	CMRLatexFormulas2 f;
	f.parse("A_2 * B_3");
	SVUT_ASSERT_EQUAL(3,f.size());
	SVUT_ASSERT_EQUAL("A_2*B_3",f.getString());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexFormula,testParse_2)
{
	CMRLatexFormulas2 f;
	CMRLatexParsorContext context("A_2 * B_3");
	SVUT_ASSERT_EQUAL(0,context.getPosition());
	f.parse(context);
	SVUT_ASSERT_TRUE(context.isEnd());
	SVUT_ASSERT_EQUAL(3,f.size());
	SVUT_ASSERT_EQUAL("A_2*B_3",f.getString());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexFormula,testParse_3)
{
	CMRLatexFormulas2 f;
	SVUT_ASSERT_THROW(CMRLatexException,f.parse("A_{2"));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexFormula,testSplit_1)
{
	CMRLatexFormulas2 f("A_2,B_3");
	SVUT_ASSERT_EQUAL(3,f.size());
	CMRLatexFormulasVector2 vect;
	f.split(vect,",");
	SVUT_ASSERT_EQUAL(2,vect.size());
	SVUT_ASSERT_EQUAL("A_2",vect[0]->getString());
	SVUT_ASSERT_EQUAL("B_3",vect[1]->getString());
	SVUT_ASSERT_TRUE(f.empty());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexFormula,testSplit_2)
{
	CMRLatexFormulas2 f("A_2");
	SVUT_ASSERT_EQUAL(1,f.size());
	CMRLatexFormulasVector2 vect;
	f.split(vect,",");
	SVUT_ASSERT_EQUAL(1,vect.size());
	SVUT_ASSERT_EQUAL("A_2",vect[0]->getString());
	SVUT_ASSERT_TRUE(f.empty());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexFormula,testGetString)
{
	CMRLatexFormulas2 f("  5 + ( A_2 * B_3 )  ");
	SVUT_ASSERT_EQUAL("5+(A_2*B_3)",f.getString());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexFormula,testDumpAsXml)
{
	CMRLatexFormulas2 f("  5 + ( A_2 * B_3 )  ");
	std::stringstream tmp;
	f.dumpAsXml(tmp);
	SVUT_ASSERT_EQUAL(TEST_CST_1,tmp.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexFormula,testDumpAsTree)
{
	CMRLatexFormulas2 f("  5 + ( A_2 * B_3 )  ");
	std::stringstream tmp;
	f.dumpAsTree(tmp);
	SVUT_ASSERT_EQUAL(TEST_CST_2,tmp.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexFormula,testClear)
{
	CMRLatexFormulas2 f("  5 + ( A_2 * B_3 )  ");
	SVUT_ASSERT_FALSE(f.empty());
	f.clear();
	SVUT_ASSERT_TRUE(f.empty());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexFormula,testIsOnlyOneName_1)
{
	CMRLatexFormulas2 f("A");
	SVUT_ASSERT_TRUE(f.isOnlyOneName());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexFormula,testIsOnlyOneName_2)
{
	CMRLatexFormulas2 f("A_2");
	SVUT_ASSERT_FALSE(f.isOnlyOneName());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestLatexFormula,testIsOnlyOneName_3)
{
	CMRLatexFormulas2 f("A+B");
	SVUT_ASSERT_FALSE(f.isOnlyOneName());
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
