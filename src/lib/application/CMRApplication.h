/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_APPLICATION_H
#define CMR_APPLICATION_H

/********************  HEADERS  *********************/
#include <common/CMRCommon.h>
#include <common/CMRGeometry.h>

/*********************  TYPES  **********************/
class CMRRunner;
class CMRVarSystem;
class CMRMeshOperation;

/*********************  CLASS  **********************/
class CMRApplication
{
	public:
		CMRApplication(void);
		virtual ~CMRApplication(void);
		virtual void initLibs(int& argc, char**& argv) = 0;
		virtual void finishLibs(void) = 0;
		virtual void init(CMRVarSystem * varSystem,int width,int height,int writeInterval) = 0;
		virtual void finish(void) = 0;
		virtual void run(int iterations) = 0;
		CMRRect getLocalRect(void);
		CMRRect getGlobalRect(void);
		void addInitAction(CMRMeshOperation * op,CMRRect rect);
		void addLoopAction(CMRMeshOperation * op,CMRRect rect);
		void addPrepareWriteAction(CMRMeshOperation * op,CMRRect rect);
		CMRRunner & getRunner(void);
	protected:
		void checkReadyForStep(CMRInitState nextState);
		void markStep(CMRInitState nextState);
		void markInitStep(CMRRunner * runner);
	private:
		CMRInitState state;
		CMRRunner * runner;
};

#endif //CMR_RUNNER_H
