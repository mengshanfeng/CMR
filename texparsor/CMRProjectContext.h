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
typedef std::vector <CMREntity*> CMRProjectEntityList;

/*********************  CLASS  **********************/
struct CMRProjectContext
{
	CMRProjectContext(CMRProjectContext * parent) {this->parent = parent;};
	CMRProjectContext * parent;
	CMRProjectEntityList entities;
};

#endif //CMR_PROJECT_CONTEXT_H
