/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_RUNNER_OMP_TASK_H
#define CMR_RUNNER_OMP_TASK_H

/********************  HEADERS  *********************/
#include "CMRRunnerSeq.h"

/*********************  CLASS  **********************/
class CMRRunnerOMPTask : public CMRRunnerSeq
{
	public:
		CMRRunnerOMPTask(CMRCmdOptions & options);
		virtual void runOperationNode(CMRMeshOperationNode & opNode);
	private:
		int nbThreads;
		int multiplier;
		int minCells;
};

#endif //CMR_RUNNER_OMP_TASK_H
