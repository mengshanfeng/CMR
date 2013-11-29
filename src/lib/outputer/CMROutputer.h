/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_OUTPUTER_H
#define CMR_OUTPUTER_H

/*********************  TYPES  **********************/
class CMRVarSystem;
struct CMRRect;

/*********************  CLASS  **********************/
class CMROutputer
{
	public:
		virtual ~CMROutputer(void) {};
		virtual void write(CMRVarSystem * system,const CMRRect & global,const CMRRect & local) = 0;
};

#endif //CMR_OUTPUTER_H
