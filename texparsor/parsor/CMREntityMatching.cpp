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
#include <cassert>
#include <cstdlib>
#include "CMREntityMatching.h"
#include "CMRParsorBasics.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
CMREntityCellMatching::CMREntityCellMatching ( bool matching )
{
	this->match = matching;
	this->depInfo = false;
	this->dx = 0;
	this->dy = 0;
	this->write = false;
	this->out = false;
}

/*******************  FUNCTION  *********************/
void CMREntityCellMatching::printDebug ( void ) const
{
	printf(" - CellMatching : %d\n",match);
	if (depInfo)
		printf("      * vect    : [ %d , %d ]\n",dx,dy);
	else
		printf("      * vect    : [ ?? , ?? ]\n");
	printf("      * write   : %d\n",write);
	printf("      * out     : %d\n",out);
}

/*******************  FUNCTION  *********************/
bool cmrEntityExactMatch(const CMRLatexEntity & e1,const CMRLatexEntity & e2,bool subsup)
{
	if (subsup)
		return (e1.name == e2.name && e1.subscriptTotalValue == e2.subscriptTotalValue && e1.superscriptTotalValue == e2.superscriptTotalValue);
	else
		return (e1.name == e2.name);
}

/*******************  FUNCTION  *********************/
const CMRLatexFormulas * cmrEntityExtractSubscriptParam(const CMRLatexEntity & entity,int id)
{
	if (entity.subscript.empty() || entity.subscript.size() <= id)
		return NULL;
	
	return entity.subscript[id];
}

/*******************  FUNCTION  *********************/
int cmrApplyOp(int orig,char op,int value)
{
	switch(op)
	{
		case '+':
			orig+=value;
			break;
		case '-':
			orig-=value;
			break;
		default:
			assert(false);
	}
	
	return orig;
}

/*******************  FUNCTION  *********************/
int cmrFormulaExtractDelta(const CMRLatexFormulas * formula,const string & varname)
{
	int res = 0;
	char op = '+';
	for (CMRLatexEntityVector::const_iterator it = formula->childs.begin() ; it != formula->childs.end() ; ++it)
	{
		const string & name = (*it)->name;
		if (name == varname)
		{
			assert(op != ' ');
			op = ' ';
		} else if (cmrIsSimpleOperator(name)) {
			assert(op == ' ' || (op == '+' && name == "-" ));
			op = name[0];
		} else if (cmrIsNum(name[0])) {
			res = cmrApplyOp(res,op,atoi(name.c_str()));
		} else {
			assert(false);
		}
	}
	return res;
}

/*******************  FUNCTION  *********************/
CMREntityCellMatching cmrEntityCellMatch(const CMRLatexEntity & entity,const string & name)
{
	CMREntityCellMatching res;
	const CMRLatexFormulas * fdx;
	const CMRLatexFormulas * fdy;
	
	if (entity.name == name)
	{
		res.match = true;
	} else if (entity.name == name + "'") {
		res.match = true;
		res.write = true;
		res.out = true;
	} else {
		return res;
	}

	//check deps
	fdx = cmrEntityExtractSubscriptParam(entity,0);
	fdy = cmrEntityExtractSubscriptParam(entity,1);
	assert(fdx != NULL);
	assert(fdy != NULL);
	
	res.dx = cmrFormulaExtractDelta(fdx,"i");
	res.dy = cmrFormulaExtractDelta(fdy,"j");
	res.depInfo = true;

	return res;
}

/*******************  FUNCTION  *********************/
void cmrExtractDeps(CMREqDepMatrix & matrix,const CMRLatexFormulasList & f,const string & varname)
{
	for (CMRLatexFormulasList::const_iterator it = f.begin() ; it != f.end() ; ++it)
		cmrExtractDeps(matrix,**it,varname);
}

/*******************  FUNCTION  *********************/
void cmrExtractDeps(CMREqDepMatrix & matrix,const CMRLatexEntity & f,const string & varname)
{
	CMREntityCellMatching match = cmrEntityCellMatch(f,varname);
	if (match.match)
	{
		printf("=> Check %s : %s\n",f.name.c_str(),f.totalValue.c_str());
		match.printDebug();
		matrix.markAccess(match.dx,match.dy,match.write);
	}
	//loop on sublists
	cmrExtractDeps(matrix,f.subscript,varname);
	cmrExtractDeps(matrix,f.superscript,varname);
	for (CMRLatexFormulasList::const_iterator it = f.params.begin() ; it != f.params.end() ; ++it)
		cmrExtractDeps(matrix,**it,varname);
}

/*******************  FUNCTION  *********************/
void cmrExtractDeps(CMREqDepMatrix & matrix,const CMRLatexFormulas & f,const string & varname)
{
	for (CMRLatexEntityVector::const_iterator it = f.childs.begin() ; it != f.childs.end() ; ++it)
		cmrExtractDeps(matrix,**it,varname);
}
