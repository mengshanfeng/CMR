/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 02/2013
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_MEMORY_MODEL_H
#define CMR_MEMORY_MODEL_H

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
class CMRMemoryAccessor : public CMRDomainMemory
{
	public:
		CMRMemoryAccessor( const CMRDomainMemory& orig, int dx, int dy );
		CMRMemoryAccessor(const CMRMemoryAccessor<DataType,MemoryModel> & orig,int dx,int dy);
		DataType & getCell(int dx,int dy);
		const DataType & getCell(int dx,int dy) const;
		int getTypeSize(void) const;
		bool isContiguous(const CMRRect & rect) const;
};


/*********************  CLASS  **********************/
struct CMRMemoryModelRowMajor
{
	static inline int getCellId(int x,int y,int width,int height);
	static inline int getRelCellId(int dx,int dy,int width,int height,int x,int y);
	static inline bool isContiguous(const CMRRect & memoryRect, const CMRRect & rect);
	//template <class T> int copyFromBuffer(T * mesh, const void* buffer, size_t size, const CMRRect & memoryRect, const CMRRect & rect);
	//template <class T> int copyToBuffer( const T * mesh,void* buffer, size_t size, const CMRRect & memoryRect, const CMRRect & rect);
};

/*******************  FUNCTION  *********************/
int CMRMemoryModelRowMajor::getCellId ( int x, int y, int width, int height )
{
	//errors
	assert(x >= 0 && x < width);
	assert(y >= 0 && y < height);
	
	return y * width + x;
}

/*******************  FUNCTION  *********************/
int CMRMemoryModelRowMajor::getRelCellId ( int dx, int dy, int width, int height, int x, int y )
{
	//errors
	assert(x+dx >= 0 && x+dx < width);
	assert(y+dy >= 0 && y+dy < height);
	
	return dy * width + dx;
}

/*******************  FUNCTION  *********************/
bool CMRMemoryModelRowMajor::isContiguous ( const CMRRect& memoryRect, const CMRRect& rect )
{
	debug_warning(memoryRect.contains(rect),"Caution, isContiguous is used with rect which didn't fit in memoryRect !");
	return (rect.height == 1 || rect.width == memoryRect.width);
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
CMRMemoryAccessor<DataType,MemoryModel>::CMRMemoryAccessor ( const CMRDomainMemory& orig, int dx, int dy )
	:CMRDomainMemory(orig)
{
	this->ptr = &getCell(dx,dy);
	this->ptrAbsPosition.x += dx;
	this->ptrAbsPosition.y += dy;
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
CMRMemoryAccessor<DataType,MemoryModel>::CMRMemoryAccessor ( const CMRMemoryAccessor< DataType, MemoryModel >& orig, int dx, int dy ) 
	: CMRDomainMemory(orig)
{
	this->ptr = &getCell(dx,dy);
	this->ptrAbsPosition.x += dx;
	this->ptrAbsPosition.y += dy;
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
DataType& CMRMemoryAccessor<DataType,MemoryModel>::getCell ( int dx, int dy )
{
	//errors
	assert(ptr != NULL);
	
	int id = MemoryModel::getRelCellId(dx,dy,memoryRect.width,memoryRect.height,ptrAbsPosition.x,ptrAbsPosition.y);
	return ((DataType*)ptr)[id];
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
const DataType& CMRMemoryAccessor<DataType,MemoryModel>::getCell ( int dx, int dy ) const
{
	//errors
	assert(ptr != NULL);
	
	int id = MemoryModel::getRelCellId(dx,dy,memoryRect.width,memoryRect.height,ptrAbsPosition.x,ptrAbsPosition.y);
	return ((DataType*)ptr)[id];
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
int CMRMemoryAccessor<DataType,MemoryModel>::getTypeSize ( void ) const
{
	return sizeof(DataType);
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
bool CMRMemoryAccessor<DataType,MemoryModel>::isContiguous ( const CMRRect& rect ) const
{
	return MemoryModel::isContiguous(memoryRect,rect);
}

#endif // CMR_SPACE_SPLITTER_H
