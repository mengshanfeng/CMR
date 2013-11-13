/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include "../CMRTransformationMarkNoTransf.h"
#include <../definitions/CMRProjectCode.h>
#include <../definitions/ProjectIterator.h>
#include <../definitions/ProjectConstant.h>

/**********************  USING  *********************/
using namespace testing;
using namespace CMRCompiler;

/*********************  CONSTS  *********************/

/*********************  CLASS  **********************/
class MockTransfBasic : public CMRTransformationBasic
{
	public:
		MockTransfBasic(void);
		virtual void transform(CMRProjectCodeNode& node);
		virtual void transform(CMRProjectCodeIteratorLoop& loop);
		virtual void transform(CMRProjectCodeEquation& equation, LatexFormulas& formula);
		virtual void transform(CMRProjectCodeEquation& equation, LatexEntity& entity);
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
void MockTransfBasic::transform(CMRProjectCodeEquation& equation, LatexFormulas& formula)
{
	if (formula.hasInfo("cmrNoTranform"))
		out << "FORMULA_NO_TRANSF [ ";
	else
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
void MockTransfBasic::transform(CMRProjectCodeEquation& equation, LatexEntity& entity)
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
TEST(TestMarkNoTransf,testConstructor)
{
	CMRTransformationMarkNoTransf transf;
}

/*******************  FUNCTION  *********************/
TEST(TestMarkNoTransf,testRun_indice)
{
	CMRTransformationMarkNoTransf transf;
	MockTransfBasic ptransf;
	MockTransfBasic ptransf2;
	
	ProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("p_{bc}","testP","int","0");
	root.addLocalVariable("a","testJ","int","0");
	root.addEquation("a","p_{bc} + 4");
	
	ptransf.run(root);
	EXPECT_EQ("NODE[ EQUATION [ a , FORMULA[ ENTITY [ p , FORMULA[ ENTITY [ b ,  ]ENTITY [ c ,  ] ] ]ENTITY [ + ,  ]ENTITY [ 4 ,  ] ] ] ]",ptransf.out.str());

	transf.run(root);
	ptransf2.run(root);
	EXPECT_EQ("NODE[ EQUATION [ a , FORMULA[ ENTITY [ p ,  ]ENTITY [ + ,  ]ENTITY [ 4 ,  ] ] ] ]",ptransf2.out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestMarkNoTransf,testRun_expo)
{
	CMRTransformationMarkNoTransf transf;
	MockTransfBasic ptransf;
	MockTransfBasic ptransf2;
	
	ProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("p^{bc}","testP","int","0");
	root.addLocalVariable("a","testJ","int","0");
	root.addEquation("a","p^{bc} + 4");
	
	ptransf.run(root);
	EXPECT_EQ("NODE[ EQUATION [ a , FORMULA[ ENTITY [ p , FORMULA[ ENTITY [ b ,  ]ENTITY [ c ,  ] ] ]ENTITY [ + ,  ]ENTITY [ 4 ,  ] ] ] ]",ptransf.out.str());

	transf.run(root);
	ptransf2.run(root);
	EXPECT_EQ("NODE[ EQUATION [ a , FORMULA[ ENTITY [ p ,  ]ENTITY [ + ,  ]ENTITY [ 4 ,  ] ] ] ]",ptransf2.out.str());
}

/*******************  FUNCTION  *********************/
TEST(TestMarkNoTransf,testRun_param)
{
	CMRTransformationMarkNoTransf transf;
	MockTransfBasic ptransf;
	MockTransfBasic ptransf2;
	
	ProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("\\frac{bc}{cd}","testP","int","0");
	root.addLocalVariable("a","testJ","int","0");
	root.addEquation("a","\\frac{bc}{cd} + 4");
	
	ptransf.run(root);
	EXPECT_EQ("NODE[ EQUATION [ a , FORMULA[ ENTITY [ \\frac , FORMULA[ ENTITY [ b ,  ]ENTITY [ c ,  ] ]FORMULA[ ENTITY [ c ,  ]ENTITY [ d ,  ] ] ]ENTITY [ + ,  ]ENTITY [ 4 ,  ] ] ] ]",ptransf.out.str());

	transf.run(root);
	ptransf2.run(root);
	EXPECT_EQ("NODE[ EQUATION [ a , FORMULA[ ENTITY [ \\frac ,  ]ENTITY [ + ,  ]ENTITY [ 4 ,  ] ] ] ]",ptransf2.out.str());
}
