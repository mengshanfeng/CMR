/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <sstream>
#include "../CMRTransformationMarkNoTransf.h"
#include <CMRTransformationReplace.h>
#include <CMRTransformationExpandFrac.h>
#include <CMRTransformationExtractLoops.h>
#include <../definitions/CMRProjectCode.h>
#include <../definitions/ProjectIterator.h>
#include <../definitions/ProjectConstant.h>

/**********************  USING  *********************/
using namespace testing;
using namespace std;
using namespace CMRCompiler;

/*********************  CONSTS  *********************/
static const char TEST_STRING_1[] = "LOOP => k\n\
	EQ => \\CMRTMP^2{0} += b\n\
EQ => a = 4*\\CMRTMP^2{0}\n";
static const char TEST_STRING_2[] = "LOOP => k\n\
	EQ => \\CMRTMP^2{1} += b+\\frac{c}{b*5}-(4+8)\n\
EQ => a = 4*\\CMRTMP^2{1}\n";
static const char TEST_STRING_3[] = "LOOP => k\n\
	LOOP => l\n\
		EQ => \\CMRTMP^3{3} += b*5\n\
	EQ => \\CMRTMP^2{2} += b+\\CMRTMP^3{3}-(4+8)\n\
EQ => a = 4*\\CMRTMP^2{2}\n";

/*********************  STRUCT  *********************/
struct TestExtractLoopsParam
{
	const char * in;
	const char * out;
};

/*********************  CONSTS  *********************/
static const TestExtractLoopsParam CST_TESTED_VALUES[] = { 
	{"a"                                   , "EQ => a = a\n"             },
	{"4 * \\sum_k{b}"                      , TEST_STRING_1     },
	{"4 * \\sum_k{b+\\frac{c}{b*5}-(4+8)}" , TEST_STRING_2     },
	{"4 * \\sum_k{b+\\sum_l{b*5}-(4+8)}"   , TEST_STRING_3     },
};

/*********************  CLASS  **********************/
class MockTransfToPrint:  public CMRTransformationBasic
{
	public:
		MockTransfToPrint ( void ) : CMRTransformationBasic("mock"){};
		virtual void transform ( CMRProjectCodeEquation& equation ) {padd(equation);out << "EQ => " << equation.getOutput() << " " << equation.getOperator() << " " << equation.getFormulas() << endl;CMRTransformationBasic::transform(equation);};
		virtual void transform ( CMRProjectCodeIteratorLoop& loop ) {padd(loop);out << "LOOP => " << loop.getIterator() << endl;CMRTransformationBasic::transform(loop);};
		void padd(CMRProjectCodeEntry & entry) {int depth = entry.getDepth(); for (int i = 1 ; i < depth ; i++) out << "\t";};
	public:
		std::stringstream out;
};

/*********************  CLASS  **********************/
class TestExtractLoopsBase : public TestWithParam<TestExtractLoopsParam>
{
};

/*******************  FUNCTION  *********************/
TEST_P(TestExtractLoopsBase,simpleTest)
{
	CMRTransformationExtractLoops transf;
	MockTransfToPrint out;

	ProjectContext context;
	ProjectIterator it("k","testK",0,9);
	ProjectIterator it2("l","testL",0,9);
	context.addEntry(&it);
	context.addEntry(&it2);
	CMRProjectCodeNode root(&context);

	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	
	const TestExtractLoopsParam param = GetParam();
	
	root.addEquation("a",param.in);
	
	transf.run(root);
	out.run(root);
	
	EXPECT_EQ(param.out,out.out.str());
}

/********************  MACRO  ***********************/
INSTANTIATE_TEST_CASE_P(TestExtractLoops, TestExtractLoopsBase, ValuesIn(CST_TESTED_VALUES));
