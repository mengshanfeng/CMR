/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_RUNNER_SEQ_H
#define CMR_RUNNER_SEQ_H

/********************  HEADERS  *********************/
#include "CMRRunner.h"

/*********************  CLASS  **********************/
class CMRRunnerSeq : public CMRRunner
{
	public:
		CMRRunnerSeq( int& argc, char**& argv ,const CMRRect & globalDomainSize,CMRRunnerInitFactory * factory);
		virtual void run(int iterations);
	protected:
		virtual void runInitStep(CMRMeshOperationNodeVector & actions);
		virtual void prepareWrite(CMRMeshOperationNodeVector & actions);
		virtual void doWrite(CMROutputer & outputer);
		virtual void runCompute(CMRMeshOperationNodeVector & actions);
};

#endif //CMR_RUNNER_MONO_PROCESS_H
