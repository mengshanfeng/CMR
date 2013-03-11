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
#include "CMRLatexEntity.h"

/*******************  FUNCTION  *********************/
CMRLatexEntity::CMRLatexEntity ( void )
{
	this->from = 0;
	this->to = 0;
}

/*******************  FUNCTION  *********************/
CMRLatexEntity::~CMRLatexEntity ( void )
{
	for(CMRLatexFormulas::iterator it = subscript.begin();it!=subscript.end();++it)
		delete *it;
	for(CMRLatexFormulas::iterator it = superscript.begin();it!=superscript.end();++it)
		delete *it;
	for(CMRLatexFormulasList::iterator it = params.begin();it!=params.end();++it)
		delete *it;
}

/*******************  FUNCTION  *********************/
void cmrPrintFormula(const CMRLatexFormulas & formula,int depth)
{
	int pos = 0;
	for(CMRLatexFormulas::const_iterator it = formula.begin();it!=formula.end();++it)
		(*it)->print(depth,pos++);
}

/*******************  FUNCTION  *********************/
void CMRLatexEntity::print ( int depth, int pos )
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
	for(CMRLatexFormulasList::const_iterator it = params.begin();it!=params.end();++it)
		cmrPrintFormula(**it,depth+1);
}