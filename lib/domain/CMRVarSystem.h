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
#include "CMRMemoryModel.h"

#define CMR_MAX_TSTEPS 2

/*********************  CLASS  **********************/
struct CMRVariable
{
	std::string name;
	CMRMemoryModel * model;
	int ghostDepth;
	CMRAbstrctDomain * domain[CMR_MAX_TSTEPS];
	int maxSteps;
};

/*********************  CLASS  **********************/
typedef std::vector<CMRVariable> CMRVariableVector;

/*********************  CLASS  **********************/
class CMRDomainBuilder
{
	public:
		virtual ~CMRDomainBuilder(void);
		virtual CMRAbstractDomain * buildDomain(const CMRVariable & variable);
};

/*********************  CLASS  **********************/
class CMRVarSystem
{
	public:
		int addVariable(std::string name,CMRMemoryModel * memoryModel,int ghostDepth,int maxSteps);
		void permutDomin(int varId,CMRMemoryModel * memoryModel);
		void permutAllDomain(void);
		CMRAbstractDomain * getDomain(int tstep,int varId);
	private:
		CMRMemoryModelVector vectors;
};

#endif // CMR_VAR_SYSTEM_H
