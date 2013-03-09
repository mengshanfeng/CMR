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
struct CMRRect;

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
		CMRMeshOperationSimpleLoop(T * in, T * out);
		virtual void run (const CMRRect& zone );
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
	CMRRect localZone = zone;
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

/*******************  FUNCTION  *********************/
struct CMRCellPosition
{
	CMRCellPosition(const CMRRect & globalMesh,const CMRRect & localMesh,const CMRVect2D & cellPos);
	CMRCellPosition(const CMRRect & globalMesh,const CMRRect & localMesh,int x,int y);
	CMRCellPosition(const CMRCellPosition & orig,int dx,int dy);
	int getAbsX(int dx = 0) const;
	int getAbsY(int dy = 0) const;
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
		CMRMeshOperationSimpleLoopWithPos(T * in, T * out);
		virtual void run (const CMRRect& zone );
	private:
		T * in;
		T * out;
};

/*******************  FUNCTION  *********************/
template <class T,class U>
CMRMeshOperationSimpleLoopWithPos<T,U>::CMRMeshOperationSimpleLoopWithPos ( T * in, T * out )
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
	CMRRect localZone = zone;
	typename T::CellAccessor cellIn(*in,localZone.x,localZone.y);
	typename T::CellAccessor cellOut(*out,localZone.x,localZone.y);
	CMRCellPosition pos(in->getGlobalRect(),in->getLocalRect(),localZone.x,localZone.y);
	
	for(int y = 0 ; y < localZone.height ; y++)
	{
		for(int x = 0 ; x < localZone.width ; x++)
		{
			typename T::CellAccessor cellLocalIn(cellIn,x,y);
			typename T::CellAccessor cellLocalOut(cellOut,x,y);
			CMRCellPosition cellPos(pos,x,y);
			U::cellAction(cellLocalIn,cellLocalOut,cellPos);
		}
	}
}

#endif // CMR_SPACE_SPLITTER_H
