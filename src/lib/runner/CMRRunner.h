/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_RUNNER_H
#define CMR_RUNNER_H

/********************  HEADERS  *********************/
#include <vector>
#include "CMRMeshOperationNode.h"
#include <common/CMRObjectFactory.h>

/*********************  TYPES  **********************/
class CMRVarSystem;
struct CMRRect;
class CMROutputer;
class CMRCmdOptions;

/*********************  TYPES  **********************/
typedef std::vector<CMRMeshOperationNode> CMRMeshOperationNodeVector;

/*********************  CLASS  **********************/
class CMRRunner : public CMRDynamicObject
{
	public:
		CMRRunner(CMRCmdOptions & options);
		virtual ~CMRRunner(void);
		void addInitAction(CMRMeshOperation * op,CMRRect rect);
		void addLoopAction(CMRMeshOperation * op,CMRRect rect);
		void addPrepareWriteAction(CMRMeshOperation * op,CMRRect rect);
		void setWriter( CMROutputer* outputer, int stepGap );
		CMRRect getLocalRect(void) const;
		CMRRect getGlobalRect(void) const;
		virtual void initLibs(int& argc, char**& argv);
		virtual void finishLibs(void);
		virtual void run(int iterations) = 0;
		virtual void init(CMRVarSystem * varSystem,const CMRRect & globalRect,const CMRRect & localRect );
	protected:
		CMRRect globalRect;
		CMRRect localRect;
		CMRMeshOperationNodeVector initActions;
		CMRMeshOperationNodeVector loopActions;
		CMRMeshOperationNodeVector writeActions;
		CMRVarSystem * system;
		CMROutputer * outputer;
		int writeInterval;
};

#endif //CMR_RUNNER_H
