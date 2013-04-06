/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <iostream>
#include "CMRProject.h"
#include "CMRProjectConstant.h"
#include "CMRProjectVariable.h"
#include "CMRProjectAction.h"
#include "CMRProjectDefinition.h"

using namespace std;

/*******************  FUNCTION  *********************/
CMREntityConstant& CMRProject::addConstant ( const string& latexName, const string& longName )
{
	CMREntityConstant * tmp = new CMREntityConstant(latexName,longName);
	constants.push_back(tmp);
	rootContext.entities.push_back(tmp);
	return *tmp;
}

/*******************  FUNCTION  *********************/
CMRProjectVariable& CMRProject::addvariable ( const string& latexName, const string& longName, const string& type )
{
	CMRProjectVariable * tmp = new CMRProjectVariable(latexName,longName,type);
	variables.push_back(tmp);
	rootContext.entities.push_back(tmp);
	return *tmp;
}

/*******************  FUNCTION  *********************/
CMRProjectAction& CMRProject::addAction ( string name, string descr )
{
	CMRProjectAction * action = new CMRProjectAction(name,descr);
	actions.push_back(action);
	return *action;
}

/*******************  FUNCTION  *********************/
void CMRProject::replaceLoops(void )
{
	int id;
	for (CMRProjectActionVector::iterator it = actions.begin() ; it != actions.end() ; ++it)
	{
		id = 0;
		(*it)->replaceLoops(&id);
	}
}

/*******************  FUNCTION  *********************/
void CMRProject::printDebug(void )
{
	cout << "=============================================" << endl;
	for (CMRProjectActionVector::iterator it = actions.begin() ; it != actions.end() ; ++it)
	{
		cout << "Actions [" << (*it)->getName() << " , " << (*it)->getDescription() << "] : " << endl;
		(*it)->printDebug(1);
	}
	cout << "=============================================" << endl;
}

/*******************  FUNCTION  *********************/
void CMRProject::genCCode(ostream& out)
{
	for (CMRProjectActionVector::iterator it = actions.begin() ; it != actions.end() ; ++it)
	{
		out << "void " << (*it)->getName() << "(void)" << endl;
		out << "{" << endl;
		(*it)->genCCode(out,0);
		out << "}" << endl;
		out << endl;
	}
}

/*******************  FUNCTION  *********************/
CMRProjectDefinition & CMRProject::addDefinition(const string& latexName, const string& longName)
{
	CMRProjectDefinition * def = new CMRProjectDefinition(latexName,longName);
	definitions.push_back(def);
	return *def;
}

/*******************  FUNCTION  *********************/
CMRProjectDefinition& CMRProject::addDefinition(const string& latexName, const string& longName, const string& compute)
{
	CMRProjectDefinition & def = addDefinition(latexName,longName);
	def.addEquation(latexName,longName,compute);
	return def;
}
