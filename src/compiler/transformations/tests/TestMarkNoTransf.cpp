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
#include "MockTransfBasic.h"

/**********************  USING  *********************/
using namespace testing;
using namespace CMRCompiler;

/*********************  CONSTS  *********************/

/*******************  FUNCTION  *********************/
TEST(TestMarkNoTransf,testConstructor)
{
	CMRTransformationMarkNoTransf transf;
}

/*******************  FUNCTION  *********************/
TEST(TestMarkNoTransf,testRun_indice)
{
	CMRTransformationMarkNoTransf transf;
	MockTransfBasic ptransf("cmrNoTranform","NO_TRANSF");
	MockTransfBasic ptransf2("cmrNoTranform","NO_TRANSF");
	
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
	MockTransfBasic ptransf("cmrNoTranform","NO_TRANSF");
	MockTransfBasic ptransf2("cmrNoTranform","NO_TRANSF");
	
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
	MockTransfBasic ptransf("cmrNoTranform","NO_TRANSF");
	MockTransfBasic ptransf2("cmrNoTranform","NO_TRANSF");
	
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
