/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_BASIC_SEQ_RUNNER_H
#define CMR_BASIC_SEQ_RUNNER_H
#include <../../src/lib/common/CMRGeometry.h>

/*********************  TYPES  **********************/
class CMRMeshOperation;
class CMROutputer;

/*********************  CLASS  **********************/
template <class TVarSystem>
class CMRBasicSeqRunner
{
	public:
		CMRBasicSeqRunner(CMRRect globalDomainSize);
		void addInitAction(CMRMeshOperation & op,CMRRect rect);
		void addLoopAction(CMRMeshOperation & op,CMRRect rect);
		void addPrepareWriteAction(CMRMeshOperation & op,CMRRect rect);
		void setWriter(CMROutputer & output,int stepGap);
		void run(int iterations);
	private:
		int width;
		int height;
};

#endif //CMR_BASIC_SEQ_RUNNER_H
