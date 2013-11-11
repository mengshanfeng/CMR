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
#include <LatexFormula.h>
#include <LatexParsorContext.h>

/**********************  USING  *********************/
using namespace std;
using namespace testing;
using namespace CMRCompiler;

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
TEST(TestLatexFormula,testConstructor_1)
{
	LatexFormulas f;
	EXPECT_TRUE(f.empty());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testConstructor_2)
{
	LatexFormulas f("A_2 * B_3");
	EXPECT_EQ(3,f.size());
	EXPECT_EQ("A_2*B_3",f.getString());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testParse_1)
{
	LatexFormulas f;
	f.parse("A_2 * B_3");
	EXPECT_EQ(3,f.size());
	EXPECT_EQ("A_2*B_3",f.getString());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testParse_2)
{
	LatexFormulas f;
	LatexParsorContext context("A_2 * B_3");
	EXPECT_EQ(0,context.getPosition());
	f.parse(context);
	EXPECT_TRUE(context.isEnd());
	EXPECT_EQ(3,f.size());
	EXPECT_EQ("A_2*B_3",f.getString());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testParse_3)
{
	LatexFormulas f;
	EXPECT_THROW(f.parse("A_{2"),LatexException);
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testSplit_1)
{
	LatexFormulas f("A_2,B_3");
	EXPECT_EQ(3,f.size());
	LatexFormulasVector vect;
	f.split(vect,",");
	EXPECT_EQ(2,vect.size());
	EXPECT_EQ("A_2",vect[0]->getString());
	EXPECT_EQ("B_3",vect[1]->getString());
	EXPECT_TRUE(f.empty());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testSplit_2)
{
	LatexFormulas f("A_2");
	EXPECT_EQ(1,f.size());
	LatexFormulasVector vect;
	f.split(vect,",");
	EXPECT_EQ(1,vect.size());
	EXPECT_EQ("A_2",vect[0]->getString());
	EXPECT_TRUE(f.empty());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testGetString)
{
	LatexFormulas f("  5 + ( A_2 * B_3 )  ");
	EXPECT_EQ("5+(A_2*B_3)",f.getString());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testDumpAsXml)
{
	LatexFormulas f("  5 + ( A_2 * B_3 )  ");
	stringstream tmp;
	f.dumpAsXml(tmp);
	EXPECT_EQ(TEST_CST_1,tmp.str());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testDumpAsTree)
{
	LatexFormulas f("  5 + ( A_2 * B_3 )  ");
	stringstream tmp;
	f.dumpAsTree(tmp);
	EXPECT_EQ(TEST_CST_2,tmp.str());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testClear)
{
	LatexFormulas f("  5 + ( A_2 * B_3 )  ");
	EXPECT_FALSE(f.empty());
	f.clear();
	EXPECT_TRUE(f.empty());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testIsOnlyOneName_1)
{
	LatexFormulas f("A");
	EXPECT_TRUE(f.isOnlyOneName());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testIsOnlyOneName_2)
{
	LatexFormulas f("A_2");
	EXPECT_FALSE(f.isOnlyOneName());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testIsOnlyOneName_3)
{
	LatexFormulas f("A+B");
	EXPECT_FALSE(f.isOnlyOneName());
}
