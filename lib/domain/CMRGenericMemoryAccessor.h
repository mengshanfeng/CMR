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
#include "../common/CMRDebug.h"
#include "../common/CMRGeometry.h"
#include "CMRAbstractMemoryAccessor.h"
#include <cstring>
#include <string>
#include <sstream>

/*********************  CLASS  **********************/
template <class DataType,class MemoryModel>
class CMRGenericMemoryAccessor : public CMRAbstractMemoryAccessor
{
	public:
		CMRGenericMemoryAccessor(void) {};
		CMRGenericMemoryAccessor(CMRDomainMemory & memoryDomain) : CMRAbstractMemoryAccessor(memoryDomain) {};
		virtual ~CMRGenericMemoryAccessor(void) {};
		size_t getTypeSize( void ) const;
		virtual bool isContiguous(const CMRRect & rect) const;
		virtual size_t copyFromBuffer ( const void* buffer, size_t size, const CMRRect & rect );
		virtual size_t copyToBuffer ( void* buffer, size_t size, const CMRRect & rect ) const;
		virtual size_t getBufferSize( const CMRRect & rect) const;
		virtual const void* getCell ( int x, int y ) const;
		virtual void* getCell ( int x, int y );
		virtual void printDebug( const CMRRect & rect ) const;
};

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
const void* CMRGenericMemoryAccessor<DataType,MemoryModel>::getCell ( int x, int y ) const
{
	//errors
	assert(ptr != NULL);
	
	//return cell
	const DataType * typedPtr = (const DataType*) ptr;
	int id = MemoryModel::getCellId(x,y,memoryRect.width,memoryRect.height);
	return (typedPtr+id);
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
void* CMRGenericMemoryAccessor<DataType,MemoryModel>::getCell ( int x, int y )
{
	//errors
	assert(ptr != NULL);
	
	//return cell
	DataType * typedPtr = (DataType*)ptr;
	int id = MemoryModel::getCellId(x - memoryRect.x,y - memoryRect.y,memoryRect.width,memoryRect.height);
	return typedPtr+id;
}

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
size_t CMRGenericMemoryAccessor<DataType,MemoryModel>::getTypeSize ( void ) const
{
	return sizeof(DataType);
}

/*******************  FUNCTION  *********************/
/** @TODO maybe prefer a generic version which deps only on model and type size, but not type **/
template <class DataType,class MemoryModel>
size_t CMRGenericMemoryAccessor<DataType,MemoryModel>::copyToBuffer ( void* buffer, size_t size, const CMRRect & rect ) const
{
	//errors
	assert(memoryRect.contains(rect));
	assert(size >= getBufferSize(rect));
	assert(ptr != NULL);
	
	//local
	CMRRect localRect = rect.relativeTo(memoryRect);
	
	//ptrs
	DataType * typedBuffer = (DataType*) buffer;
	const DataType * typedDomain = (const DataType*) ptr;
	
	//copy
	for (int y = 0 ; y < rect.height ; y++)
	{
		for (int x = 0 ; x < rect.width ; x++)
		{
			void * dest = &typedBuffer[y*rect.width + x];
			const void * src = &typedDomain[MemoryModel::getCellId(x + localRect.x,y + localRect.y,memoryRect.width,memoryRect.height)];
			/** @TODO optimize small sizes **/
			memcpy(dest,src,sizeof(DataType));
		}
	}

	//return current used size
	return getBufferSize(rect);
}

/*******************  FUNCTION  *********************/
/** @TODO maybe prefer a generic version which deps only on model and type size, but not type **/
template <class DataType,class MemoryModel>
size_t CMRGenericMemoryAccessor<DataType,MemoryModel>::copyFromBuffer ( const void* buffer, size_t size, const CMRRect & rect )
{
	//errors
	assert(memoryRect.contains(rect));
	assert(size >= getBufferSize(rect));
	assert(ptr != NULL);
	
	//local
	CMRRect localRect = rect.relativeTo(memoryRect);
	
	//ptrs
	const DataType * typedBuffer = (const DataType*) buffer;
	DataType * typedDomain = (DataType*) ptr;
	
	//copy
	for (int y = 0 ; y < rect.height ; y++)
	{
		for (int x = 0 ; x < rect.width ; x++)
		{
			void * dest = &typedDomain[MemoryModel::getCellId(x + localRect.x,y + localRect.y,memoryRect.width,memoryRect.height)];
			const void * src = &typedBuffer[y*rect.width + x];
			/** @TODO optimize small sizes **/
			memcpy(dest,src,sizeof(DataType));
		}
	}

	//return current used size
	return getBufferSize(rect);
}

/*******************  FUNCTION  *********************/
template <class T>
std::string cmrTypeToString(const T & value)
{
	std::stringstream buffer;
	buffer << value;
	return buffer.str();
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
void CMRGenericMemoryAccessor<DataType,MemoryModel>::printDebug ( const CMRRect& rect ) const
{
	//errors
	assert(memoryRect.contains(rect));
	assert(ptr != NULL);
	
	//fill buffer
	std::stringstream buffer;
	const DataType * typedDomain = (const DataType*) ptr;
	
	//local
	CMRRect localRect = rect.relativeTo(memoryRect);
	
	for (int y = 0 ; y < localRect.height ; y++)
	{
		for (int x = 0 ; x < localRect.width ; x++)
		{
			const DataType & cell = typedDomain[MemoryModel::getCellId(x + localRect.x,y + localRect.y,memoryRect.width,memoryRect.height)];
			buffer << cmrTypeToString(cell) << " ";
		}
		buffer << std::endl;
	}
	
	debug("Memory values in [ %d , %d : %d x %d ]\n-------------------------------------------------\n%s\n--------------------------------------------------",rect.x,rect.y,rect.width,rect.height,buffer.str().c_str());
}

#endif //CMR_GENERIC_MEMORY_ACCESSOR_H
