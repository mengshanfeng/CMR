/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 02/2013
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_TYPED_DOMAIN_H
#define CMR_TYPED_DOMAIN_H

/********************  HEADERS  *********************/
#include <vector>
#include <cassert>
#include "CMRAbstractDomain.h"
#include "CMRDebug.h"

/*********************  CLASS  **********************/
template <class T,class TBaseDomain>
class CMRTypedDomain : public TBaseDomain
{
	public:
		CMRAbstractDomain(size_t typeSize,int width,int height,int ghostDepth,int originX,int originY);
};

/*******************  FUNCTION  *********************/
template <class T,class TBaseDomain>
CMRAbstractDomain(size_t typeSize,int width,int height,int ghostDepth,int originX,int originY)
{
}

#endif // CMR_SPACE_SPLITTER_H
