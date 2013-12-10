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
#include <FormulaMatcher.h>
#include "transformations/tests/MockTransfBasic.h"

/**********************  USING  *********************/
using namespace std;
using namespace testing;
using namespace CMRCompiler;

/*********************  CONSTS  *********************/
static const char TEST_CST_1[] = "FORMULA[ ENTITY [ a , FORMULA[ ENTITY [ 3 ,  ] ]FORMULA[ ENTITY [ 5 ,  ] ] ]ENTITY [ + ,  ]ENTITY_CAPTURE [ c ,  ]ENTITY [ + ,  ]ENTITY [ v , FORMULA[ ENTITY [ i ,  ]ENTITY [ + ,  ]ENTITY [ \\frac , FORMULA[ ENTITY [ 1 ,  ] ]FORMULA[ ENTITY [ 2 ,  ] ] ] ] ] ]";
static const char TEST_CST_2[] = "FORMULA[ ENTITY_CAPTURE [ a , FORMULA[ ENTITY [ 3 ,  ] ]FORMULA[ ENTITY [ 5 ,  ] ] ]ENTITY [ + ,  ]ENTITY [ c ,  ]ENTITY [ + ,  ]ENTITY [ v , FORMULA[ ENTITY [ i ,  ]ENTITY [ + ,  ]ENTITY [ \\frac , FORMULA[ ENTITY [ 1 ,  ] ]FORMULA[ ENTITY [ 2 ,  ] ] ] ] ] ]";
static const char TEST_CST_3[] =  "FORMULA[ ENTITY [ a , FORMULA[ ENTITY [ 3 ,  ] ]FORMULA[ ENTITY [ 5 ,  ] ] ]ENTITY [ + ,  ]ENTITY [ c ,  ]ENTITY [ + ,  ]ENTITY [ v , FORMULA[ ENTITY_CAPTURE [ i ,  ]ENTITY [ + ,  ]ENTITY [ \\frac , FORMULA[ ENTITY [ 1 ,  ] ]FORMULA[ ENTITY [ 2 ,  ] ] ] ] ] ]";
static const char TEST_CST_4[] =  "FORMULA[ ENTITY [ a , FORMULA[ ENTITY [ 3 ,  ] ]FORMULA[ ENTITY [ 5 ,  ] ] ]ENTITY [ + ,  ]ENTITY [ c ,  ]ENTITY [ + ,  ]ENTITY_CAPTURE [ v , FORMULA[ ENTITY [ i ,  ]ENTITY [ + ,  ]ENTITY [ \\frac , FORMULA[ ENTITY [ 1 ,  ] ]FORMULA[ ENTITY [ 2 ,  ] ] ] ] ] ]";
static const char TEST_CST_5[] = "FORMULA[ ENTITY [ a , FORMULA[ ENTITY [ 3 ,  ] ]FORMULA[ ENTITY [ 5 ,  ] ] ]ENTITY [ + ,  ]ENTITY [ c ,  ]ENTITY [ + ,  ]ENTITY [ v , FORMULA[ ENTITY [ i ,  ]ENTITY [ + ,  ]ENTITY [ \\frac , FORMULA[ ENTITY_CAPTURE [ d ,  ]ENTITY [ * ,  ]ENTITY [ u , FORMULA[ ENTITY [ k ,  ] ] ] ]FORMULA[ ENTITY [ 2 ,  ] ] ] ] ] ]";
static const char TEST_CST_6[] = "FORMULA[ ENTITY [ a , FORMULA[ ENTITY [ 3 ,  ] ]FORMULA[ ENTITY [ 5 ,  ] ] ]ENTITY [ + ,  ]ENTITY [ c ,  ]ENTITY [ + ,  ]ENTITY [ v , FORMULA[ ENTITY [ i ,  ]ENTITY [ + ,  ]ENTITY [ \\frac , FORMULA[ ENTITY [ d ,  ]ENTITY [ * ,  ]ENTITY [ u , FORMULA[ ENTITY_CAPTURE [ k ,  ] ] ] ]FORMULA[ ENTITY [ 2 ,  ] ] ] ] ] ]";
static const char TEST_CST_7[] =  "FORMULA[ ENTITY_CAPTURE [ x , FORMULA[ ENTITY_CAPTURE [ y ,  ] ] ] ]";

/********************  MACRO  ***********************/
#define EXPECT_NOT_NULL(x) EXPECT_NE((const void*)NULL,(const void*)(x))


/*********************  CLASS  **********************/
class ForTestFormulaMatcher : public FormulaMatcher
{
	public:
		ForTestFormulaMatcher(const std::string & value) : FormulaMatcher(value) {};
		LatexFormulas & internalGetFormula(void) {return formula;};
};

