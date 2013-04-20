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

/*********************  TYPES  **********************/
typedef std::vector <CMRProjectEntity*> CMRProjectEntityList;

/*********************  CLASS  **********************/
class CMRProjectContext
{
	public:
		CMRProjectContext(const CMRProjectContext * parent = NULL);
		int countTotalEntries(void) const;
		void printDebug(std::ostream & out = std::cerr) const;
		CMRProjectEntity & addEntry(CMRProjectEntity * entry);
		CMRProjectEntity* checkUnique(CMRProjectEntity & entry);
		const CMRProjectEntity * find( CMRLatexEntity2& entity ) const;
	private:
		const CMRProjectContext * parent;
		CMRProjectEntityList entities;
};

#endif //CMR_PROJECT_CONTEXT_H
