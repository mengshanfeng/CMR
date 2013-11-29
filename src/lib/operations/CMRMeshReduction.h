/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 02/2013
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_MESH_REDUCTION_H
#define CMR_MESH_REDUCTION_H

/********************  HEADERS  *********************/
#include "CMRMeshOperation.h"

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

	typename T::CellAccessor cell(*sys,CMR_CURRENT_STEP,localZone.x,localZone.y);
	U reduction;

	for(int y = 0 ; y < localZone.height ; y++)
		for(int x = 0 ; x < localZone.width ; x++)
			reduction.cellAction(cell,x,y);

	gblReduction->doReduce(reduction);
}

#endif //CMR_MESH_REDUCTION_H
