/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <svUnitTest.h>
#include "../CMRTransformationMarkNoTransf.h"
#include <../definitions/CMRProjectCode.h>
#include <../definitions/CMRProjectIterator.h>
#include <../definitions/CMRProjectConstant.h>

/**********************  USING  *********************/
using namespace svUnitTest;

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
SVUT_DECLARE_FLAT_TEST(TestProjectTransfBasic,testConstructor)
{
	CMRTransformationMarkNoTransf transf;
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectTransfBasic,testTransformLoop_indice)
{
	CMRTransformationMarkNoTransf transf;
	MockTransfBasic ptransf;
	MockTransfBasic ptransf2;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("p_{bc}","testP","int","0");
	root.addLocalVariable("a","testJ","int","0");
	root.addEquation("a","p_{bc} + 4");
	
	ptransf.run(root);
	SVUT_ASSERT_EQUAL("NODE[ EQUATION [ a , FORMULA[ ENTITY [ p , FORMULA[ ENTITY [ b ,  ]ENTITY [ c ,  ] ] ]ENTITY [ + ,  ]ENTITY [ 4 ,  ] ] ] ]",ptransf.out.str());

	transf.run(root);
	ptransf2.run(root);
	SVUT_ASSERT_EQUAL("NODE[ EQUATION [ a , FORMULA[ ENTITY [ p ,  ]ENTITY [ + ,  ]ENTITY [ 4 ,  ] ] ] ]",ptransf2.out.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectTransfBasic,testTransformLoop_expo)
{
	CMRTransformationMarkNoTransf transf;
	MockTransfBasic ptransf;
	MockTransfBasic ptransf2;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("p^{bc}","testP","int","0");
	root.addLocalVariable("a","testJ","int","0");
	root.addEquation("a","p^{bc} + 4");
	
	ptransf.run(root);
	SVUT_ASSERT_EQUAL("NODE[ EQUATION [ a , FORMULA[ ENTITY [ p , FORMULA[ ENTITY [ b ,  ]ENTITY [ c ,  ] ] ]ENTITY [ + ,  ]ENTITY [ 4 ,  ] ] ] ]",ptransf.out.str());

	transf.run(root);
	ptransf2.run(root);
	SVUT_ASSERT_EQUAL("NODE[ EQUATION [ a , FORMULA[ ENTITY [ p ,  ]ENTITY [ + ,  ]ENTITY [ 4 ,  ] ] ] ]",ptransf2.out.str());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectTransfBasic,testTransformLoop_param)
{
	CMRTransformationMarkNoTransf transf;
	MockTransfBasic ptransf;
	MockTransfBasic ptransf2;
	
	CMRProjectContext context;
	CMRProjectCodeNode root(&context);
	root.addLocalVariable("\\frac{bc}{cd}","testP","int","0");
	root.addLocalVariable("a","testJ","int","0");
	root.addEquation("a","\\frac{bc}{cd} + 4");
	
	ptransf.run(root);
	SVUT_ASSERT_EQUAL("NODE[ EQUATION [ a , FORMULA[ ENTITY [ \\frac , FORMULA[ ENTITY [ b ,  ]ENTITY [ c ,  ] ]FORMULA[ ENTITY [ c ,  ]ENTITY [ d ,  ] ] ]ENTITY [ + ,  ]ENTITY [ 4 ,  ] ] ] ]",ptransf.out.str());

	transf.run(root);
	ptransf2.run(root);
	SVUT_ASSERT_EQUAL("NODE[ EQUATION [ a , FORMULA[ ENTITY [ \\frac ,  ]ENTITY [ + ,  ]ENTITY [ 4 ,  ] ] ] ]",ptransf2.out.str());
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
