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
#include "CMRProjectAction.h"
#include "parsor/CMRTexParsor.h"
#include "parsor/CMRLatexEntity.h"
#include "parsor/CMRParsorBasics.h"

using namespace std;

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
void CMRProjectTransformation::run ( CMRProjectAction* rootAction )
{
	int depth ;
	int initDepth;

	//errors
	assert(rootAction != NULL);

	//init
	init();
	
	//loop until end
	CMRProjectAction::Iterator it(rootAction);
	
	//setup
	initDepth = depth = it->getDepth();
	
	//loop
	while(!it.isEnd() && depth >= initDepth)
	{
		CMRProjectAction::Iterator next = transform(it,depth);
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
CMRProjectAction::Iterator CMRProjectTransformation::transform ( CMRProjectAction::Iterator action,int depth )
{
	return action;
}

/*******************  FUNCTION  *********************/
void CMRProjectTransformation::closeNode ( CMRProjectAction::Iterator , int depth )
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
CMRProjectAction::Iterator CMRProjectTransfExtractLoops::transform ( CMRProjectAction::Iterator action, int depth )
{
	CMRLatexEntity * term;
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
			CMRLatexFormulas f;
			string tmpName = getTempName(tmpId);
			string longTmpName = getLongTempName(tmpId);
			tmpId++;
			action->addEquation(tmpName,longTmpName,"0",CMR_INSERT_BEFORE);
			cout << "Replace loops with iterator (" << term->subscriptTotalValue << ") and core (" << term->params[0]->string << ")" << endl;
 			CMRProjectAction & ac = action->addIteratorLoop(term->subscriptTotalValue,CMR_INSERT_BEFORE);
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
CMRLatexEntity * simpleEntity(const std::string & value)
{
	int tmp = 0;
	CMRLatexEntity * e = cmrParseLatexEntitySimple(value,tmp);
	return e;
}

/*******************  FUNCTION  *********************/
void CMRProjectTransfImplicitMul::replaceImplMul ( CMRLatexFormulasList& formula )
{
	for (CMRLatexFormulasList::iterator it = formula.begin() ;  it != formula.end() ; ++it)
		replaceImplMul(**it);
}

/*******************  FUNCTION  *********************/
void CMRProjectTransfImplicitMul::replaceImplMul ( CMRLatexFormulas & formula )
{
	int cnt = 0;
	CMRLatexEntityVector & elems = formula.childs;
	CMRLatexEntityVector newList;
	for (CMRLatexEntityVector::iterator it = elems.begin() ;  it != elems.end() ; ++it)
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
		
		for (CMRLatexFormulasList::iterator itf = (*it)->params.begin() ; itf != (*it)->params.end() ; ++itf)
			replaceImplMul(**itf);
	}
	
	if (newList.size() != elems.size())
		elems = newList;
}

/*******************  FUNCTION  *********************/
CMRProjectAction::Iterator CMRProjectTransfImplicitMul::transform ( CMRProjectAction::Iterator action, int depth )
{
	if (action->getName() == "cmrEquation")
	{
		assert(action->hasEquation());
		replaceImplMul(action->getEquation().formula);
	}
	return action;
}

/*******************  FUNCTION  *********************/
CMRProjectAction::Iterator CMRProjectTransfExpandFrac::transform ( CMRProjectAction::Iterator action, int depth )
{
	if (action->getName() == "cmrEquation")
	{
		assert(action->hasEquation());
		expandFrac(action->getEquation().formula);
	}
	return action;
}

/*******************  FUNCTION  *********************/
CMRLatexEntity * securedEntity(CMRLatexFormulas & formula)
{
	if (formula.childs.size() == 1)
	{
		return formula.childs[0];
	} else {
		CMRLatexEntity * e = new CMRLatexEntity;
		e->name = "(";
		e->params.push_back(&formula);
		return e;
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectTransfExpandFrac::expandFrac ( CMRLatexEntity& entity )
{
	if (entity.name == "\\frac")
	{
		entity.name = "(";
		CMRLatexFormulas * f = new CMRLatexFormulas;
		f->childs.push_back(securedEntity(*entity.params[0]));
		f->childs.push_back(simpleEntity("/"));
		f->childs.push_back(securedEntity(*entity.params[1]));
		entity.params.clear();
		entity.params.push_back(f);
	}

	//do for inner
	expandFrac(entity.subscript);
	expandFrac(entity.superscript);
		
	for (CMRLatexFormulasList::iterator itf = entity.params.begin() ; itf != entity.params.end() ; ++itf)
		expandFrac(**itf);
}

/*******************  FUNCTION  *********************/
void CMRProjectTransfExpandFrac::expandFrac ( CMRLatexFormulasList& forumlas )
{
	for (CMRLatexFormulasList::iterator it = forumlas.begin() ;  it != forumlas.end() ; ++it)
		expandFrac(**it);
}

/*******************  FUNCTION  *********************/
void CMRProjectTransfExpandFrac::expandFrac ( CMRLatexFormulas& forumlas )
{
	CMRLatexEntityVector & elems = forumlas.childs;
	for (CMRLatexEntityVector::iterator it = elems.begin() ;  it != elems.end() ; ++it)
		expandFrac(**it);
}

/*******************  FUNCTION  *********************/
CMRProjectAction::Iterator CMRProjectTransfExpendExponent::transform ( CMRProjectAction::Iterator action, int depth )
{
	if (action->getName() == "cmrEquation")
	{
		assert(action->hasEquation());
		expandExponent(action->getEquation().formula,*action);
	}
	return action;
}

/*******************  FUNCTION  *********************/
void CMRProjectTransfExpendExponent::expandExponent ( CMRLatexFormulas& formulas,CMRProjectAction & action)
{
	CMRLatexEntityVector & elems = formulas.childs;
	for (CMRLatexEntityVector::iterator it = elems.begin() ;  it != elems.end() ; ++it)
		expandExponent(**it,action);
}

/*******************  FUNCTION  *********************/
void CMRProjectTransfExpendExponent::expandExponent ( CMRLatexEntity& entity ,CMRProjectAction & action)
{
	if (entity.name[0] == '\\' || entity.superscript.empty())
		return;

	CMRIndiceCaptureMap capture;
	const CMREntity * e = action.getContext().find(entity);
	assert(e != NULL);
	e->match(entity,capture);
	if (capture.find("cmrExponent") != capture.end())
	{
		if (entity.superscript[0]->string == "2")
		{
			CMRLatexEntity * e = new CMRLatexEntity();
			*e = entity;
			entity.name = "(";
			CMRLatexFormulas * f = new CMRLatexFormulas;
			f->childs.push_back(e);
			f->childs.push_back(simpleEntity("*"));
			f->childs.push_back(e);
			entity.params.clear();
			entity.params.push_back(f);
		} else if (entity.superscript[0]->string == "3") {
			CMRLatexEntity * e = new CMRLatexEntity();
			*e = entity;
			entity.name = "(";
			CMRLatexFormulas * f = new CMRLatexFormulas;
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
