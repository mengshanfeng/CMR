/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 02/2013
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_CELL_POSITION_H
#define CMR_CELL_POSITION_H

/********************  HEADERS  *********************/
#include "common/CMRGeometry.h"

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

#endif //CMR_CELL_POSITION_H
