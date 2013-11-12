/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cassert>
#include <iostream>
#include <sstream>
#include "CMRProjectTransformation.h"
#include "ProjectActionOld.h"
#include "parsor/TexParsor.h"
#include "parsor/LatexEntityOld.h"
#include "parsor/ParsorBasics.h"

using namespace std;
using namespace CMRCompiler;

/********************  NAMESPACE  *******************/
namespace CMRCompilerOld
{

/*******************  FUNCTION  *********************/
CMRProjectTransformation::CMRProjectTransformation ( bool loopOnLatexEntities)
{
	this->loopOnLatexEntities = loopOnLatexEntities;
}

/*******************  FUNCTION  *********************/
CMRProjectTransformation::~CMRProjectTransformation ( void )
{

}

/*******************  FUNCTION  *********************/
void CMRProjectTransformation::init ( void )
{

}

/*******************  FUNCTION  *********************/
void CMRProjectTransformation::finish ( void )
{

}

/*******************  FUNCTION  *********************/
void CMRProjectTransformation::run ( ProjectActionOld* rootAction )
{
	int depth ;
	int initDepth;

	//errors
	assert(rootAction != NULL);

	//init
	init();
	
	//loop until end
	ProjectActionOld::Iterator it(rootAction);
	
	//setup
	initDepth = depth = it->getDepth();
	
	//loop
	while(!it.isEnd() && depth >= initDepth)
	{
		ProjectActionOld::Iterator next = transform(it,depth);
		if (next != it) {
			it = next;
			depth = it->getDepth();
			assert(depth >= initDepth);
		} else if (it.hasChild()) {
			depth++;
			it.moveDown();
		} else if(it.hasNext()) {
			it.moveNext();
		} else {
			do {
				if (!it.isEnd() && depth >= initDepth)
					closeNode(it,depth);
				depth--;
				it.moveUp();
			} while(!it.hasNext() && !it.isEnd());
			it.moveNext();
		}
	}

	//finish
	finish();
}

/*******************  FUNCTION  *********************/
ProjectActionOld::Iterator CMRProjectTransformation::transform ( ProjectActionOld::Iterator action,int depth )
{
	return action;
}

/*******************  FUNCTION  *********************/
void CMRProjectTransformation::closeNode ( ProjectActionOld::Iterator , int depth )
{

}

/*******************  FUNCTION  *********************/
CMRProjectTransfExtractLoops::CMRProjectTransfExtractLoops ( void )
	:CMRProjectTransformation(false)
{

}

/*******************  FUNCTION  *********************/
void CMRProjectTransfExtractLoops::init ( void )
{
	this->tmpId = 0;
}

/*******************  FUNCTION  *********************/
static std::string getTempName(int id)
{
	stringstream tmp;
	tmp << "\\CMRTMP^"<< id <<"{" << id << "}";
	return tmp.str();
}

/*******************  FUNCTION  *********************/
static std::string getLongTempName(int id)
{
	stringstream tmp;
	tmp << "cmrTmpValue" << id;
	return tmp.str();
}

/*******************  FUNCTION  *********************/
ProjectActionOld::Iterator CMRProjectTransfExtractLoops::transform ( ProjectActionOld::Iterator action, int depth )
{
	LatexEntityOld * term;
	string op;

	if (action->getName() == "cmrEquation")
	{
		assert(action->hasEquation());
		term = action->getEquation().extractNextInnerLoop();
		if (term != NULL)
		{
			if (term->name == "\\sum")
				op = " + ";
			else
				assert(false);
			LatexFormulasOld f;
			string tmpName = getTempName(tmpId);
			string longTmpName = getLongTempName(tmpId);
			tmpId++;
			action->addEquation(tmpName,longTmpName,"0",CMR_INSERT_BEFORE);
			cout << "Replace loops with iterator (" << term->subscriptTotalValue << ") and core (" << term->params[0]->string << ")" << endl;
 			ProjectActionOld & ac = action->addIteratorLoop(term->subscriptTotalValue,CMR_INSERT_BEFORE);
			ac.addEquation(tmpName,longTmpName,string(tmpName) + op + term->params[0]->string);
			cmrParseLatexFormula(f,tmpName);
			*term = *f.childs[0];
			return &ac;
		}
	}
	
	return action;
}

/*******************  FUNCTION  *********************/
CMRProjectTransfImplicitMul::CMRProjectTransfImplicitMul ( void ) 
	: CMRProjectTransformation(false)
{

}

/*******************  FUNCTION  *********************/
LatexEntityOld * simpleEntity(const std::string & value)
{
	int tmp = 0;
	LatexEntityOld * e = cmrParseLatexEntitySimple(value,tmp);
	return e;
}

/*******************  FUNCTION  *********************/
void CMRProjectTransfImplicitMul::replaceImplMul ( LatexFormulasListOld& formula )
{
	for (LatexFormulasListOld::iterator it = formula.begin() ;  it != formula.end() ; ++it)
		replaceImplMul(**it);
}

/*******************  FUNCTION  *********************/
void CMRProjectTransfImplicitMul::replaceImplMul ( LatexFormulasOld & formula )
{
	int cnt = 0;
	LatexEntityVectorOld & elems = formula.childs;
	LatexEntityVectorOld newList;
	for (LatexEntityVectorOld::iterator it = elems.begin() ;  it != elems.end() ; ++it)
	{
		if (cnt % 2 == 1 && !cmrIsOperator((*it)->totalValue[0]))
		{
			newList.push_back(simpleEntity("*"));
			cnt++;
		}
		newList.push_back(*it);
		cnt++;
		
		//do for inner
		replaceImplMul((*it)->subscript);
		replaceImplMul((*it)->superscript);
		
		for (LatexFormulasListOld::iterator itf = (*it)->params.begin() ; itf != (*it)->params.end() ; ++itf)
			replaceImplMul(**itf);
	}
	
	if (newList.size() != elems.size())
		elems = newList;
}

/*******************  FUNCTION  *********************/
ProjectActionOld::Iterator CMRProjectTransfImplicitMul::transform ( ProjectActionOld::Iterator action, int depth )
{
	if (action->getName() == "cmrEquation")
	{
		assert(action->hasEquation());
		replaceImplMul(action->getEquation().formula);
	}
	return action;
}

/*******************  FUNCTION  *********************/
ProjectActionOld::Iterator CMRProjectTransfExpandFrac::transform ( ProjectActionOld::Iterator action, int depth )
{
	if (action->getName() == "cmrEquation")
	{
		assert(action->hasEquation());
		expandFrac(action->getEquation().formula);
	}
	return action;
}

/*******************  FUNCTION  *********************/
LatexEntityOld * securedEntity(LatexFormulasOld & formula)
{
	if (formula.childs.size() == 1)
	{
		return formula.childs[0];
	} else {
		LatexEntityOld * e = new LatexEntityOld;
		e->name = "(";
		e->params.push_back(&formula);
		return e;
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectTransfExpandFrac::expandFrac ( LatexEntityOld& entity )
{
	if (entity.name == "\\frac")
	{
		entity.name = "(";
		LatexFormulasOld * f = new LatexFormulasOld;
		f->childs.push_back(securedEntity(*entity.params[0]));
		f->childs.push_back(simpleEntity("/"));
		f->childs.push_back(securedEntity(*entity.params[1]));
		entity.params.clear();
		entity.params.push_back(f);
	}

	//do for inner
	expandFrac(entity.subscript);
	expandFrac(entity.superscript);
		
	for (LatexFormulasListOld::iterator itf = entity.params.begin() ; itf != entity.params.end() ; ++itf)
		expandFrac(**itf);
}

/*******************  FUNCTION  *********************/
void CMRProjectTransfExpandFrac::expandFrac ( LatexFormulasListOld& forumlas )
{
	for (LatexFormulasListOld::iterator it = forumlas.begin() ;  it != forumlas.end() ; ++it)
		expandFrac(**it);
}

/*******************  FUNCTION  *********************/
void CMRProjectTransfExpandFrac::expandFrac ( LatexFormulasOld& forumlas )
{
	LatexEntityVectorOld & elems = forumlas.childs;
	for (LatexEntityVectorOld::iterator it = elems.begin() ;  it != elems.end() ; ++it)
		expandFrac(**it);
}

/*******************  FUNCTION  *********************/
ProjectActionOld::Iterator CMRProjectTransfExpendExponent::transform ( ProjectActionOld::Iterator action, int depth )
{
	if (action->getName() == "cmrEquation")
	{
		assert(action->hasEquation());
		expandExponent(action->getEquation().formula,*action);
	}
	return action;
}

/*******************  FUNCTION  *********************/
void CMRProjectTransfExpendExponent::expandExponent ( LatexFormulasOld& formulas,ProjectActionOld & action)
{
	LatexEntityVectorOld & elems = formulas.childs;
	for (LatexEntityVectorOld::iterator it = elems.begin() ;  it != elems.end() ; ++it)
		expandExponent(**it,action);
}

/*******************  FUNCTION  *********************/
void CMRProjectTransfExpendExponent::expandExponent ( LatexEntityOld& entity ,ProjectActionOld & action)
{
	if (entity.name[0] == '\\' || entity.superscript.empty())
		return;

	CMRProjectCaptureMap capture;
	const CMRProjectEntity * e = action.getContext().find(entity);
	assert(e != NULL);
	e->match(entity,capture);
	if (capture.find("cmrExponent") != capture.end())
	{
		if (entity.superscript[0]->string == "2")
		{
			LatexEntityOld * e = new LatexEntityOld();
			*e = entity;
			entity.name = "(";
			LatexFormulasOld * f = new LatexFormulasOld;
			f->childs.push_back(e);
			f->childs.push_back(simpleEntity("*"));
			f->childs.push_back(e);
			entity.params.clear();
			entity.params.push_back(f);
		} else if (entity.superscript[0]->string == "3") {
			LatexEntityOld * e = new LatexEntityOld();
			*e = entity;
			entity.name = "(";
			LatexFormulasOld * f = new LatexFormulasOld;
			f->childs.push_back(e);
			f->childs.push_back(simpleEntity("*"));
			f->childs.push_back(e);
			f->childs.push_back(simpleEntity("*"));
			f->childs.push_back(e);
			entity.params.clear();
			entity.params.push_back(f);
		}
	}
}

}
