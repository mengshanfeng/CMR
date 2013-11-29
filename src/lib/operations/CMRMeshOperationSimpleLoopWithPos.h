/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 02/2013
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_MESH_OPERATION_SIMPLE_LOOP_WITH_POS_H
#define CMR_MESH_OPERATION_SIMPLE_LOOP_WITH_POS_H

/********************  HEADERS  *********************/
#include "CMRMeshOperation.h"
#include "operations/CMRCellPosition.h"

/*******************  FUNCTION  *********************/
template <class T,class U>
class CMRMeshOperationSimpleLoopWithPos : public CMRMeshOperation
{
	public:
		CMRMeshOperationSimpleLoopWithPos(const U * action = NULL);
		~CMRMeshOperationSimpleLoopWithPos(void);
		virtual void run (CMRVarSystem * sys,const CMRRect& zone );
	protected:
		const U * action;
};

/*******************  FUNCTION  *********************/
template <class T,class U>
CMRMeshOperationSimpleLoopWithPos<T,U>::CMRMeshOperationSimpleLoopWithPos(const U * action)
	:CMRMeshOperation(true)
{
	this->action = action;
}

/*******************  FUNCTION  *********************/
template <class T,class U>
CMRMeshOperationSimpleLoopWithPos<T,U>::~CMRMeshOperationSimpleLoopWithPos(void)
{
	if (action != NULL) 
		delete action;
}

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
	
	const typename T::CellAccessor cellIn(*sys,CMR_PREV_STEP,localZone.x,localZone.y);
	typename T::CellAccessor cellOut(*sys,CMR_CURRENT_STEP,localZone.x,localZone.y);
	CMRCellPosition pos(sys->getDomain(0,0)->getGlobalRect(),sys->getDomain(0,0)->getLocalDomainRect(),localZone.x,localZone.y);
	const U * localAction = action;
	
	for(int y = 0 ; y < localZone.height ; y++)
		for(int x = 0 ; x < localZone.width ; x++)
			localAction->cellAction(cellIn,cellOut,pos,x,y);
}


#endif //CMR_MESH_OPERATION_SIMPLE_LOOP_WITH_POS_H
