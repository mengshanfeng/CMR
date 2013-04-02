/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_PROJECT_ACTION_H
#define CMR_PROJECT_ACTION_H

/********************  HEADERS  *********************/
#include <vector>
#include <list>
#include "CMRProjectContext.h"
#include "CMRProjectEquation.h"

/*********************  TYPES  **********************/
struct CMRProjectContext;
class CMRProjectAction;
typedef std::list <CMRProjectAction*> CMRProjectActionVector;

/*********************  CLASS  *********************/
class CMRProjectAction
{
	public:
		CMRProjectAction(CMRProjectContext * parentContext,std::string name,std::string descr = "");
		CMRProjectAction & addSubBlock(std::string loopDescr,std::string parameter);
		CMRProjectEquation& addEquation( const std::string& latexName, const std::string& longName, const std::string& compute );
		CMRProjectEquation& addEquationBefore(CMRProjectAction * action, const std::string& latexName, const std::string& longName, const std::string& compute );
		void replaceLoops(CMRProjectAction * parent);
		void printDebug(int depth);
	private:
	CMRProjectEquation * eq;
	CMRProjectActionVector childs;
	std::string name;
	std::string description;
	CMRProjectContext context;
};

#endif //CMR_PROJECT_ACTION_H
