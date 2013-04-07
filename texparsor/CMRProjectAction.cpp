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
#include <cstdlib>
#include <sstream>
#include "CMRProjectAction.h"
#include "CMRProject.h"
#include "parsor/CMRTexParsor.h"
#include "parsor/CMRParsorBasics.h"
#include "CMRProjectIterator.h"
#include "CMRGenCode.h"

using namespace std;

/*******************  FUNCTION  *********************/
CMRProjectAction::CMRProjectAction ( string name, string descr , CMRProjectContext  * parentContext)
	:context(parentContext)
{
	this->name = name;
	this->description = descr;
	this->eq = NULL;
}

/*******************  FUNCTION  *********************/
CMRProjectEquation& CMRProjectAction::addEquation ( const string& latexName, const string& longName, const string& compute,CMRProjectCodeTreeInsert location )
{
	CMRProjectAction * tmpBlock = new CMRProjectAction("cmrEquation",latexName);
	CMRProjectEquation * tmp = tmpBlock->eq = new CMRProjectEquation(latexName,longName,compute);
	insertAction(tmpBlock,location);
	
	if (context.find(tmp->latexEntity) == NULL)
		addContextEntry(tmp,location);
	
	return *tmp;
}

/*******************  FUNCTION  *********************/
CMRProjectAction& CMRProjectAction::addSubBlock ( string loopDescr, string parameter,CMRProjectCodeTreeInsert location )
{
	CMRProjectAction * tmpBlock = new CMRProjectAction("cmrSubBlock",loopDescr);
	tmpBlock->eq = new CMRProjectEquation(parameter,"cmrIndice",parameter);
	insertAction(tmpBlock,location);
	return *tmpBlock;
}

/*******************  FUNCTION  *********************/
CMRProjectAction& CMRProjectAction::addIteratorLoop ( const string& iterator ,CMRProjectCodeTreeInsert location)
{
	CMRProjectAction & ac = this->addSubBlock("cmrIteratorLoop",iterator,location);
	ac.context.addEntry(new CMRProjectLocalVariable(iterator,iterator));
	return ac;
}

/*******************  FUNCTION  *********************/
void CMRProjectAction::addContextEntry ( CMREntity* entity )
{
	context.addEntry(entity);
}

/*******************  FUNCTION  *********************/
std::string getTempName(int id)
{
	stringstream tmp;
	tmp << "\\CMRTMP^"<< id <<"{" << id << "}";
	return tmp.str();
}

/*******************  FUNCTION  *********************/
std::string getLongTempName(int id)
{
	stringstream tmp;
	tmp << "cmrTmpValue" << id;
	return tmp.str();
}

