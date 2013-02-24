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
#include "domain/CMRAbstractDomain.h"
#include "common/CMRDebug.h"

/*********************  CLASS  **********************/
class CMROperation;
class CMRAbstractDomain;
class CMRRect2D;

/*********************  CLASS  **********************/
class CMRMeshOperation
{
	public:
		CMRMeshOperation(void){}
		virtual ~CMRMeshOperation(void){}
		virtual void run(const CMRRect2D & zone) = 0;
};

/*********************  CLASS  **********************/
template <class T,class U>
class CMRMeshOperationSimpleLoop : public CMRMeshOperation
{
	public:
		CMRMeshOperationSimpleLoop(T * in, T * out);
		virtual void run (const CMRRect2D& zone );
	private:
		T * in;
		T * out;
};

/*******************  FUNCTION  *********************/
class CellAccessor
{
};

/*******************  FUNCTION  *********************/
template <class T,class U>
CMRMeshOperationSimpleLoop<T,U>::CMRMeshOperationSimpleLoop ( T * in, T * out )
{
	//errors
	assert(in != NULL);
	assert(out != NULL);
	
	//setup
	this->in = in;
	this->out = out;
}

/*******************  FUNCTION  *********************/
template <class T,class U>
void CMRMeshOperationSimpleLoop<T,U>::run ( const CMRRect2D & zone )
{
	//errors
	//assume(domainIn->isFullyInDomainMemory(zone),"Invalid zone not fully in domain.");
	//assume(domainOut->isFullyInDomainMemory(zone),"Invalid zone not fully in domain.");
	//TODO : to check if the computation accept local calculation (same somaines)
	//assert(domainIn != domainOut || T::acceptLocalCompute());
	
	//errors if no contiguous on X axis
	//if (!domainIn->isContiguous(0) && ! domainOut->isContiguous(0))
	//	warning("Caution, you loop on two domain with inner loop on X but the two domains are contiguous on Y !");
	
	//local copy to avoid deref
	CMRRect2D localZone = zone;
	typename T::CellAccessor cellIn(*in,localZone.x,localZone.y);
	typename T::CellAccessor cellOut(*out,localZone.x,localZone.y);
	
	for(int y = 0 ; y < localZone.height ; y++)
	{
		for(int x = 0 ; x < localZone.width ; x++)
		{
			typename T::CellAccessor cellLocalIn(cellIn,x,y);
			typename T::CellAccessor cellLocalOut(cellOut,x,y);
			U::cellAction(cellLocalIn,cellLocalOut);
		}
	}
}

#endif // CMR_SPACE_SPLITTER_H
