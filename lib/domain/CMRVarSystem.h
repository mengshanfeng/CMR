/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 02/2013
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_VAR_SYSTEM_H
#define CMR_VAR_SYSTEM_H

/********************  HEADERS  *********************/
#include <vector>
#include <string>
#include <cstdlib>
#include "CMRDomainStorage.h"

/********************  MACRO  ***********************/
#define CMR_MAX_TSTEPS 2

/*********************  CLASS  **********************/
struct CMRVariable
{
	CMRVariable(const std::string & name,size_t typeSize,int ghostDepth);
	std::string name;
	int ghostDepth;
	size_t typeSize;
	CMRDomainStorage * domain[CMR_MAX_TSTEPS];
};

/*********************  CLASS  **********************/
typedef std::vector<CMRVariable> CMRVariableVector;
typedef int CMRVariableId;

/*********************  CLASS  **********************/
class CMRDomainBuilder
{
	public:
		virtual ~CMRDomainBuilder(void) {};
		virtual CMRAbstractDomain * buildDomain(const CMRVariable & variable) = 0;
};

/*********************  CLASS  **********************/
class CMRVarSystem
{
	public:
		CMRVarSystem(CMRDomainBuilder * domainBuilder);
		~CMRVarSystem(void);
		CMRVariableId addVariable(std::string name,int typeSize,int ghostDepth);
		void permutVar(CMRVariableId varId);
		void permutAllVars(void);
		CMRAbstractDomain * getDomain(CMRVariableId varId,int tstep);
		void freeDomain(CMRVariableId varId,int tstep);
		void freeAllDomains(int tstep);
		void printDebug(void) const;
	private:
		//never copy such objects
		CMRVarSystem(const CMRVarSystem & sys);
		CMRVarSystem & operator =(const CMRVarSystem & sys);
	private:
		CMRVariableVector variables;
		CMRDomainBuilder * domainBuilder;
};

#endif // CMR_VAR_SYSTEM_H
