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
#include <sstream>
#include "ProjectContext.h"

using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

#warning remove this
static int tmpCnt = 0;

/*******************  FUNCTION  *********************/
ProjectContext::ProjectContext(const ProjectContext* parent)
{
	//errors
	assert(parent != this);
	
	//setup
	this->parent = parent;
}

/*******************  FUNCTION  *********************/
ProjectEntity & ProjectContext::addEntry(ProjectEntity* entry)
{
	//vars
	ProjectEntity * conflict;
	
	//errors
	assert(entry != NULL);
	
	//check conflicts
	conflict = checkUnique(*entry);
	if (conflict != NULL)
	{
		printDebug();
		stringstream tmp;
		tmp << "Caution, entity " << entry->getLatexName() << " (" << entry->getLongName()
			<< ") conflicts with another definition : " << conflict->getLatexName()
			<< " (" << conflict->getLongName() << ")." << endl;
		throw LatexException(tmp.str());
	}
	
	//add it
	entities.push_back(entry);
	return *entry;
}

/*******************  FUNCTION  *********************/
ProjectEntity * ProjectContext::checkUnique(const ProjectEntity & entry)
{
	//search in list
	for (ProjectEntityList::iterator it = entities.begin() ; it != entities.end() ; ++it)
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
const ProjectEntity* ProjectContext::findInParent(const LatexEntity& entity, bool onlyWildCardNames) const
{
	if (parent == NULL)
		return NULL;
	else
		return parent->find(entity,onlyWildCardNames);
}

/*******************  FUNCTION  *********************/
const ProjectEntity* ProjectContext::find( const LatexEntity & entity , bool onlyWildCardNames) const
{
// 	#warning "Do some stuff on priority rules when found multiple matches (similar to what CSS dores)"
// 	//check wildcard name in parent
// 	if (parent != NULL)
// 	{
// 		const ProjectEntity* res = parent->find(entity,true);
// 		if (res != NULL)
// 			return res;
// 	}

	//searh in list
	for (ProjectEntityList::const_iterator it = entities.begin() ; it != entities.end() ; ++it)
	{
		if ((onlyWildCardNames == false || (*it)->isWildcardName()) && (*it)->match(entity))
			return *it;
	}
	
	//if not found, search in parent
	if (parent != NULL)
		return parent->find(entity);
	else
		return NULL;
}

/*******************  FUNCTION  *********************/
int ProjectContext::countTotalEntries ( void ) const
{
	//vars
	int cnt = 0;
	const ProjectContext * cur = this;
	
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
void ProjectContext::printDebug ( std::ostream & out ) const
{
	//vars
	const ProjectContext * cur = this;
	
	//loop on entries
	while (cur != NULL)
	{
		out << "    - Level : " << endl;
		for (ProjectEntityList::const_iterator it = cur->entities.begin(); it != cur->entities.end() ; ++it)
			out << "          + " << (*it)->getLatexName() << " : " << (*it)->getLongName() << endl;
		cur = cur->parent;
	}
}

/*******************  FUNCTION  *********************/
int ProjectContext::getDepth ( void ) const
{
	//vars
	int tmp = 0;
	const ProjectContext * current = this;
	
	//loop in tree
	while (current->parent != NULL)
	{
		tmp++;
		current = current->parent;
	}
	
	//finish
	return tmp;
}

/*******************  FUNCTION  *********************/
ProjectTempNames ProjectContext::genTempName ( const std::string & base )
{
	ProjectTempNames res;

	stringstream tmpLong;
	tmpLong << base << "_" << getDepth() << "_" << tmpCnt;
	res.longName = tmpLong.str();

	stringstream tmpShort;
	tmpShort << "\\CMRTMP^"<< getDepth() <<"{" << tmpCnt << "}";
	res.shortName = tmpShort.str();

	tmpCnt++;

	return res;
}

/*******************  FUNCTION  *********************/
void ProjectContext::setParent ( const ProjectContext* parent )
{
	if (parent != NULL)
	{
		for (ProjectEntityList::const_iterator it = entities.begin() ; it != entities.end() ; ++it)
			checkUnique(**it);
	}
	
	this->parent = parent;
}

/*******************  FUNCTION  *********************/
void ProjectContext::setKey ( const string& key, const string& value )
{
	this->keys[key] = value;
}

/*******************  FUNCTION  *********************/
string ProjectContext::readKey ( const string& key ) const
{
	const ProjectContext * cur = this;
	
	while (cur != NULL)
	{
		ProjectContextKeyMap::const_iterator it = cur->keys.find(key);
		if (it == cur->keys.end())
			cur = cur->parent;
		else
			return it->second;
	}
	
	return "";
}

}
