/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_APPLICATION_SEQ_H
#define CMR_APPLICATION_SEQ_H

/********************  HEADERS  *********************/
#include "CMRApplication.h"

/*********************  TYPES  **********************/
class CMRVarSystem;

/*********************  CLASS  **********************/
class CMRApplicationSeq : public CMRApplication
{
	public:
		CMRApplicationSeq(int& argc, char**& argv,CMRVarSystem * varSystem,int width,int height,int writeInterval);
		virtual ~CMRApplicationSeq(void);
		virtual void run(int iterations);
	protected:
		virtual void initLibs(int& argc, char**& argv);
		virtual void finishLibs(void);
		virtual void init( CMRVarSystem* varSystem, int width, int height, int writeInternval );
		virtual void finish(void);
		void initMPI( int& argc, char**& argv );
		void finishMPI(void);
};

#endif //CMR_APPLICATION_SEQ_H
