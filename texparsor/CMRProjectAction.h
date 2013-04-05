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
enum CMRProjectActionInsert
{
	CMR_ACTION_INSERT_FIRST_CHILD,
	CMR_ACTION_INSERT_LAST_CHILD,
	CMR_ACTION_INSERT_FIRST,
	CMR_ACTION_INSERT_LAST,
	CMR_ACTION_INSERT_AFTER,
	CMR_ACTION_INSERT_BEFORE,
	CMR_ACTION_INSERT_INPLACE
};

/*********************  CLASS  *********************/
class CMRProjectAction
{
	public:
		CMRProjectAction(std::string name,std::string descr = "");
		CMRProjectAction & addSubBlock(std::string loopDescr,std::string parameter,CMRProjectActionInsert location = CMR_ACTION_INSERT_LAST_CHILD);
		CMRProjectEquation& addEquation( const std::string& latexName, const std::string& longName, const std::string& compute,CMRProjectActionInsert location = CMR_ACTION_INSERT_LAST_CHILD);
		void replaceLoops(void);
		void printDebug(int depth);
		void insertAction(CMRProjectAction * action,CMRProjectActionInsert location);
	private:
	CMRProjectEquation * eq;
	CMRProjectAction * next;
	CMRProjectAction * prev;
	CMRProjectAction * parent;
	CMRProjectAction * firstChild;
	CMRProjectAction * lastChild;
	std::string name;
	std::string description;
};

#endif //CMR_PROJECT_ACTION_H
