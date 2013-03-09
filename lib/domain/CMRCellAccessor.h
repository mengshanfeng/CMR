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
#include "CMRDomainStorage.h"
#include "CMRGenericMemoryAccessor.h"

/*********************  CLASS  **********************/
template <class DataType,class MemoryModel>
class CMRCellAccessor : public CMRDomainMemory
{
	public:
		CMRCellAccessor( CMRDomainStorage & orig, int dx, int dy, bool absolute = false);
		CMRCellAccessor( CMRDomainMemory& orig, int dx, int dy, bool absolute = false);
		CMRCellAccessor( const CMRCellAccessor<DataType,MemoryModel> & orig,int dx,int dy, bool absolute = false);
		DataType & getCell(int dx,int dy);
		const DataType & getCell(int dx,int dy) const;
		size_t getTypeSize(void) const;
		bool isContiguous(const CMRRect & rect) const;
	private:
		void move(int dx,int dy,bool absolute = false);
};

/*******************  FUNCTION  *********************/
//TODO check usage, maybe this is a hack
template <class DataType,class MemoryModel>
CMRCellAccessor<DataType,MemoryModel>::CMRCellAccessor ( CMRDomainStorage& orig, int dx, int dy, bool absolute)
{
	//get accessor
	warning("Provide a compat check system !");
	
	//if acc is NULL, setup a new one compatible with current model
	if (orig.hasMemoryAccessor() == false)
		orig.setMemoryAccessor(new CMRGenericMemoryAccessor<DataType,MemoryModel>());
	
	//setup memory domain
	CMRAbstractMemoryAccessor * acc = &orig.getMemoryAccessor();
	this->set(*acc);
	
	//setup all
	this->move(dx,dy,absolute);
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
CMRCellAccessor<DataType,MemoryModel>::CMRCellAccessor ( CMRDomainMemory& orig, int dx, int dy, bool absolute)
	:CMRDomainMemory(orig)
{
	this->move(dx,dy,absolute);
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
CMRCellAccessor<DataType,MemoryModel>::CMRCellAccessor ( const CMRCellAccessor< DataType, MemoryModel >& orig, int dx, int dy , bool absolute) 
	: CMRDomainMemory(orig)
{
	this->move(dx,dy,absolute);
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
void CMRCellAccessor<DataType,MemoryModel>::move ( int dx, int dy, bool absolute )
{
	if (absolute)
	{
		this->ptr = &getCell(dx - memoryRect.x,dy - memoryRect.y);
		this->ptrAbsPosition.x = dx;
		this->ptrAbsPosition.y = dy;
	} else {
		this->ptr = &getCell(dx,dy);
		this->ptrAbsPosition.x += dx;
		this->ptrAbsPosition.y += dy;
	}
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
DataType& CMRCellAccessor<DataType,MemoryModel>::getCell ( int dx, int dy )
{
	//errors
	assert(ptr != NULL);
	
	int id = MemoryModel::getRelCellId(dx,dy,memoryRect.width,memoryRect.height,ptrAbsPosition.x+dx-memoryRect.x,ptrAbsPosition.y+dy-memoryRect.y);
	return ((DataType*)ptr)[id];
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
const DataType& CMRCellAccessor<DataType,MemoryModel>::getCell ( int dx, int dy ) const
{
	//errors
	assert(ptr != NULL);
	
	int id = MemoryModel::getRelCellId(dx,dy,memoryRect.width,memoryRect.height,ptrAbsPosition.x+dx-memoryRect.x,ptrAbsPosition.y+dy-memoryRect.y);
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
