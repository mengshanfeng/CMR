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
#include <../definitions/CMRProjectIterator.h>
#include <../definitions/CMRProjectConstant.h>

/**********************  USING  *********************/
using namespace testing;
using namespace std;

/*********************  CONSTS  *********************/
static const char TEST_STRING_1[] = "LOOP => k\n\
	EQ => \\CMRTMP^1{1} += b\n\
EQ => a = 4*\\CMRTMP^1{1}\n";
static const char TEST_STRING_2[] = "LOOP => k\n\
	EQ => \\CMRTMP^1{1} += b+\\frac{c}{b*5}-(4+8)\n\
EQ => a = 4*\\CMRTMP^1{1}\n";
static const char TEST_STRING_3[] = "LOOP => k\n\
	LOOP => l\n\
		EQ => \\CMRTMP^1{1} += b*5\n\
	EQ => \\CMRTMP^1{1} += b+\\CMRTMP^1{1}-(4+8)\n\
EQ => a = 4*\\CMRTMP^1{1}\n";

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

/*******************  FUNCTION  *********************/
TEST(TestExtractLoops,testConstructor)
{
	CMRTransformationExtractLoops transf;
}

/*******************  FUNCTION  *********************/
TEST(TestExtractLoops,testRun_1)
{
	CMRTransformationExtractLoops transf;
	MockTransfToPrint out;
	
	CMRProjectContext context;
	CMRProjectIterator it("k","testK",0,9);
	context.addEntry(&it);
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	CMRProjectCodeEquation & eq = root.addEquation("a","4 * \\sum_k{b}");
	
	transf.run(root);
	out.run(root);
	
	EXPECT_EQ(TEST_STRING_1,out.out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestExtractLoops,testRun_2)
{
	CMRTransformationExtractLoops transf;
	MockTransfToPrint out;
	
	CMRProjectContext context;
	CMRProjectIterator it("k","testK",0,9);
	context.addEntry(&it);
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	CMRProjectCodeEquation & eq = root.addEquation("a","4 * \\sum_k{b+\\frac{c}{b*5}-(4+8)}");
	
	transf.run(root);
	out.run(root);
	
	EXPECT_EQ(TEST_STRING_2,out.out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestExtractLoops,testRun_3)
{
	CMRTransformationExtractLoops transf;
	MockTransfToPrint out;
	
	CMRProjectContext context;
	CMRProjectIterator it("k","testK",0,9);
	CMRProjectIterator it2("l","testL",0,9);
	context.addEntry(&it);
	context.addEntry(&it2);
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("a","testA","int","0");
	root.addLocalVariable("b","testB","int","0");
	root.addLocalVariable("c","testC","int","0");
	CMRProjectCodeEquation & eq = root.addEquation("a","4 * \\sum_k{b+\\sum_l{b*5}-(4+8)}");
	
	transf.run(root);
	out.run(root);
	
	EXPECT_EQ(TEST_STRING_3,out.out.str());
}
