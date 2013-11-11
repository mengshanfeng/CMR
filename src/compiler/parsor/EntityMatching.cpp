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
#include "EntityMatching.h"
#include "ParsorBasics.h"

/**********************  USING  *********************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
EntityCellMatching::EntityCellMatching ( bool matching )
{
	this->match = matching;
	this->depInfo = false;
	this->dx = 0;
	this->dy = 0;
	this->write = false;
	this->out = false;
}

/*******************  FUNCTION  *********************/
void EntityCellMatching::printDebug ( void ) const
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
bool cmrEntityExactMatch(const LatexEntityOld & e1,const LatexEntityOld & e2,bool subsup)
{
	if (subsup)
		return (e1.name == e2.name && e1.subscriptTotalValue == e2.subscriptTotalValue && e1.superscriptTotalValue == e2.superscriptTotalValue);
	else
		return (e1.name == e2.name);
}

/*******************  FUNCTION  *********************/
const LatexFormulasOld * cmrEntityExtractSubscriptParam(const LatexEntityOld & entity,int id)
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
int cmrFormulaExtractDelta(const LatexFormulasOld * formula,const string & varname)
{
	int res = 0;
	char op = '+';
	for (LatexEntityVectorOld::const_iterator it = formula->childs.begin() ; it != formula->childs.end() ; ++it)
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
EntityCellMatching cmrEntityCellMatch(const LatexEntityOld & entity,const string & name)
{
	EntityCellMatching res;
	const LatexFormulasOld * fdx;
	const LatexFormulasOld * fdy;
	
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
void cmrExtractDeps(EqDepMatrix & matrix,const LatexFormulasListOld & f,const string & varname)
{
	for (LatexFormulasListOld::const_iterator it = f.begin() ; it != f.end() ; ++it)
		cmrExtractDeps(matrix,**it,varname);
}

/*******************  FUNCTION  *********************/
void cmrExtractDeps(EqDepMatrix & matrix,const LatexEntityOld & f,const string & varname)
{
	EntityCellMatching match = cmrEntityCellMatch(f,varname);
	if (match.match)
	{
		printf("=> Check %s : %s\n",f.name.c_str(),f.totalValue.c_str());
		match.printDebug();
		matrix.markAccess(match.dx,match.dy,match.write);
	}
	//loop on sublists
	cmrExtractDeps(matrix,f.subscript,varname);
	cmrExtractDeps(matrix,f.superscript,varname);
	for (LatexFormulasListOld::const_iterator it = f.params.begin() ; it != f.params.end() ; ++it)
		cmrExtractDeps(matrix,**it,varname);
}

/*******************  FUNCTION  *********************/
void cmrExtractDeps(EqDepMatrix & matrix,const LatexFormulasOld & f,const string & varname)
{
	for (LatexEntityVectorOld::const_iterator it = f.childs.begin() ; it != f.childs.end() ; ++it)
		cmrExtractDeps(matrix,**it,varname);
}

};