/*******************  FUNCTION  *********************/
void CMRProjectAction::printDebug(int depth) const
{
	for (int i = 0 ; i < depth ; i++)
			cout << "\t";
	if (name == "cmrEquation")
	{
		cout << this->eq->latexName << " = " << this->eq->compute << endl;
	} else {
		if (eq != NULL)
			cout << name << " - " << description << " - " << eq->compute << ":" << endl;
		else
			cout << name << " - " << description << ":" << endl;
		for (ConstIterator it = getFirstChild() ; ! it.isEnd() ; ++it)
			it->printDebug(depth+1);
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectAction::insertAction(CMRProjectAction* action, CMRProjectCodeTreeInsert location)
{
	insert(action,location);
	
	Iterator it(this);

	switch(location)
	{
		case CMR_INSERT_AFTER:
		case CMR_INSERT_BEFORE:
		case  CMR_INSERT_FIRST:
		case  CMR_INSERT_LAST:
			assert(it.hasParent());
			it.moveUp();
			action->context.parent = &it->context;
			break;
		case CMR_INSERT_FIRST_CHILD:
		case CMR_INSERT_LAST_CHILD:
			action->context.parent = &this->context;
			break;
		default:
			assert(false);
			abort();
	}
}

/*******************  FUNCTION  *********************/
std::string genCCodeIndent(int depth)
{
	std::string tmp;
	//indent
	for (int i = 0 ; i < depth ; i++)
		tmp += "\t";

	return tmp;
}

/*******************  FUNCTION  *********************/
void CMRProjectAction::checkContext ( CMRProjectContext& context ) const
{
	ConstIterator it(this);
	const CMRProjectContext * cur = &this->context;
	
	do {
		assert(cur == &it->context);
		it.moveUp();
		cur = cur->parent;
	} while (it.hasParent());
	
	if (context.countTotalEntries() != this->context.countTotalEntries())
	{
		cerr << endl << "!!!!!!!!!!!!!!! CONTEXT ERROR !!!!!!!!!!!!!!!!" << endl;
		cerr << "Correct :" << endl;
		context.printDebug();
		cerr << "------------------------------" << endl;
		cerr << "Local :" << endl;
		this->context.printDebug();
		//abort();
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectAction::genEqCCode(ostream& out, CMRProjectContext& context, int depth) const
{
	//errors
	assert(eq != NULL);
	//check if in context of if new def
	CMREntity * entity = context.find(eq->latexEntity);
	
	//indent
	out << genCCodeIndent(depth);
	
	//if new def, need to insert declaration code
	if (entity == NULL)
	{
		out << "double ";
		entity = & context.addEntry(new CMRProjectLocalVariable(eq->latexName,eq->longName));
	}
	
	//write left member
	entity->genUsageCCode(out,context,eq->latexEntity);
	out << " = ";
	
	//loop on body elements
	cmrGenEqCCode(out,context,eq->formula);
	
	out << ";" << endl;
}

/*******************  FUNCTION  *********************/
void CMRProjectAction::addContextEntry ( CMREntity* entity, CMRProjectCodeTreeInsert location )
{
	Iterator it(this);

	switch(location)
	{
		case CMR_INSERT_AFTER:
		case CMR_INSERT_BEFORE:
		case  CMR_INSERT_FIRST:
		case  CMR_INSERT_LAST:
			assert(it.hasParent());
			it.moveUp();
			it->addContextEntry(entity);
			break;
		case CMR_INSERT_FIRST_CHILD:
		case CMR_INSERT_LAST_CHILD:
			addContextEntry(entity);
			break;
		default:
			assert(false);
			abort();
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectAction::genItLoopCCode ( ostream& out, CMRProjectContext& context, int depth ) const
{
	CMRProjectContext localContext(&context);

	//errors
	assert(name == "cmrSubBlock" && description == "cmrIteratorLoop");
	
	//search the related iterator definition
	CMREntity * entity = localContext.find(eq->latexEntity);
	if (entity == NULL)
	{
		cerr << "Can't find the definition of iterator " << eq->compute << " in current context." << endl;
		abort();
	}

	CMRProjectIterator * iterator = dynamic_cast<CMRProjectIterator*>(entity);
	if (iterator == NULL)
	{
		cerr << "Cuation, expect iterator " << eq->compute << " but get another type." << endl;
		abort();
	}
	assert(iterator != NULL);
	
	CMRProjectLocalVariable localVar(eq->compute,eq->compute);
	localContext.addEntry(&localVar);
	out << genCCodeIndent(depth) << "for (int " << eq->compute << " = " << iterator->start << " ; " << eq->compute << " < " << iterator->end << " ; " << eq->compute << "++ )" <<endl;
	out << genCCodeIndent(depth) << "{" << endl;
	for (ConstIterator it = getFirstChild() ; ! it.isEnd() ; ++it)
		it->genCCode(out,localContext,depth+1);
	out << genCCodeIndent(depth) << "}" << endl;
	
	//checkContext(localContext);
}

/*******************  FUNCTION  *********************/
void CMRProjectAction::genRootElemCCode ( ostream& out, CMRProjectContext& context, int depth ) const
{
	CMRProjectContext localContext(&context);
	for (ConstIterator it = getFirstChild() ; ! it.isEnd() ; ++it)
		it->genCCode(out,localContext,depth+1);
	//checkContext(localContext);
}

/*******************  FUNCTION  *********************/
void CMRProjectAction::genCCode(std::ostream & out,CMRProjectContext & context,int depth) const
{
	//cases
	if (depth <= 1)
	{
		genRootElemCCode(out,context,depth);
	}else if (name == "cmrMainLoop" && description == "cmrMainLoop") {
		genRootElemCCode(out,context,depth);
	} else if (name == "cmrSubBlock" && description == "cmrIteratorLoop") {
		genItLoopCCode(out,context,depth);
	} else if (name == "cmrEquation") {
		genEqCCode(out,context,depth);
	} else {
		out << genCCodeIndent(depth) << "//Unknown member : " << name << " : " << description;
		if (eq != NULL)
			out << " : " << description << " = " << eq->compute;
		out << endl;
	}
}
