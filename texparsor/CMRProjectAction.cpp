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
	CMRProjectEquation * tmp = tmpBlock->eq = new CMRProjectEquation(parameter,"cmrIndice",parameter);
	insertAction(tmpBlock,location);
	return *tmpBlock;
}

/*******************  FUNCTION  *********************/
std::string getTempName(int * tmpId)
{
	int id = (*tmpId)++;
	stringstream tmp;
	tmp << "\\CMRTMP{" << id << "}";
	return tmp.str();
}

/*******************  FUNCTION  *********************/
void CMRProjectAction::replaceLoops(int* tmpId)
{
	CMRLatexEntity * term;
	string op;
	assert(tmpId != NULL);

	if (name == "cmrEquation")
	{
		assert(eq != NULL);
		//assert(parent != NULL);
		while ((term = eq->extractNextInnerLoop()) != NULL)
		{
			if (term->name == "\\sum")
				op = " + ";
			else
				assert(false);
			CMRLatexFormulas f;
			string tmpName = getTempName(tmpId);
			this->addEquation(tmpName,"cmrTmpValue1","0",CMR_INSERT_BEFORE);
			cout << "Replace loops with iterator (" << term->subscriptTotalValue << ") and core (" << term->params[0]->string << ")" << endl;
			CMRProjectAction & ac = this->addSubBlock("cmrLoop",term->subscriptTotalValue,CMR_INSERT_BEFORE);
			ac.addEquation(tmpName,"cmrTmpValue1",string(tmpName) + op + term->params[0]->string);
			cmrParseLatexFormula(f,tmpName);
			*term = *f.childs[0];
			ac.replaceLoops(tmpId);
		}
	} else {
		for (Iterator it = getFirstChild() ; ! it.isEnd() ; ++it)
			it->replaceLoops(tmpId);
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectAction::printDebug(int depth)
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
		for (Iterator it = getFirstChild() ; ! it.isEnd() ; ++it)
			it->printDebug(depth+1);
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectAction::insertAction(CMRProjectAction* action, CMRProjectCodeTreeInsert location)
{
	insert(action,location);
}

std::string genCCodeIndent(int depth)
{
	std::string tmp;
	//indent
	for (int i = 0 ; i < depth ; i++)
		tmp += "\t";

	return tmp;
}

/*******************  FUNCTION  *********************/
void CMRProjectAction::genCCode(std::ostream & out,int depth)
{
	//cases
	if (name == "cmrMainLoop" && description == "cmrMainLoop")
	{
		out << genCCodeIndent(depth) << "//mainLoop" << endl;
		for (Iterator it = getFirstChild() ; ! it.isEnd() ; ++it)
			it->genCCode(out,depth+1);
	} else if (name == "cmrSubBlock" && description == "cmrLoop") {
		out << genCCodeIndent(depth) << "for (" << eq->compute << " = 0 ; " << eq->compute << " < 9 ; " << eq->compute << "++ )" <<endl;
		out << genCCodeIndent(depth) << "{" << endl;
		for (Iterator it = getFirstChild() ; ! it.isEnd() ; ++it)
			it->genCCode(out,depth+1);
		out << genCCodeIndent(depth) << "}" << endl;
	} else if (name == "cmrEquation") {
		out << genCCodeIndent(depth) << "//eq TODO : "<< description << " = " << eq->compute << endl;
	} else {
		out << genCCodeIndent(depth) << "//Unknown member : " << name << " : " << description;
		if (eq != NULL)
			out << " : " << description << " = " << eq->compute;
		out << endl;
	}
}
