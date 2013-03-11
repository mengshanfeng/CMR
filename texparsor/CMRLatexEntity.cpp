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
	for(CMRLatexEntityVector::iterator it = subscript.childs.begin();it!=subscript.childs.end();++it)
		delete *it;
	for(CMRLatexEntityVector::iterator it = superscript.childs.begin();it!=superscript.childs.end();++it)
		delete *it;
	for(CMRLatexFormulasList::iterator it = params.begin();it!=params.end();++it)
		delete *it;
}

/*******************  FUNCTION  *********************/
void cmrPrintFormula(const CMRLatexFormulas & formula,int depth)
{
	int pos = 0;
	for(CMRLatexEntityVector::const_iterator it = formula.childs.begin();it!=formula.childs.end();++it)
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

/*******************  FUNCTION  *********************/
std::string CMRLatexEntity::getString ( void ) const
{
	return parent.substr(from,to-from);
}

/*******************  FUNCTION  *********************/
int CMRLatexEntity::countIndices ( void ) const
{
	if (subscript.childs.empty())
		return 0;
	else if (subscript.childs[0]->name == "\\COMMA_GROUP")
		return subscript.childs[0]->params.size();
	else
		return 1;
}

/*******************  FUNCTION  *********************/
CMRLatexFormulasList CMRLatexEntity::getIndices ( void )
{
	CMRLatexFormulasList tmp;
	if (subscript.childs.empty())
	{
		return tmp;
	} else if (subscript.childs[0]->name == "\\COMMA_GROUP") {
		return subscript.childs[0]->params;
	} else {
		tmp.push_back(&subscript);
		return tmp;
	}
}
