/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_PROJECT_CONTEXT_H
#define CMR_PROJECT_CONTEXT_H

/********************  HEADERS  *********************/
#include <vector>
#include <iostream>
#include "CMRProjectEntity.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  TYPES  **********************/
typedef std::vector <CMRProjectEntity*> CMRProjectEntityList;

/*********************  STRUCT  *********************/
struct CMRTempNames
{
	std::string shortName;
	std::string longName;
};

/*********************  CLASS  **********************/
class CMRProjectContext
{
	public:
		CMRProjectContext(const CMRProjectContext * parent = NULL);
		int countTotalEntries(void) const;
		void printDebug(std::ostream & out = std::cerr) const;
		CMRProjectEntity & addEntry(CMRProjectEntity * entry);
		CMRProjectEntity* checkUnique( const CMRProjectEntity& entry );
		const CMRProjectEntity * find( const CMRCompiler::LatexEntity& entity, bool onlyWildCardNames = false ) const;
		const CMRProjectEntity * findInParent( const CMRCompiler::LatexEntity& entity, bool onlyWildCardNames = false ) const;
		int getDepth(void) const;
		CMRTempNames genTempName( const std::string& base = "temp");
		void setParent(const CMRProjectContext * parent);
	private:
		const CMRProjectContext * parent;
		CMRProjectEntityList entities;
};

}

#endif //CMR_PROJECT_CONTEXT_H
