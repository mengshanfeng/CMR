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
#include <sstream>
#include "CMRProjectContext.h"

using namespace std;

/*******************  FUNCTION  *********************/
CMRProjectContext::CMRProjectContext(const CMRProjectContext* parent)
{
	//errors
	assert(parent != this);
	
	//setup
	this->parent = parent;
}

/*******************  FUNCTION  *********************/
CMRProjectEntity & CMRProjectContext::addEntry(CMRProjectEntity* entry)
{
	//vars
	CMRProjectEntity * conflict;
	
	//errors
	assert(entry != NULL);
	
	//check conflicts
	conflict = checkUnique(*entry);
	if (conflict != NULL)
	{
		stringstream tmp;
		tmp << "Caution, entity " << entry->getLatexName() << " (" << entry->getLongName()
			<< ") conflicts with another definition : " << conflict->getLatexName()
			<< " (" << conflict->getLongName() << ")." << endl;
		throw CMRLatexException(tmp.str());
	}
	
	//add it
	entities.push_back(entry);
	return *entry;
}

/*******************  FUNCTION  *********************/
CMRProjectEntity * CMRProjectContext::checkUnique(CMRProjectEntity & entry)
{
	//search in list
	for (CMRProjectEntityList::iterator it = entities.begin() ; it != entities.end() ; ++it)
	{
		//march names
		if (entry.getLongName() == (*it)->getLongName())
			return *it;
		
		//match entity
		if (entry.match((*it)->getLatexName()))
			return *it;
		
		//reverse match
		if ((*it)->match(entry.getLatexName()))
			return *it;
	}
	
	//not found
	return NULL;
}

/*******************  FUNCTION  *********************/
const CMRProjectEntity* CMRProjectContext::find( CMRLatexEntity2 & entity ) const
{
	//searh in list
	for (CMRProjectEntityList::const_iterator it = entities.begin() ; it != entities.end() ; ++it)
	{
		if ((*it)->match(entity))
			return *it;
	}
	
	//if not found, search in parent
	if (parent != NULL)
		return parent->find(entity);
	else
		return NULL;
}

/*******************  FUNCTION  *********************/
int CMRProjectContext::countTotalEntries ( void ) const
{
	//vars
	int cnt = 0;
	const CMRProjectContext * cur = this;
	
	//sum parent entries
	while (cur != NULL)
	{
		cnt += cur->entities.size();
		cur = cur->parent;
	}
	
	//return res
	return cnt;
}

/*******************  FUNCTION  *********************/
void CMRProjectContext::printDebug ( std::ostream & out ) const
{
	//vars
	const CMRProjectContext * cur = this;
	
	//loop on entries
	while (cur != NULL)
	{
		out << "   - Level : " << endl;
		for (CMRProjectEntityList::const_iterator it = cur->entities.begin(); it != cur->entities.end() ; ++it)
			out << "          + " << (*it)->getLatexName() << " : " << (*it)->getLongName() << endl;
		cur = cur->parent;
	}
}
