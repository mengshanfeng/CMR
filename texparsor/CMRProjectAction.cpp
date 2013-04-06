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
void CMRProjectAction::replaceLoops()
{
	CMRLatexEntity * term;
	string op;

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
			this->addEquation("\\CMRTMP{1}","cmrTmpValue1","0",CMR_INSERT_BEFORE);
			cout << "Replace loops with iterator (" << term->subscriptTotalValue << ") and core (" << term->params[0]->string << ")" << endl;
			CMRProjectAction & ac = this->addSubBlock("cmrLoop",term->subscriptTotalValue,CMR_INSERT_BEFORE);
			ac.addEquation("\\CMRTMP{1}","cmrTmpValue1",string("\\CMRTMP{1}") + op + term->params[0]->string);
			cmrParseLatexFormula(f,"\\CMRTMP{1}");
			*term = *f.childs[0];
			ac.replaceLoops();
		}
	} else {
		for (Iterator it = getFirstChild() ; ! it.isEnd() ; ++it)
			it->replaceLoops();
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
