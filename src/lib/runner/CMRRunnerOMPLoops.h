/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_RUNNER_OMP_LOOPS_H
#define CMR_RUNNER_OMP_LOOPS_H

/********************  HEADERS  *********************/
#include "CMRRunnerSeq.h"

/*********************  CLASS  **********************/
class CMRRunnerOMPLoops : public CMRRunnerSeq
{
	public:
		CMRRunnerOMPLoops(CMRCmdOptions & options);
		virtual void runOperationNode(CMRMeshOperationNode & opNode);
};

#endif //CMR_RUNNER_OMP_LOOPS_H
