/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cstdio>
// #include <cstdlib>
#include "LatexEntityOld.h"
// #include "CMRTexParsor.h"
// #include "CMRParsorBasics.h"
// #include "CMRLatexParsorContext.h"

/**********************  USING  *********************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
LatexEntityOld::LatexEntityOld ( void )
{
	this->from = 0;
	this->to = 0;
}

/*******************  FUNCTION  *********************/
LatexEntityOld::~LatexEntityOld ( void )
{
// 	for(CMRLatexEntityVector::iterator it = subscript.childs.begin();it!=subscript.childs.end();++it)
// 		delete *it;
// 	for(CMRLatexEntityVector::iterator it = superscript.childs.begin();it!=superscript.childs.end();++it)
// 		delete *it;
// 	for(CMRLatexFormulasList::iterator it = params.begin();it!=params.end();++it)
// 		delete *it;
}

/*******************  FUNCTION  *********************/
void cmrPrintFormula(const LatexFormulasListOld & formulas,int depth)
{
	for(LatexFormulasListOld::const_iterator it = formulas.begin();it!=formulas.end();++it)
	{
		if (it != formulas.begin())
			printf(",");
		cmrPrintFormula(**it,depth);
	}
}

/*******************  FUNCTION  *********************/
void cmrPrintFormula(const LatexFormulasOld & formula,int depth)
{
	int pos = 0;
	for(LatexEntityVectorOld::const_iterator it = formula.childs.begin();it!=formula.childs.end();++it)
		(*it)->print(depth,pos++);
}

/*******************  FUNCTION  *********************/
void LatexEntityOld::print ( int depth, int pos )
{
	printf(" - Entity [depth = %d, pos = %d]\n",depth,pos);
	printf("       * name        : %s\n",name.c_str());
	printf("       * totalValue  : %s\n",totalValue.c_str());
	printf("       * pos         : [%d - %d]\n",from,to);
	printf("       * subscript   : %s\n",subscriptTotalValue.c_str());
	printf("       * superscript : %s\n",superscriptTotalValue.c_str());
	printf("       * args        : %s\n",argsTotalValue.c_str());
	printf("       * parent      : %s\n",parent.c_str());
	cmrPrintFormula(subscript,depth+1);
	cmrPrintFormula(superscript,depth+1);
	for(LatexFormulasListOld::const_iterator it = params.begin();it!=params.end();++it)
		cmrPrintFormula(**it,depth+1);
}

/*******************  FUNCTION  *********************/
std::string LatexEntityOld::getString ( void ) const
{
	return parent.substr(from,to-from);
}

/*******************  FUNCTION  *********************/
int LatexEntityOld::countIndices ( void ) const
{
	return subscript.size();
}

/*******************  FUNCTION  *********************/
LatexFormulasListOld LatexEntityOld::getIndices ( void )
{
	return subscript;
}

}
