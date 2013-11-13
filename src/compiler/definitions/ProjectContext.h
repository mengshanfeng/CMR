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
#include "ProjectEntity.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  TYPES  **********************/
typedef std::vector <ProjectEntity*> ProjectEntityList;

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
		ProjectEntity & addEntry(ProjectEntity * entry);
		ProjectEntity* checkUnique( const ProjectEntity& entry );
		const ProjectEntity * find( const LatexEntity& entity, bool onlyWildCardNames = false ) const;
		const ProjectEntity * findInParent( const LatexEntity& entity, bool onlyWildCardNames = false ) const;
		int getDepth(void) const;
		ProjectTempNames genTempName( const std::string& base = "temp");
		void setParent(const ProjectContext * parent);
	private:
		const ProjectContext * parent;
		ProjectEntityList entities;
};

}

#endif //CMR_COMPILER_PROJECT_CONTEXT_H
