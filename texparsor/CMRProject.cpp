/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "CMRProject.h"
#include "CMRProjectConstant.h"
#include "CMRProjectVariable.h"
#include "CMRProjectAction.h"

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
	CMRProjectAction * tmp = new CMRProjectAction(&rootContext,name,descr);
	actions.push_back(tmp);
	return *tmp;
}
