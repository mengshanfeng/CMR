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
CMRProjectContext::CMRProjectContext(const CMRProjectContext* parent)
{
	this->parent = parent;
}

/*******************  FUNCTION  *********************/
CMRProjectEntity & CMRProjectContext::addEntry(CMRProjectEntity* entry)
{
	CMRProjectEntity * conflict;
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
CMRProjectEntity * CMRProjectContext::checkUnique(CMRProjectEntity & entry)
{
	for (CMRProjectEntityList::iterator it = entities.begin() ; it != entities.end() ; ++it)
	{
		CMRProjectCaptureMap capture;
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
const CMRProjectEntity* CMRProjectContext::find( CMRLatexEntity& entity ) const
{
	for (CMRProjectEntityList::const_iterator it = entities.begin() ; it != entities.end() ; ++it)
	{
		CMRProjectCaptureMap capture;
		if ((*it)->match(entity,capture))
			return *it;
	}
	
	if (parent != NULL)
		return parent->find(entity);
	else
		return NULL;
}

/*******************  FUNCTION  *********************/
int CMRProjectContext::countTotalEntries ( void ) const
{
	int cnt = 0;
	const CMRProjectContext * cur = this;
	while (cur != NULL)
	{
		cnt += cur->entities.size();
		cur = cur->parent;
	}
	return cnt;
}

/*******************  FUNCTION  *********************/
void CMRProjectContext::printDebug ( void ) const
{
	const CMRProjectContext * cur = this;
	while (cur != NULL)
	{
		cerr << "   - Level : " << endl;
		for (CMRProjectEntityList::const_iterator it = cur->entities.begin(); it != cur->entities.end() ; ++it)
			cerr << "          + " << (*it)->latexName << " : " << (*it)->longName << endl;
		cur = cur->parent;
	}
}
