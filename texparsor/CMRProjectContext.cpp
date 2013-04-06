/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "CMRProjectContext.h"

using namespace std;

/*******************  FUNCTION  *********************/
CMRProjectContext::CMRProjectContext(CMRProjectContext* parent)
{
	this->parent = parent;
}

/*******************  FUNCTION  *********************/
CMREntity & CMRProjectContext::addEntry(CMREntity* entry)
{
	CMREntity * conflict;
	assert(entry != NULL);
	conflict = checkUnique(*entry);
	if (conflict != NULL)
	{
		cerr << "Caution, entity " << entry->latexName << " (" << entry->longName 
			<< ") conflicts with another definition : " << conflict->latexName 
			<< " (" << conflict->longName << ")." << endl;
		abort();
	}
	entities.push_back(entry);
	return *entry;
}

/*******************  FUNCTION  *********************/
CMREntity * CMRProjectContext::checkUnique(CMREntity & entry)
{
	for (CMRProjectEntityList::iterator it = entities.begin() ; it != entities.end() ; ++it)
	{
		CMRIndiceCaptureMap capture;
		if (entry.longName == (*it)->longName)
			return *it;
		if (entry.match((*it)->latexEntity, capture))
			return *it;
		if ((*it)->match(entry.latexEntity,capture))
			return *it;
	}
	return NULL;
}

/*******************  FUNCTION  *********************/
CMREntity* CMRProjectContext::find(CMRLatexEntity& entity)
{
	for (CMRProjectEntityList::iterator it = entities.begin() ; it != entities.end() ; ++it)
	{
		CMRIndiceCaptureMap capture;
		if ((*it)->match(entity,capture))
			return *it;
	}
	
	if (parent != NULL)
		return parent->find(entity);
	else
		return NULL;
}