/*******************  FUNCTION  *********************/
TEST(TestFormulaMatcher,testConstructor)
{
	FormulaMatcher matcher("a^{3,5}+c+v_{i+\\frac{1}{2}}");
}

/*******************  FUNCTION  *********************/
TEST(TestFormulaMatcher,testMarkForCapture_1)
{
	ForTestFormulaMatcher matcher("a^{3,5}+c+v_{i+\\frac{1}{2}}");
	matcher.markForCapture("c");
	
	MockTransfBasic transf("FormulaMatcher::captureFlag","CAPTURE");
	transf.transform((*(CMRProjectCodeEquation*)NULL),matcher.internalGetFormula());
	
	EXPECT_EQ(TEST_CST_1,transf.out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestFormulaMatcher,testMarkForCapture_2)
{
	ForTestFormulaMatcher matcher("a^{3,5}+c+v_{i+\\frac{1}{2}}");
	matcher.markForCapture("a");
	
	MockTransfBasic transf("FormulaMatcher::captureFlag","CAPTURE");
	transf.transform((*(CMRProjectCodeEquation*)NULL),matcher.internalGetFormula());
	
	EXPECT_EQ(TEST_CST_2,transf.out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestFormulaMatcher,testMarkForCapture_3)
{
	ForTestFormulaMatcher matcher("a^{3,5}+c+v_{i+\\frac{1}{2}}");
	matcher.markForCapture("i");
	
	MockTransfBasic transf("FormulaMatcher::captureFlag","CAPTURE");
	transf.transform((*(CMRProjectCodeEquation*)NULL),matcher.internalGetFormula());
	
	EXPECT_EQ(TEST_CST_3,transf.out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestFormulaMatcher,testMarkForCapture_4)
{
	ForTestFormulaMatcher matcher("a^{3,5}+c+v_{i+\\frac{1}{2}}");
	matcher.markForCapture("v");
	
	MockTransfBasic transf("FormulaMatcher::captureFlag","CAPTURE");
	transf.transform((*(CMRProjectCodeEquation*)NULL),matcher.internalGetFormula());
	
	EXPECT_EQ(TEST_CST_4,transf.out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestFormulaMatcher,testMarkForCapture_5)
{
	ForTestFormulaMatcher matcher("a^{3,5}+c+v_{i+\\frac{d*u^k}{2}}");
	matcher.markForCapture("d");
	
	MockTransfBasic transf("FormulaMatcher::captureFlag","CAPTURE");
	transf.transform((*(CMRProjectCodeEquation*)NULL),matcher.internalGetFormula());
	
	EXPECT_EQ(TEST_CST_5,transf.out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestFormulaMatcher,testMarkForCapture_6)
{
	ForTestFormulaMatcher matcher("a^{3,5}+c+v_{i+\\frac{d*u^k}{2}}");
	matcher.markForCapture("k");
	
	MockTransfBasic transf("FormulaMatcher::captureFlag","CAPTURE");
	transf.transform((*(CMRProjectCodeEquation*)NULL),matcher.internalGetFormula());
	
	EXPECT_EQ(TEST_CST_6,transf.out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestFormulaMatcher,testMarkForCapture_7)
{
	ForTestFormulaMatcher matcher("x^y");
	matcher.markForCapture("x");
	matcher.markForCapture("y");
	
	MockTransfBasic transf("FormulaMatcher::captureFlag","CAPTURE");
	transf.transform((*(CMRProjectCodeEquation*)NULL),matcher.internalGetFormula());
	
	EXPECT_EQ(TEST_CST_7,transf.out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestFormulaMatcher,testMatchExact_1)
{
	FormulaMatcher matcher("a^3");
	
	LatexFormulas ok("a^3");
	LatexFormulas nok1("a^3+2");
	LatexFormulas nok2("a^2");
	LatexFormulas nok3("b^3");
	
	EXPECT_TRUE(matcher.match(ok));
	EXPECT_FALSE(matcher.match(nok1));
	EXPECT_FALSE(matcher.match(nok2));
	EXPECT_FALSE(matcher.match(nok3));
}

/*******************  FUNCTION  *********************/
TEST(TestFormulaMatcher,testMatchExact_2)
{
	FormulaMatcher matcher("a^3+b^2");
	
	LatexFormulas ok("a^3+b^2");
	LatexFormulas nok1("a^3+b^2+c");
	LatexFormulas nok2("a^2+b^2");
	LatexFormulas nok3("b^2");
	LatexFormulas nok4("a^3+b^3");
	
	EXPECT_TRUE(matcher.match(ok));
	EXPECT_FALSE(matcher.match(nok1));
	EXPECT_FALSE(matcher.match(nok2));
	EXPECT_FALSE(matcher.match(nok3));
	EXPECT_FALSE(matcher.match(nok4));
}

/*******************  FUNCTION  *********************/
TEST(TestFormulaMatcher,testMatchPartialRoot_1)
{
	FormulaMatcher matcher("a^3");
	
	LatexFormulas ok("a^3");
	LatexFormulas ok1("a^3+2");
	LatexFormulas nok2("a^2");
	LatexFormulas nok3("b^3");
	LatexFormulas nok4("a^{3+2}+2");
	
	EXPECT_TRUE(matcher.match(ok,FORMULA_MATCHER_DEFAULT|FORMULA_MATCHER_ROOT_PARTIAL));
	EXPECT_TRUE(matcher.match(ok1,FORMULA_MATCHER_DEFAULT|FORMULA_MATCHER_ROOT_PARTIAL));
	EXPECT_FALSE(matcher.match(nok2,FORMULA_MATCHER_DEFAULT|FORMULA_MATCHER_ROOT_PARTIAL));
	EXPECT_FALSE(matcher.match(nok3,FORMULA_MATCHER_DEFAULT|FORMULA_MATCHER_ROOT_PARTIAL));
	EXPECT_FALSE(matcher.match(nok4,FORMULA_MATCHER_DEFAULT|FORMULA_MATCHER_ROOT_PARTIAL));
}

/*******************  FUNCTION  *********************/
TEST(TestFormulaMatcher,testMatchCapture_1)
{
	FormulaMatcher matcher("a^3");
	matcher.markForCapture("a");
	
	LatexFormulas ok("a^3");
	LatexFormulas nok1("a^3+2");
	LatexFormulas ok2("b^3");
	LatexFormulas nok3("b^2");
	LatexFormulas nok4("c^{3+2}+2");
	
	EXPECT_TRUE(matcher.match(ok));
	EXPECT_FALSE(matcher.match(nok1));
	EXPECT_TRUE(matcher.match(ok2));
	EXPECT_FALSE(matcher.match(nok3));
	EXPECT_FALSE(matcher.match(nok4));
}

/*******************  FUNCTION  *********************/
TEST(TestFormulaMatcher,testMatchCapture_2)
{
	FormulaMatcher matcher("a^b");
	matcher.markForCapture("b");
	
	LatexFormulas ok("a^3");
	LatexFormulas nok1("a^3+2");
	LatexFormulas nok2("b^3");
	LatexFormulas nok3("b^2");
	LatexFormulas nok4("c^{3+2}+2");
	
	EXPECT_TRUE(matcher.match(ok));
	EXPECT_FALSE(matcher.match(nok1));
	EXPECT_FALSE(matcher.match(nok2));
	EXPECT_FALSE(matcher.match(nok3));
	EXPECT_FALSE(matcher.match(nok4));
}

/*******************  FUNCTION  *********************/
TEST(TestFormulaMatcher,testMatchCapture_3)
{
	FormulaMatcher matcher("a_{i+\\frac{1}{2}}");
	matcher.markForCapture("i");
	
	LatexFormulas test1("a_{k+\\frac{1}{2}}");
	LatexFormulas test2("a_{j+\\frac{1}{2}}");
	LatexFormulas test3("a_{j+1+\\frac{1}{2}}");
	LatexFormulas test4("a_{(j+1)+\\frac{1}{2}}");
	LatexFormulas test5("b_{i+\\frac{1}{2}}");
	LatexFormulas test6("a_{i+\\frac{1}{4}}");
	
	EXPECT_TRUE(matcher.match(test1));
	EXPECT_TRUE(matcher.match(test2));
	EXPECT_FALSE(matcher.match(test3));
	EXPECT_TRUE(matcher.match(test4));
	EXPECT_FALSE(matcher.match(test5));
	EXPECT_FALSE(matcher.match(test6));
}

/*******************  FUNCTION  *********************/
TEST(TestFormulaMatcher,testMatchCapture_4)
{
	FormulaMatcher matcher("x^y");
	matcher.markForCapture("x");
	matcher.markForCapture("y");
	
	LatexFormulas test1("a^2");
	LatexFormulas test2("b^c");
	LatexFormulas test3("a^{2,4}");
	
	EXPECT_TRUE(matcher.match(test1));
	EXPECT_TRUE(matcher.match(test2));
	EXPECT_FALSE(matcher.match(test3));
}

/*******************  FUNCTION  *********************/
TEST(TestFormulaMatcher,testMatchCapture_5)
{
	FormulaMatcher matcher("xyz");
	matcher.markForCapture("x");
	matcher.markForCapture("y");
	matcher.markForCapture("z");
	
	LatexFormulas test1("a+b");
	LatexFormulas test2("abc");
	LatexFormulas test3("a+cd");
	LatexFormulas test4("a*b*c");
	
	EXPECT_FALSE(matcher.match(test1));
	EXPECT_TRUE(matcher.match(test2));
	EXPECT_FALSE(matcher.match(test3));
	EXPECT_FALSE(matcher.match(test4));
}

/*******************  FUNCTION  *********************/
TEST(TestFormulaMatcher,testMatchCapture_6)
{
	FormulaMatcher matcher("xyz");
	matcher.markForCapture("x");
	matcher.markForCapture("y",new FormulaMatcherFilterDefault(ENTITY_CAT_OPERATOR));
	matcher.markForCapture("z");
	
	LatexFormulas test1("a+b");
	LatexFormulas test2("abc");
	LatexFormulas test3("a+cd");
	LatexFormulas test4("a*b*c");
	
	EXPECT_TRUE(matcher.match(test1));
	EXPECT_FALSE(matcher.match(test2));
	EXPECT_FALSE(matcher.match(test3));
	EXPECT_FALSE(matcher.match(test4));
}

/*******************  FUNCTION  *********************/
TEST(TestFormulaMatcher,testExtractValue_1)
{
	FormulaMatcher matcher("a^b");
	matcher.markForCapture("b");
	
	LatexFormulas test1("a^3");
	LatexFormulas test2("a^(3+2)");
	
	FormulaMatcherResult res1;
	matcher.match(test1,res1,FORMULA_MATCHER_DEFAULT | FORMULA_MATCHER_DO_CAPTURE);
	EXPECT_NOT_NULL(res1.captures["b"]);
	EXPECT_EQ(res1.captures["b"]->getString(),"3");
	EXPECT_EQ(1,res1.cntCaptureMatch);
	EXPECT_EQ(1,res1.cntExactMatch);
	EXPECT_EQ(2,res1.cntCompared);
	
	FormulaMatcherResult res2;
	matcher.match(test2,res2,FORMULA_MATCHER_DEFAULT | FORMULA_MATCHER_DO_CAPTURE);
	EXPECT_NOT_NULL(res2.captures["b"]);
	EXPECT_EQ(res2.captures["b"]->getString(),"3+2");
	EXPECT_EQ(1,res1.cntCaptureMatch);
	EXPECT_EQ(1,res1.cntExactMatch);
	EXPECT_EQ(2,res1.cntCompared);
}

/*******************  FUNCTION  *********************/
TEST(TestFormulaMatcher,testMiddleMatch_1)
{
	FormulaMatcher matcher("a^b");
	matcher.markForCapture("b");
	
	LatexFormulas test1("a^3");
	
	LatexFormulas::const_iterator it = test1.begin();
	EXPECT_TRUE(matcher.match(test1,it));
}

/*******************  FUNCTION  *********************/
TEST(TestFormulaMatcher,testMiddleMatch_2)
{
	FormulaMatcher matcher("a^b");
	matcher.markForCapture("b");
	
	LatexFormulas test2("cc+a^3");
	LatexFormulas test3("dd+a^(3+2)+rerer");
	
	LatexFormulas::const_iterator it;
	it = test2.begin();
	EXPECT_FALSE(matcher.match(test2,it));
	EXPECT_EQ(0,it - test2.begin());
	it = test2.begin() + 3;
	EXPECT_TRUE(matcher.match(test2,it));
	EXPECT_EQ(4,it - test2.begin());
	EXPECT_EQ(test2.end(),it);

}

/*******************  FUNCTION  *********************/
TEST(TestFormulaMatcher,testMiddleMatch_3)
{
	FormulaMatcher matcher("a^b");
	matcher.markForCapture("b");

	LatexFormulas test3("dd+a^(3+2)+rerer");
	
	LatexFormulas::const_iterator it;
	it = test3.begin();
	EXPECT_FALSE(matcher.match(test3,it,FORMULA_MATCHER_DEFAULT|FORMULA_MATCHER_ROOT_PARTIAL));
	EXPECT_EQ(0,it - test3.begin());
	it = test3.begin() + 3;
	EXPECT_TRUE(matcher.match(test3,it,FORMULA_MATCHER_DEFAULT|FORMULA_MATCHER_ROOT_PARTIAL));
	EXPECT_EQ(4,it - test3.begin());
}
