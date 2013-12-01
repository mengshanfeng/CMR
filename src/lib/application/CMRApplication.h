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
class CMRMeshOperation;

/*********************  CLASS  **********************/
class CMRApplication
{
	public:
		CMRApplication(void);
		virtual ~CMRApplication(void);
		virtual void run(int iterations = -1) = 0;
		CMRRect getLocalRect(void);
		CMRRect getGlobalRect(void);
		void addInitAction(CMRMeshOperation * op,CMRRect rect);
		void addLoopAction(CMRMeshOperation * op,CMRRect rect);
		void addPrepareWriteAction(CMRMeshOperation * op,CMRRect rect);
		CMRRunner & getRunner(void);
	protected:
		void setRunner(CMRRunner * runner);
	private:
		CMRRunner * runner;
};

#endif //CMR_RUNNER_H
