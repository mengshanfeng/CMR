/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cstring>
#include <cassert>
#include "CMRVarSystem.h"
#include "CMRDomainBuilder.h"

/*******************  FUNCTION  *********************/
CMRVariable::CMRVariable ( const std::string& name, size_t typeSize, int ghostDepth, CMRVarMode mode )
{
	this->mode = mode;
	this->name = name;
	this->typeSize = typeSize;
	this->ghostDepth = ghostDepth;
	memset(domain,0,sizeof(domain));
}

/*******************  FUNCTION  *********************/
void CMRVariable::clearStep ( int id )
{
	//errors
	assert((id >= 0 && id < CMR_MAX_TSTEPS) || id == CMR_ALL);
	
	//remove all
	if (id == CMR_ALL)
	{
		for (int i = 0 ; i < CMR_MAX_TSTEPS ; i++)
		{
			if (domain[i] != NULL)
			{
				delete domain[i];
				domain[i] = NULL;
			}
		}
	} else {
		if (domain[id] != NULL)
		{
			delete domain[id];
			domain[id] = NULL;
		}
	}
}

/*******************  FUNCTION  *********************/
void CMRVariable::permut ( void )
{
	cmrPermut(domain[0],domain[1]);
}

/*******************  FUNCTION  *********************/
CMRVarSystem::CMRVarSystem ( CMRDomainBuilder* domainBuilder )
{
	this->domainBuilder = domainBuilder;
}

/*******************  FUNCTION  *********************/
CMRVarSystem::~CMRVarSystem ( void )
{
	freeDomain(CMR_ALL,CMR_ALL);
}

/*******************  FUNCTION  *********************/
void CMRVarSystem::setDomainBuilder ( CMRDomainBuilder* domainBuilder )
{
	assume(domainBuilder != NULL,"Cannot associate NULL CMRDomainBuilder.");
	assume(this->domainBuilder == NULL,"Cannot override an already setup CMRDomainBuilder.");
	this->domainBuilder = domainBuilder;
}

/*******************  FUNCTION  *********************/
CMRVariableId CMRVarSystem::addVariable ( std::string name, int typeSize, int ghostDepth )
{
	CMRVariable var(name,typeSize,ghostDepth);
	variables.push_back(var);
	return (int)variables.size() - 1;
}

/*******************  FUNCTION  *********************/
void CMRVarSystem::freeDomain ( CMRVariableId varId, int tstep )
{
	//errors
	assert((tstep >= 0 && tstep < CMR_MAX_TSTEPS) || tstep == CMR_ALL);
	assert((varId >= 0 && varId < (int)variables.size()) || varId == CMR_ALL);
	
	if (varId == CMR_ALL)
	{
		for (CMRVariableVector::iterator it = variables.begin() ; it != variables.end() ; ++it)
		{
			it->clearStep(tstep);
		}
	} else {
		variables[varId].clearStep(tstep);
	}
}

/*******************  FUNCTION  *********************/
CMRDomainStorage* CMRVarSystem::getDomain ( CMRVariableId varId, int tstep )
{
	//vars
	CMRDomainStorage * res;

	//errors
	assert(varId >= 0 && varId < (int)variables.size());
	assert(tstep >= 0 && tstep < CMR_MAX_TSTEPS);
	assume(domainBuilder != NULL,"Get a NULL CMRDomainBuilder, maybe you provide a NULL one or forget to use setDomainBuilder().");
	
	//get domain
	res = variables[varId].domain[tstep];
	
	//if NULL create it
	if (res == NULL)
		res = variables[varId].domain[tstep] = domainBuilder->buildDomain(variables[varId]);
	
	//return
	return res;
}

/*******************  FUNCTION  *********************/
void CMRVarSystem::permutVar ( CMRVariableId varId )
{
	//errors
	assert((varId >= 0 && varId < (int)variables.size()) || varId == CMR_ALL);
	
	if (varId == CMR_ALL)
	{
		for (CMRVariableVector::iterator it = variables.begin() ; it != variables.end() ; ++it)
		{
			it->permut();
		}
	} else {
		variables[varId].permut();
	}
}
