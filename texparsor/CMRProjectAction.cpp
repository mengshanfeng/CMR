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
CMRProjectAction::CMRProjectAction ( string name, string descr )
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
	return ac;
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
void CMRProjectAction::genItLoopCCode ( ostream& out, CMRProjectContext& context, int depth ) const
{
	CMRProjectContext localContext(&context);

	//errors
	assert(name == "cmrSubBlock" && description == "cmrIteratorLoop");
	
	//search the related iterator definition
	CMREntity * entity = context.find(eq->latexEntity);
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
}

/*******************  FUNCTION  *********************/
void CMRProjectAction::genRootElemCCode ( ostream& out, CMRProjectContext& context, int depth ) const
{
	CMRProjectContext localContext(&context);
	for (ConstIterator it = getFirstChild() ; ! it.isEnd() ; ++it)
		it->genCCode(out,localContext,depth+1);
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
