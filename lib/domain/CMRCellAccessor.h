/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 02/2013
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_CELL_ACCESSOR_H
#define CMR_CELL_ACCESSOR_H

/********************  HEADERS  *********************/
#include <vector>
#include <cstdlib>
#include <cassert>
#include "CMRAbstractDomain.h"
#include "../common/CMRDebug.h"
#include "../common/CMRGeometry.h"
#include "CMRDomainMemory.h"

/*********************  CLASS  **********************/
template <class DataType,class MemoryModel>
class CMRCellAccessor : public CMRDomainMemory
{
	public:
		CMRCellAccessor( const CMRDomainMemory& orig, int dx, int dy );
		CMRCellAccessor(const CMRCellAccessor<DataType,MemoryModel> & orig,int dx,int dy);
		DataType & getCell(int dx,int dy);
		const DataType & getCell(int dx,int dy) const;
		size_t getTypeSize(void) const;
		bool isContiguous(const CMRRect & rect) const;
};

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
CMRCellAccessor<DataType,MemoryModel>::CMRCellAccessor ( const CMRDomainMemory& orig, int dx, int dy )
	:CMRDomainMemory(orig)
{
	this->ptr = &getCell(dx,dy);
	this->ptrAbsPosition.x += dx;
	this->ptrAbsPosition.y += dy;
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
CMRCellAccessor<DataType,MemoryModel>::CMRCellAccessor ( const CMRCellAccessor< DataType, MemoryModel >& orig, int dx, int dy ) 
	: CMRDomainMemory(orig)
{
	this->ptr = &getCell(dx,dy);
	this->ptrAbsPosition.x += dx;
	this->ptrAbsPosition.y += dy;
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
DataType& CMRCellAccessor<DataType,MemoryModel>::getCell ( int dx, int dy )
{
	//errors
	assert(ptr != NULL);
	
	int id = MemoryModel::getRelCellId(dx,dy,memoryRect.width,memoryRect.height,ptrAbsPosition.x,ptrAbsPosition.y);
	return ((DataType*)ptr)[id];
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
const DataType& CMRCellAccessor<DataType,MemoryModel>::getCell ( int dx, int dy ) const
{
	//errors
	assert(ptr != NULL);
	
	int id = MemoryModel::getRelCellId(dx,dy,memoryRect.width,memoryRect.height,ptrAbsPosition.x,ptrAbsPosition.y);
	return ((DataType*)ptr)[id];
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
size_t CMRCellAccessor<DataType,MemoryModel>::getTypeSize ( void ) const
{
	return sizeof(DataType);
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
bool CMRCellAccessor<DataType,MemoryModel>::isContiguous ( const CMRRect& rect ) const
{
	return MemoryModel::isContiguous(memoryRect,rect);
}

#endif // CMR_CELL_ACCESSOR_H
