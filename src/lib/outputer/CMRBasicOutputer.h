/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_BASIC_OUTPUTER_H
#define CMR_BASIC_OUTPUTER_H

/********************  HEADERS  *********************/
#include <cstdio>
#include <string>
#include "CMROutputer.h"

/*********************  TYPES  **********************/
class CMRAbstractSpaceSplitter;

/*********************  CLASS  **********************/
class CMRBasicOutputer : public CMROutputer
{
	public:
		CMRBasicOutputer( const std::string& fname, const CMRAbstractSpaceSplitter& splitter );
		virtual ~CMRBasicOutputer(void);
		virtual void write(CMRVarSystem * system,const CMRRect & global,const CMRRect & local);
	protected:
		void writeFileHeader(const CMRAbstractSpaceSplitter & splitter);
	private:
		std::string fname;
		FILE * fp;
};

#endif //CMR_BASIC_OUTPUTER_H
