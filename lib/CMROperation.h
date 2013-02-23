/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 02/2013
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_OPERATION_H
#define CMR_OPERATION_H

/********************  HEADERS  *********************/
#include <vector>
#include <cassert>
#include "CMRAbstractDomain.h"
#include "CMRDebug.h"

/*********************  CLASS  **********************/
class CMROperation;
class CMRAbstractDomain;
class CMRRect2D;

/*********************  CLASS  **********************/
class CMRMeshOperation
{
	public:
		CMROperation(void);
		virtual ~CMROperation(void);
		virtual void run(const CMRRect2D & zone);
};

/*********************  CLASS  **********************/
template <class T>
class CMRMeshOperationSimpleLoop : public CMRMeshOperation
{
	public:
		CMRMeshOperationSimpleLoop(CMRAbstractDomain* in, CMRAbstractDomain* out);
		virtual void run (const CMRRect2D& zone );
	private:
		CMRAbstractDomain* domainIn;
		CMRAbstractDomain* domainOut;
};

/*******************  FUNCTION  *********************/
class CellAccessor
{
};

/*******************  FUNCTION  *********************/
template <class T>
CMRMeshOperationSimpleLoop<T>::CMRMeshOperationSimpleLoop ( CMRAbstractDomain* in, CMRAbstractDomain* out )
{
	//errors
	assert(in != NULL);
	assert(out != NULL);
	
	//setup
	this->domainIn = in;
	this->domainOut = out;
}

/*******************  FUNCTION  *********************/
template <class T>
void CMRMeshOperationSimpleLoop<T>::run ( const CMRRect2D & zone )
{
	//errors
	assume(domainIn->isFullyInDomainMemory(zone),"Invalid zone not fully in domain.");
	assume(domainOut->isFullyInDomainMemory(zone),"Invalid zone not fully in domain.");
	//TODO : to check if the computation accept local calculation (same somaines)
	//assert(domainIn != domainOut || T::acceptLocalCompute());
	
	//errors if no contiguous on X axis
	if (!domainIn->isContiguous(0) && ! domainOut->isContiguous(0))
		warning("Caution, you loop on two domain with inner loop on X but the two domains are contiguous on Y !");
	
	//local copy to avoid deref
	CMRRect2D localZone = zone;
	
	for(int y = localZone.y ; y < localZone.height ; y++)
		for(int x = localZone.x ; x < localZone.width ; x++)
			T::cellAction(cell);
}

#endif // CMR_SPACE_SPLITTER_H
