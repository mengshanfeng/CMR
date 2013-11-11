/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_BASIC_SEQ_RUNNER_H
#define CMR_BASIC_SEQ_RUNNER_H

/********************  HEADERS  *********************/
#include <vector>
#include "common/CMRGeometry.h"
#include <common/CMRSpaceSplitter.h>

/*********************  TYPES  **********************/
class CMRMeshOperation;
class CMRVarSystem;

/*********************  STRUCT  *********************/
struct CMRMeshOperationNode
{
	CMRMeshOperationNode(CMRMeshOperation *op,const CMRRect & rect) {this->op = op; this->rect = rect;};
	CMRMeshOperation * op;
	CMRRect rect;
};

/*********************  TYPES  **********************/
typedef std::vector<CMRMeshOperationNode> CMRMeshOperationNodeVector;

/*********************  CLASS  **********************/
class CMROutputer
{
	public:
		virtual ~CMROutputer(void) {};
		virtual void write(CMRVarSystem * system,const CMRRect & global,const CMRRect & local) = 0;
};

/*********************  CLASS  **********************/
template <class TVarSystem>
class CMRBasicSeqRunner
{
	public:
		CMRBasicSeqRunner( int& argc, char**& argv, const CMRRect & globalDomainSize );
		~CMRBasicSeqRunner(void);
		void addInitAction(CMRMeshOperation * op,CMRRect rect);
		void addLoopAction(CMRMeshOperation * op,CMRRect rect);
		void addPrepareWriteAction(CMRMeshOperation * op,CMRRect rect);
		void setWriter( CMROutputer* outputer, int stepGap );
		void run(int iterations);
		CMRRect getLocalRect(void) const;
		CMRRect getGlobalRect(void) const;
		const CMRAbstractSpaceSplitter & getSplitter(void) const;
	protected:
		void initMPI( int& argc, char**& argv );
		void initDomain(const CMRRect & globalDomainSize);
	private:
		CMRRect globalRect;
		CMRRect localRect;
		CMRMeshOperationNodeVector initActions;
		CMRMeshOperationNodeVector loopActions;
		CMRMeshOperationNodeVector writeActions;
		CMRVarSystem * system;
		CMROutputer * outputer;
		int writeInterval;
		CMRAbstractSpaceSplitter * splitter;
};

#include "CMRBasicSeqRunner_impl.h"

#endif //CMR_BASIC_SEQ_RUNNER_H
