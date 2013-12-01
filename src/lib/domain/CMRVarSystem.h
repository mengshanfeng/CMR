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

/********************  ENUM  ************************/
enum CMRVarMode
{
	CMR_VARMODE_CELL
};

/*********************  CLASS  **********************/
struct CMRVariable
{
	CMRVariable(const std::string & name,size_t typeSize,int ghostDepth,CMRVarMode mode = CMR_VARMODE_CELL);
	void clearStep(int id);
	void permut(void);
	std::string name;
	int ghostDepth;
	size_t typeSize;
	CMRDomainStorage * domain[CMR_MAX_TSTEPS];
	CMRVarMode mode;
};

/*********************  CLASS  **********************/
typedef std::vector<CMRVariable> CMRVariableVector;
typedef unsigned int CMRVariableId;
class CMRDomainBuilder;

/*********************  CLASS  **********************/
class CMRVarSystem
{
	public:
		CMRVarSystem(CMRDomainBuilder * domainBuilder);
		virtual ~CMRVarSystem(void);
		CMRVariableId addVariable(std::string name,int typeSize,int ghostDepth);
		void permutVar(CMRVariableId varId);
		CMRDomainStorage * getDomain(CMRVariableId varId,int tstep);
		void freeDomain(CMRVariableId varId,int tstep);
		void printDebug(void) const;
		void setDomainBuilder(CMRDomainBuilder * domainBuilder);
	private:
		//never copy such objects
		CMRVarSystem(const CMRVarSystem & sys);
		CMRVarSystem & operator =(const CMRVarSystem & sys);
	private:
		CMRVariableVector variables;
		CMRDomainBuilder * domainBuilder;
};

#endif // CMR_VAR_SYSTEM_H
