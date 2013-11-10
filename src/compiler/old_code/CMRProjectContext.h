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
#include "CMRProjectEntity.h"

/*********************  TYPES  **********************/
typedef std::vector <CMRProjectEntity*> CMRProjectEntityList;

/*********************  CLASS  **********************/
struct CMRProjectContext
{
	CMRProjectContext(const CMRProjectContext * parent = NULL);
	int countTotalEntries(void) const;
	void printDebug(void) const;
	CMRProjectEntity & addEntry(CMRProjectEntity * entry);
	CMRProjectEntity* checkUnique(CMRProjectEntity & entry);
// 	CMREntity * find(CMRLatexEntity & entity);
	const CMRProjectEntity * find(CMRLatexEntity & entity) const;
	const CMRProjectContext * parent;
	CMRProjectEntityList entities;
};

#endif //CMR_PROJECT_CONTEXT_H
