/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include "../CMRTransformationBasic.h"
#include <../definitions/CMRProjectCode.h>
#include <../definitions/CMRProjectIterator.h>
#include <../definitions/CMRProjectConstant.h>

/**********************  USING  *********************/
using namespace testing;

/*********************  CONSTS  *********************/

/*********************  CLASS  **********************/
class MockTransfBasic : public CMRTransformationBasic
{
	public:
		MockTransfBasic(void);
		virtual void transform(CMRProjectCodeNode& node);
		virtual void transform(CMRProjectCodeIteratorLoop& loop);
		virtual void transform(CMRProjectCodeEquation& equation, CMRLatexFormulas2& formula);
		virtual void transform(CMRProjectCodeEquation& equation, CMRLatexEntity2& entity);
		virtual void transform(CMRProjectCodeEquation& equation);
		std::stringstream out;
};

/*******************  FUNCTION  *********************/
MockTransfBasic::MockTransfBasic(void )
	: CMRTransformationBasic("mock")
{

}

/*******************  FUNCTION  *********************/
void MockTransfBasic::transform(CMRProjectCodeNode& node)
{
	out << "NODE[ ";
	CMRTransformationBasic::transform(node);
	out << " ]"; 
}

/*******************  FUNCTION  *********************/
void MockTransfBasic::transform(CMRProjectCodeEquation& equation, CMRLatexFormulas2& formula)
{
	out << "FORMULA[ ";
	CMRTransformationBasic::transform(equation,formula);
	out << " ]"; 
}

/*******************  FUNCTION  *********************/
void MockTransfBasic::transform(CMRProjectCodeIteratorLoop& loop)
{
	out << "LOOP[ " << loop.getIterator().getLatexName() << " , ";
	CMRTransformationBasic::transform(loop);
	out << " ]";
}

/*******************  FUNCTION  *********************/
void MockTransfBasic::transform(CMRProjectCodeEquation& equation, CMRLatexEntity2& entity)
{
	if (entity.hasInfo("cmrNoTranform"))
		out << "ENTITY_NO_TRANSF [ ";
	else
		out << "ENTITY [ " << entity.getName() << " , ";
	CMRTransformationBasic::transform(equation, entity);
	out << " ]";
}

/*******************  FUNCTION  *********************/
void MockTransfBasic::transform(CMRProjectCodeEquation& equation)
{
	out << "EQUATION [ " << equation.getOutput() << " , ";
	CMRTransformationBasic::transform(equation);
	out << " ]";
}

/*******************  FUNCTION  *********************/
TEST(TestProjectTransfBasic,testConstructor)
{
	MockTransfBasic transf;
}

/*******************  FUNCTION  *********************/
TEST(TestProjectTransfBasic,testTransformLoop)
{
	MockTransfBasic transf;
	
	CMRProjectContext context;
	CMRProjectIterator it("k","testK",0,9);
	context.addEntry(&it);
	CMRProjectCodeNode root(&context);
	root.addIteratorLoop("k");
	
	transf.run(root);
	EXPECT_EQ("NODE[ LOOP[ k , NODE[  ] ] ]",transf.out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectTransfBasic,testTransformEquation_1)
{
	MockTransfBasic transf;
	
	CMRProjectContext context;
	CMRProjectIterator it("k","testK",0,9);
	context.addEntry(&it);
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("j","testJ","int","0");
	root.addEquation("j","k * 2");
	
	transf.run(root);
	EXPECT_EQ("NODE[ EQUATION [ j , FORMULA[ ENTITY [ k ,  ]ENTITY [ * ,  ]ENTITY [ 2 ,  ] ] ] ]",transf.out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectTransfBasic,testTransformEquation_2)
{
	MockTransfBasic transf;
	
	CMRProjectContext context;
	CMRProjectIterator it("k","testK",0,9);
	context.addEntry(&it);
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("j","testJ","int","0");
	root.addEquation("j","k^{4 + 5}_{9 + 10} * \\frac{1}{2}");
	
	transf.run(root);
	EXPECT_EQ("NODE[ EQUATION [ j , FORMULA[ ENTITY [ k , FORMULA[ ENTITY [ 9 ,  ]ENTITY [ + ,  ]ENTITY [ 10 ,  ] ]FORMULA[ ENTITY [ 4 ,  ]ENTITY [ + ,  ]ENTITY [ 5 ,  ] ] ]ENTITY [ * ,  ]ENTITY [ \\frac , FORMULA[ ENTITY [ 1 ,  ] ]FORMULA[ ENTITY [ 2 ,  ] ] ] ] ] ]",transf.out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectTransfBasic,testTransformEquation_3)
{
	MockTransfBasic transf;
	
	CMRProjectContext context;
	CMRProjectIterator it("k","testK",0,9);
	context.addEntry(&it);
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("j","testJ","int","0");
	root.addEquation("j","1 + \\frac{1}{2}");
	
	transf.run(root);
	EXPECT_EQ("NODE[ EQUATION [ j , FORMULA[ ENTITY [ 1 ,  ]ENTITY [ + ,  ]ENTITY [ \\frac , FORMULA[ ENTITY [ 1 ,  ] ]FORMULA[ ENTITY [ 2 ,  ] ] ] ] ] ]",transf.out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectTransfBasic,testTransformEquation_4)
{
	MockTransfBasic transf;
	
	CMRProjectContext context;
	CMRProjectIterator it("k","testK",0,9);
	context.addEntry(&it);
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("j","testJ","int","0");
	root.addEquation("j","1 +(2 * 3)");
	
	transf.run(root);
	EXPECT_EQ("NODE[ EQUATION [ j , FORMULA[ ENTITY [ 1 ,  ]ENTITY [ + ,  ]ENTITY [ () , FORMULA[ ENTITY [ 2 ,  ]ENTITY [ * ,  ]ENTITY [ 3 ,  ] ] ] ] ] ]",transf.out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestProjectTransfBasic,testGetName)
{
	MockTransfBasic transf;
	
	EXPECT_EQ("mock",transf.getName());
}
