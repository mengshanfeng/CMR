/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <CMRLatexFormula.h>
#include <CMRLatexParsorContext.h>

/**********************  USING  *********************/
using namespace testing;

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
	CMRLatexFormulas2 f;
	EXPECT_TRUE(f.empty());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testConstructor_2)
{
	CMRLatexFormulas2 f("A_2 * B_3");
	EXPECT_EQ(3,f.size());
	EXPECT_EQ("A_2*B_3",f.getString());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testParse_1)
{
	CMRLatexFormulas2 f;
	f.parse("A_2 * B_3");
	EXPECT_EQ(3,f.size());
	EXPECT_EQ("A_2*B_3",f.getString());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testParse_2)
{
	CMRLatexFormulas2 f;
	CMRLatexParsorContext context("A_2 * B_3");
	EXPECT_EQ(0,context.getPosition());
	f.parse(context);
	EXPECT_TRUE(context.isEnd());
	EXPECT_EQ(3,f.size());
	EXPECT_EQ("A_2*B_3",f.getString());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testParse_3)
{
	CMRLatexFormulas2 f;
	EXPECT_THROW(f.parse("A_{2"),CMRLatexException);
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testSplit_1)
{
	CMRLatexFormulas2 f("A_2,B_3");
	EXPECT_EQ(3,f.size());
	CMRLatexFormulasVector2 vect;
	f.split(vect,",");
	EXPECT_EQ(2,vect.size());
	EXPECT_EQ("A_2",vect[0]->getString());
	EXPECT_EQ("B_3",vect[1]->getString());
	EXPECT_TRUE(f.empty());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testSplit_2)
{
	CMRLatexFormulas2 f("A_2");
	EXPECT_EQ(1,f.size());
	CMRLatexFormulasVector2 vect;
	f.split(vect,",");
	EXPECT_EQ(1,vect.size());
	EXPECT_EQ("A_2",vect[0]->getString());
	EXPECT_TRUE(f.empty());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testGetString)
{
	CMRLatexFormulas2 f("  5 + ( A_2 * B_3 )  ");
	EXPECT_EQ("5+(A_2*B_3)",f.getString());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testDumpAsXml)
{
	CMRLatexFormulas2 f("  5 + ( A_2 * B_3 )  ");
	std::stringstream tmp;
	f.dumpAsXml(tmp);
	EXPECT_EQ(TEST_CST_1,tmp.str());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testDumpAsTree)
{
	CMRLatexFormulas2 f("  5 + ( A_2 * B_3 )  ");
	std::stringstream tmp;
	f.dumpAsTree(tmp);
	EXPECT_EQ(TEST_CST_2,tmp.str());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testClear)
{
	CMRLatexFormulas2 f("  5 + ( A_2 * B_3 )  ");
	EXPECT_FALSE(f.empty());
	f.clear();
	EXPECT_TRUE(f.empty());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testIsOnlyOneName_1)
{
	CMRLatexFormulas2 f("A");
	EXPECT_TRUE(f.isOnlyOneName());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testIsOnlyOneName_2)
{
	CMRLatexFormulas2 f("A_2");
	EXPECT_FALSE(f.isOnlyOneName());
}

/*******************  FUNCTION  *********************/
TEST(TestLatexFormula,testIsOnlyOneName_3)
{
	CMRLatexFormulas2 f("A+B");
	EXPECT_FALSE(f.isOnlyOneName());
}
