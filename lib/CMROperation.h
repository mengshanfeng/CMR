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
#include "domain/CMRVarSystem.h"
#include "common/CMRDebug.h"

/*********************  CLASS  **********************/
class CMROperation;
class CMRAbstractDomain;
struct CMRRect;
class CMRVarSystem;

/*********************  CLASS  **********************/
class CMRMeshOperation
{
	public:
		CMRMeshOperation(void){}
		virtual ~CMRMeshOperation(void){}
		virtual void run(const CMRRect & zone) = 0;
};

/*********************  CLASS  **********************/
template <class T,class U>
class CMRMeshOperationSimpleLoop : public CMRMeshOperation
{
	public:
		CMRMeshOperationSimpleLoop(CMRVarSystem * sys);
		virtual void run (const CMRRect& zone );
	private:
		CMRVarSystem * sys;
};

/*******************  FUNCTION  *********************/
template <class T,class U>
CMRMeshOperationSimpleLoop<T,U>::CMRMeshOperationSimpleLoop (CMRVarSystem * sys)
{
	//errors
	assert(sys != NULL);
	
	//setup
	this->sys = sys;
}

/*******************  FUNCTION  *********************/
template <class T,class U>
void CMRMeshOperationSimpleLoop<T,U>::run ( const CMRRect & zone )
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
	debug("Start to compute on [ %d , %d : %d x %d ] into ",zone.x,zone.y,zone.width,zone.height);
	CMRRect localZone = zone;
	assert(sys->getDomain(0,0)->getMemoryRect().contains(localZone));
	const typename T::CellAccessor cellIn(*sys,CMR_PREV_STEP,localZone.x,localZone.y,true);
	typename T::CellAccessor cellOut(*sys,CMR_CURRENT_STEP,localZone.x,localZone.y,true);
	
	for(int y = 0 ; y < localZone.height ; y++)
		for(int x = 0 ; x < localZone.width ; x++)
			U::cellAction(cellIn,cellOut,x,y);
}

/*********************  CLASS  **********************/
template <class T,class U>
class CMRMeshOperationSimpleLoopInPlace : public CMRMeshOperation
{
	public:
		CMRMeshOperationSimpleLoopInPlace(CMRVarSystem * sys);
		virtual void run (const CMRRect& zone );
	private:
		CMRVarSystem * sys;
};

/*******************  FUNCTION  *********************/
template <class T,class U>
CMRMeshOperationSimpleLoopInPlace<T,U>::CMRMeshOperationSimpleLoopInPlace (CMRVarSystem * sys)
{
	//errors
	assert(sys != NULL);
	
	//setup
	this->sys = sys;
}

/*******************  FUNCTION  *********************/
template <class T,class U>
void CMRMeshOperationSimpleLoopInPlace<T,U>::run ( const CMRRect & zone )
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
	debug("Start to compute on [ %d , %d : %d x %d ] into ",zone.x,zone.y,zone.width,zone.height);
	CMRRect localZone = zone;
	assert(sys->getDomain(0,0)->getMemoryRect().contains(localZone));
	typename T::CellAccessor cell(*sys,CMR_PREV_STEP,localZone.x,localZone.y,true);
	
	for(int y = 0 ; y < localZone.height ; y++)
		for(int x = 0 ; x < localZone.width ; x++)
			U::cellAction(cell,x,y);
}

/*******************  FUNCTION  *********************/
struct CMRCellPosition
{
	CMRCellPosition(const CMRRect & globalMesh,const CMRRect & localMesh,const CMRVect2D & cellPos);
	CMRCellPosition(const CMRRect & globalMesh,const CMRRect & localMesh,int x,int y);
	CMRCellPosition(const CMRCellPosition & orig,int dx,int dy);
	int getAbsX(int dx) const;
	int getAbsY(int dy) const;
	bool cellExist(int dx,int dy,int ghostDepth) const;
	CMRRect localMesh;
	CMRRect globalMesh;
	CMRVect2D cellPos;
};

/*******************  FUNCTION  *********************/
CMRCellPosition::CMRCellPosition ( const CMRRect & globalMesh, const CMRRect & localMesh, const CMRVect2D & cellPos )
{
	this->globalMesh = globalMesh;
	this->localMesh = localMesh;
	this->cellPos = cellPos;
}

/*******************  FUNCTION  *********************/
CMRCellPosition::CMRCellPosition ( const CMRRect & globalMesh, const CMRRect & localMesh, int x, int y )
{
	this->globalMesh = globalMesh;
	this->localMesh = localMesh;
	this->cellPos.x = x;
	this->cellPos.y = y;
}

/*******************  FUNCTION  *********************/
CMRCellPosition::CMRCellPosition ( const CMRCellPosition& orig, int dx, int dy )
{
	*this = orig;
	this->cellPos.x+=dx;
	this->cellPos.y+=dy;
}

/*******************  FUNCTION  *********************/
int CMRCellPosition::getAbsX ( int dx ) const
{
	return localMesh.x + cellPos.x + dx;
}

/*******************  FUNCTION  *********************/
int CMRCellPosition::getAbsY ( int dy ) const
{
	return localMesh.y + cellPos.y + dy;
}

/*******************  FUNCTION  *********************/
bool CMRCellPosition::cellExist ( int dx, int dy, int ghostDepth ) const
{
	int posX = cellPos.x + dx;
	int posY = cellPos.y + dy;
	return ((posX >= -ghostDepth && posX <= localMesh.width  + ghostDepth)
         && (posY >= -ghostDepth && posY <= localMesh.height + ghostDepth));
}

/*******************  FUNCTION  *********************/
template <class T,class U>
class CMRMeshOperationSimpleLoopWithPos : public CMRMeshOperation
{
	public:
		CMRMeshOperationSimpleLoopWithPos(CMRVarSystem * sys);
		virtual void run (const CMRRect& zone );
	private:
		CMRVarSystem * sys;
};

/*******************  FUNCTION  *********************/
template <class T,class U>
CMRMeshOperationSimpleLoopWithPos<T,U>::CMRMeshOperationSimpleLoopWithPos ( CMRVarSystem * sys )
{
	//errors
	assert(sys != NULL);
	
	//setup
	this->sys = sys;
}

/*******************  FUNCTION  *********************/
template <class T,class U>
void CMRMeshOperationSimpleLoopWithPos<T,U>::run ( const CMRRect & zone )
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
	debug("Start to compute on [ %d , %d : %d x %d ]",zone.x,zone.y,zone.width,zone.height);
	CMRRect localZone = zone;
	assert(sys->getDomain(0,0)->getMemoryRect().contains(localZone));
	typename T::CellAccessor cellIn(*sys,CMR_PREV_STEP,localZone.x,localZone.y);
	typename T::CellAccessor cellOut(*sys,CMR_CURRENT_STEP,localZone.x,localZone.y);
	CMRCellPosition pos(sys->getDomain(0,0)->getGlobalRect(),sys->getDomain(0,0)->getLocalDomainRect(),localZone.x,localZone.y);
	
	for(int y = 0 ; y < localZone.height ; y++)
		for(int x = 0 ; x < localZone.width ; x++)
			U::cellAction(cellIn,cellOut,pos,x,y);
}

#endif // CMR_SPACE_SPLITTER_H
