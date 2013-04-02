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
#include "CMRProjectContext.h"
#include "CMRProjectEquation.h"

/*********************  TYPES  **********************/
struct CMRProjectContext;
class CMRProjectActionBlock;
typedef std::vector <CMRProjectActionBlock*> CMRProjectActionBlockVector;

/*********************  CLASS  *********************/
struct CMRProjectActionBlock
{
	CMRProjectActionBlock(CMRProjectContext * parent) :context(parent) {eq = NULL;};
	void replaceLoops(void);
	std::string loopDescr;
	std::string parameter;
	CMRProjectEquation * eq;
	CMRProjectActionBlockVector subblocks;
	CMRProjectContext context;
};

/*********************  CLASS  *********************/
class CMRProjectAction
{
	public:
		CMRProjectAction(CMRProjectContext * parentContext,std::string name,std::string descr = "");
		CMRProjectActionBlock & addSubBlock(std::string loopDescr,std::string parameter);
		CMRProjectEquation& addEquation( const std::string& latexName, const std::string& longName, const std::string& compute );
		void replaceLoops(void);
	private:
	CMRProjectActionBlockVector blocks;
	std::string name;
	std::string description;
	CMRProjectContext context;
};

#endif //CMR_PROJECT_ACTION_H
