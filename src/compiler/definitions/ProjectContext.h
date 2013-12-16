/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMPILER_PROJECT_CONTEXT_H
#define CMR_COMPILER_PROJECT_CONTEXT_H

/********************  HEADERS  *********************/
#include <vector>
#include <iostream>
#include "ProjectEntityOld.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  TYPES  **********************/
typedef std::vector <IProjectEntity*> ProjectEntityList;
typedef std::map<std::string,std::string> ProjectContextKeyMap;

/*********************  STRUCT  *********************/
struct ProjectTempNames
{
	std::string shortName;
	std::string longName;
};

/*********************  CLASS  **********************/
class ProjectContext
{
	public:
		ProjectContext(const ProjectContext * parent = NULL);
		int countTotalEntries(void) const;
		void printDebug(std::ostream & out = std::cerr) const;
		IProjectEntity & addEntry(IProjectEntity * entry);
		IProjectEntity* checkUnique( const IProjectEntity& entry );
		const IProjectEntity * find( const LatexEntity& entity, bool onlyWildCardNames = false ) const;
		const IProjectEntity * findInParent( const LatexEntity& entity, bool onlyWildCardNames = false ) const;
		std::string readKey(const std::string & key) const;
		void setKey(const std::string & key, const std::string & value);
		int getDepth(void) const;
		ProjectTempNames genTempName( const std::string& base = "temp");
		void setParent(const ProjectContext * parent);
	private:
		const ProjectContext * parent;
		ProjectEntityList entities;
		ProjectContextKeyMap keys;
};

}

#endif //CMR_COMPILER_PROJECT_CONTEXT_H
