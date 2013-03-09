/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cstring>
#include "CMRVarSystem.h"

/*******************  FUNCTION  *********************/
CMRVariable::CMRVariable ( const std::string& name, size_t typeSize, int ghostDepth )
{
	this->name = name;
	this->typeSize = typeSize;
	this->ghostDepth = ghostDepth;
	memset(domain,0,sizeof(domain));
}

/*******************  FUNCTION  *********************/
CMRVarSystem::~CMRVarSystem ( void )
{
	freeAllDomains(CMR_ALL);
}

/*******************  FUNCTION  *********************/
CMRVariableId CMRVarSystem::addVariable ( std::string name, int typeSize, int ghostDepth )
{
	CMRVariable var(name,typeSize,ghostDepth);
	variables.push_back(var);
	return variables.size();
}

/*******************  FUNCTION  *********************/

