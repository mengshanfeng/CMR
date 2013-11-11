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
		virtual void run(CMRVarSystem * sys, const CMRRect & zone) = 0;
};

/*********************  CLASS  **********************/
template <class T,class U>
class CMRMeshOperationSimpleLoop : public CMRMeshOperation
{
	public:
		virtual void run (CMRVarSystem * sys, const CMRRect& zone );
};

/*******************  FUNCTION  *********************/
template <class T,class U>
void CMRMeshOperationSimpleLoop<T,U>::run ( CMRVarSystem * sys, const CMRRect & zone )
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
	//local copy to avoid deref
	CMRRect memoryRect(sys->getDomain(0,0)->getMemoryRect());
	CMRRect localZone(memoryRect.intersect(zone));
	
	debug("Start to compute on [ %d , %d : %d x %d ] into [ %d , %d : %d x %d ]",zone.x,zone.y,zone.width,zone.height,memoryRect.x,memoryRect.y,memoryRect.width,memoryRect.height);
	
	//if not in zone, exit
	if (localZone.surface() == 0)
	{
		debug("Caution, you request an operation out of local zone, do nothing.");
		return;
	}
	
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
		CMRMeshOperationSimpleLoopInPlace(const U * action = NULL);
		~CMRMeshOperationSimpleLoopInPlace(void) {if (action != NULL) delete action;};
		virtual void run (CMRVarSystem * sys,const CMRRect& zone);
	private:
		const U * action;
};

/*******************  FUNCTION  *********************/
template <class T,class U>
CMRMeshOperationSimpleLoopInPlace<T,U>::CMRMeshOperationSimpleLoopInPlace (const U * action)
{
	//setup
	this->action = action;
}

/*******************  FUNCTION  *********************/
template <class T,class U>
void CMRMeshOperationSimpleLoopInPlace<T,U>::run ( CMRVarSystem * sys, const CMRRect & zone )
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
	CMRRect memoryRect(sys->getDomain(0,0)->getMemoryRect());
	CMRRect localZone(memoryRect.intersect(zone));
	
	debug("Start to compute on [ %d , %d : %d x %d ] into [ %d , %d : %d x %d ]",zone.x,zone.y,zone.width,zone.height,memoryRect.x,memoryRect.y,memoryRect.width,memoryRect.height);
	
	//if not in zone, exit
	if (localZone.surface() == 0)
	{
		debug("Caution, you request an operation out of local zone, do nothing.");
		return;
	}
	
	typename T::CellAccessor cell(*sys,CMR_PREV_STEP,localZone.x,localZone.y,true);
	const U * loacalAction = action;
	
	for(int y = 0 ; y < localZone.height ; y++)
		for(int x = 0 ; x < localZone.width ; x++)
			loacalAction->cellAction(cell,x,y);
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
inline CMRCellPosition::CMRCellPosition ( const CMRRect & globalMesh, const CMRRect & localMesh, const CMRVect2D & cellPos )
{
	this->globalMesh = globalMesh;
	this->localMesh = localMesh;
	this->cellPos = cellPos;
}

/*******************  FUNCTION  *********************/
inline CMRCellPosition::CMRCellPosition ( const CMRRect & globalMesh, const CMRRect & localMesh, int x, int y )
{
	this->globalMesh = globalMesh;
	this->localMesh = localMesh;
	this->cellPos.x = x;
	this->cellPos.y = y;
}

/*******************  FUNCTION  *********************/
inline CMRCellPosition::CMRCellPosition ( const CMRCellPosition& orig, int dx, int dy )
{
	*this = orig;
	this->cellPos.x+=dx;
	this->cellPos.y+=dy;
}

/*******************  FUNCTION  *********************/
inline int CMRCellPosition::getAbsX ( int dx ) const
{
	return localMesh.x + cellPos.x + dx;
}

/*******************  FUNCTION  *********************/
inline int CMRCellPosition::getAbsY ( int dy ) const
{
	return localMesh.y + cellPos.y + dy;
}

/*******************  FUNCTION  *********************/
inline bool CMRCellPosition::cellExist ( int dx, int dy, int ghostDepth ) const
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
		virtual void run (CMRVarSystem * sys,const CMRRect& zone );
};

/*******************  FUNCTION  *********************/
template <class T,class U>
void CMRMeshOperationSimpleLoopWithPos<T,U>::run ( CMRVarSystem * sys,const CMRRect & zone )
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
	//local copy to avoid deref
	CMRRect memoryRect(sys->getDomain(0,0)->getMemoryRect());
	CMRRect localZone(memoryRect.intersect(zone));
	
	debug("Start to compute on [ %d , %d : %d x %d ] into [ %d , %d : %d x %d ]",zone.x,zone.y,zone.width,zone.height,memoryRect.x,memoryRect.y,memoryRect.width,memoryRect.height);
	
	//if not in zone, exit
	if (localZone.surface() == 0)
	{
		debug("Caution, you request an operation out of local zone, do nothing.");
		return;
	}
	
	typename T::CellAccessor cellIn(*sys,CMR_PREV_STEP,localZone.x,localZone.y);
	typename T::CellAccessor cellOut(*sys,CMR_CURRENT_STEP,localZone.x,localZone.y);
	CMRCellPosition pos(sys->getDomain(0,0)->getGlobalRect(),sys->getDomain(0,0)->getLocalDomainRect(),localZone.x,localZone.y);
	
	for(int y = 0 ; y < localZone.height ; y++)
		for(int x = 0 ; x < localZone.width ; x++)
			U::cellAction(cellIn,cellOut,pos,x,y);
}

/*******************  FUNCTION  *********************/
template <class T,class U>
class CMRMeshReduction : public CMRMeshOperation
{
	public:
		CMRMeshReduction(U * reduction);
		virtual void run (CMRVarSystem * sys ,const CMRRect& zone );
		U * gblReduction;
};

/*******************  FUNCTION  *********************/
template <class T,class U>
CMRMeshReduction<T,U>::CMRMeshReduction ( U * reduction)
{
	//errors
	assert(reduction != NULL);
	
	//setup
	this->gblReduction = reduction;
}

/*******************  FUNCTION  *********************/
template <class T,class U>
void CMRMeshReduction<T,U>::run ( CMRVarSystem * sys , const CMRRect & zone )
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
	//local copy to avoid deref
	CMRRect memoryRect(sys->getDomain(0,0)->getMemoryRect());
	CMRRect localZone(memoryRect.intersect(zone));
	
	debug("Start to compute on [ %d , %d : %d x %d ] into [ %d , %d : %d x %d ]",zone.x,zone.y,zone.width,zone.height,memoryRect.x,memoryRect.y,memoryRect.width,memoryRect.height);
	
	//if not in zone, exit
	if (localZone.surface() == 0)
	{
		debug("Caution, you request an operation out of local zone, do nothing.");
		return;
	}

	typename T::CellAccessor cell(*sys,CMR_PREV_STEP,localZone.x,localZone.y);
	U reduction;

	for(int y = 0 ; y < localZone.height ; y++)
		for(int x = 0 ; x < localZone.width ; x++)
			reduction.cellAction(cell,x,y);

	gblReduction->doReduce(reduction);
}

#endif // CMR_SPACE_SPLITTER_H
