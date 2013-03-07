/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 02/2013
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_GENERIC_MEMORY_ACCESSOR_H
#define CMR_GENERIC_MEMORY_ACCESSOR_H

/********************  HEADERS  *********************/
#include <cassert>
#include <cstdlib>
#include "../common/CMRGeometry.h"
#include "CMRDomainMemory.h"

/*********************  CLASS  **********************/
class CMRAbstractMemoryAccessor : public CMRDomainMemory
{
	public:
		virtual ~CMRAbstractMemoryAccessor(void) {};
		virtual int getTypeSize(void) const = 0;
		virtual bool isContiguous(const CMRRect & rect) const = 0;
		virtual int copyFromBuffer ( const void* buffer, size_t size, const CMRRect & rect ) = 0;
		virtual int copyToBuffer ( void* buffer, size_t size, const const CMRRect & rect ) const = 0;
		virtual size_t getBufferSize( const CMRRect & rect) const = 0;
};

/*********************  CLASS  **********************/
template <class DataType,class MemoryModel>
class CMRGenericMemoryAccessor : public CMRAbstractMemoryAccessor
{
	public:
		virtual ~CMRGenericMemoryAccessor(void) {};
		virtual int getTypeSize(void) const;
		virtual bool isContiguous(const CMRRect & rect) const;
		virtual int copyFromBuffer ( const void* buffer, size_t size, const CMRRect & rect );
		virtual int copyToBuffer ( void* buffer, size_t size, const CMRRect & rect ) const;
		virtual size_t getBufferSize( const CMRRect & rect) const;
};

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
bool CMRGenericMemoryAccessor<DataType,MemoryModel>::isContiguous ( const CMRRect& rect ) const
{
	return MemoryModel::isContiguous(this->memoryRect,rect);
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
size_t CMRGenericMemoryAccessor<DataType,MemoryModel>::getBufferSize ( const CMRRect& rect ) const
{
	return rect.surface() * getTypeSize();
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
int CMRGenericMemoryAccessor<DataType,MemoryModel>::getTypeSize ( void ) const
{
	return sizeof(DataType);
}

/*******************  FUNCTION  *********************/
/** @TODO maybe prefer a generic version which deps only on model and type size, but not type **/
template <class DataType,class MemoryModel>
int CMRGenericMemoryAccessor<DataType,MemoryModel>::copyToBuffer ( void* buffer, size_t size, const CMRRect& rect ) const
{
	//errors
	assert(memoryRect.contains(rect));
	assert(size >= getBufferSize());
	
	//local
	int xMin = rect.x - ptrAbsPosition.x;
	int yMin = rect.y - ptrAbsPosition.y;
	int xMax = rect.width;
	int yMax = rect.height;
	int rectW = rect.width;
	int memW = memoryRect.width;
	int memH = memoryRect.height;
	
	//ptrs
	DataType * typedBuffer = buffer;
	const DataType * typedDomain = ptr;
	
	//copy
	for (int y = yMin ; y < yMax ; y++)
		for (int x = xMin ; x < xMax ; x++)
			typedBuffer[y*rectW + x] = typedDomain[MemoryModel::getRelCellId(x,y,memW,memH,ptrAbsPosition.x,ptrAbsPosition.y)];

	//return current used size
	return getBufferSize();
}

/*******************  FUNCTION  *********************/
/** @TODO maybe prefer a generic version which deps only on model and type size, but not type **/
template <class DataType,class MemoryModel>
int CMRGenericMemoryAccessor<DataType,MemoryModel>::copyFromBuffer ( const void* buffer, size_t size, const CMRRect& rect )
{
	//errors
	assert(memoryRect.contains(rect));
	assert(size >= getBufferSize());
	
	//local
	int xMin = rect.x - ptrAbsPosition.x;
	int yMin = rect.y - ptrAbsPosition.y;
	int xMax = rect.width;
	int yMax = rect.height;
	int rectW = rect.width;
	int memW = memoryRect.width;
	int memH = memoryRect.height;
	
	//ptrs
	const DataType * typedBuffer = buffer;
	DataType * typedDomain = ptr;
	
	//copy
	for (int y = yMin ; y < yMax ; y++)
		for (int x = xMin ; x < xMax ; x++)
			typedDomain[MemoryModel::getRelCellId(x,y,memW,memH,ptrAbsPosition.x,ptrAbsPosition.y)] = typedBuffer[y*rectW + x];

	//return current used size
	return getBufferSize();
}

#endif //CMR_GENERIC_MEMORY_ACCESSOR_H
