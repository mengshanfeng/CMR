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

/*********************  TYPES  **********************/
class CMRVarSystem;
struct CMRRect;
class CMRAbstractSpaceSplitter;
class CMROutputer;
class CMRRunnerInitFactory;

/*********************  TYPES  **********************/
typedef std::vector<CMRMeshOperationNode> CMRMeshOperationNodeVector;

/*********************  CLASS  **********************/
class CMRRunner
{
	public:
		CMRRunner( int& argc, char**& argv ,const CMRRect & globalDomainSize,CMRRunnerInitFactory * factory);
		~CMRRunner(void);
		void addInitAction(CMRMeshOperation * op,CMRRect rect);
		void addLoopAction(CMRMeshOperation * op,CMRRect rect);
		void addPrepareWriteAction(CMRMeshOperation * op,CMRRect rect);
		void setWriter( CMROutputer* outputer, int stepGap );
		CMRRect getLocalRect(void) const;
		CMRRect getGlobalRect(void) const;
		const CMRAbstractSpaceSplitter & getSplitter(void) const;
		virtual void run(int iterations) = 0;
	protected:
		void initMPI( int& argc, char**& argv );
		virtual void initDomain(CMRRunnerInitFactory * factory);
	protected:
		CMRRect globalRect;
		CMRRect localRect;
		CMRMeshOperationNodeVector initActions;
		CMRMeshOperationNodeVector loopActions;
		CMRMeshOperationNodeVector writeActions;
		CMRVarSystem * system;
		CMROutputer * outputer;
		int writeInterval;
		CMRAbstractSpaceSplitter * splitter;
		int localId;
};

#endif //CMR_RUNNER_H
