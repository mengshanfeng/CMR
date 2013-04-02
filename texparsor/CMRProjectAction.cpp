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
#include "CMRProjectAction.h"
#include "CMRProject.h"
#include "parsor/CMRTexParsor.h"

using namespace std;

/*******************  FUNCTION  *********************/
CMRProjectAction::CMRProjectAction ( CMRProjectContext* parentContext, string name, string descr )
	:context(parentContext)
{
	assert(parentContext != NULL);
	this->name = name;
	this->description = descr;
}

/*******************  FUNCTION  *********************/
CMRProjectEquation& CMRProjectAction::addEquation ( const string& latexName, const string& longName, const string& compute )
{
	CMRProjectAction * tmpBlock = new CMRProjectAction(&context,"cmrEquation",latexName);
	CMRProjectEquation * tmp = tmpBlock->eq = new CMRProjectEquation(latexName,longName,compute);
	context.entities.push_back(tmp);
	childs.push_back(tmpBlock);
	return *tmp;
}

/*******************  FUNCTION  *********************/
CMRProjectEquation& CMRProjectAction::addEquationBefore(CMRProjectAction* action, const string& latexName, const string& longName, const string& compute)
{
	CMRProjectAction * tmpBlock = new CMRProjectAction(&context,"cmrEquation",latexName);
	CMRProjectEquation * tmp = tmpBlock->eq = new CMRProjectEquation(latexName,longName,compute);
	context.entities.push_back(tmp);
	CMRProjectActionVector::iterator it = childs.begin();
	childs.insert(it,tmpBlock);
	return *tmp;
}

/*******************  FUNCTION  *********************/
CMRProjectAction& CMRProjectAction::addSubBlock ( string loopDescr, string parameter )
{
	CMRProjectAction * tmpBlock = new CMRProjectAction(&context,"cmrSubBlock",loopDescr);
	CMRProjectEquation * tmp = tmpBlock->eq = new CMRProjectEquation(parameter,"cmrIndice",parameter);
	context.entities.push_back(tmp);
	childs.push_back(tmpBlock);
	return *tmpBlock;
}

/*******************  FUNCTION  *********************/
void CMRProjectAction::replaceLoops(CMRProjectAction * parent )
{
	CMRLatexEntity * term;
	string op;

	if (name == "cmrEquation")
	{
		assert(eq != NULL);
		assert(parent != NULL);
		while ((term = eq->extractNextInnerLoop()) != NULL)
		{
			if (term->name == "\\sum")
				op = " + ";
			else
				assert(false);
			CMRLatexFormulas f;
			parent->addEquation("\\CMRTMP{1}","cmrTmpValue1","0");
			cout << "Replace loops with iterator (" << term->subscriptTotalValue << ") and core (" << term->params[0]->string << ")" << endl;
			CMRProjectAction & ac = parent->addSubBlock("cmrLoop",term->subscriptTotalValue);
			ac.addEquation("\\CMRTMP{1}","cmrTmpValue1",string("\\CMRTMP{1}") + op + term->params[0]->string);
			cmrParseLatexFormula(f,"\\CMRTMP{1}");
			*term = *f.childs[0];
			ac.replaceLoops(parent);
		}
	} else {
		for (CMRProjectActionVector::iterator it = childs.begin() ; it != childs.end() ; ++it)
			(*it)->replaceLoops(this);
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
		for (CMRProjectActionVector::iterator it = childs.begin() ; it != childs.end() ; ++it)
			(*it)->printDebug(depth+1);
	}
}
