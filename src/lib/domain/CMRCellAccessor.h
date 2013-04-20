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
		CMRCellAccessor( CMRCellAccessor<DataType,MemoryModel> & orig,int dx,int dy, bool absolute = false);
		DataType * operator()(int dx,int dy);
		const DataType * operator()(int dx,int dy) const;
		size_t getTypeSize(void) const;
		bool isContiguous(const CMRRect & rect) const;
		CMRVect2D getAbsPosition(int dx = 0 , int dy = 0) const;
	private:
		void setPosition(int dx,int dy,bool absolute);
		CMRVect2D position;
};

/*******************  FUNCTION  *********************/
//TODO check usage, maybe this is a hack
template <class DataType,class MemoryModel>
CMRCellAccessor<DataType,MemoryModel>::CMRCellAccessor ( CMRDomainStorage& orig, int dx, int dy, bool absolute)
{
	//get accessor
	//warning("Provide a compat check system !");
	#warning Provide a compat check system !
	
	//if acc is NULL, setup a new one compatible with current model
	if (orig.hasMemoryAccessor() == false)
		orig.setMemoryAccessor(new CMRGenericMemoryAccessor<DataType,MemoryModel>());
	
	//setup memory domain
	CMRAbstractMemoryAccessor * acc = &orig.getMemoryAccessor();
	assert(acc != NULL);
	this->set(*acc);
	
	//setup all
	this->setPosition(dx,dy,absolute);
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
CMRCellAccessor<DataType,MemoryModel>::CMRCellAccessor ( CMRDomainMemory& orig, int dx, int dy, bool absolute)
	:CMRDomainMemory(orig)
{
	this->setPosition(dx,dy,absolute);
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
CMRCellAccessor<DataType,MemoryModel>::CMRCellAccessor ( CMRCellAccessor< DataType, MemoryModel >& orig, int dx, int dy , bool absolute) 
	:CMRDomainMemory(orig)
{
	position = orig.position;
	this->setPosition(dx,dy,absolute);
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
CMRVect2D CMRCellAccessor<DataType,MemoryModel>::getAbsPosition ( int dx, int dy ) const
{
	return position.getRel(dx+memoryRect.x,dy+memoryRect.y);
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
void CMRCellAccessor<DataType,MemoryModel>::setPosition ( int dx, int dy, bool absolute )
{
	if (absolute)
	{
		this->position.x = dx - memoryRect.x;
		this->position.y = dy - memoryRect.y;
	} else {
		this->position.x += dx;
		this->position.y += dy;
	}
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
DataType * CMRCellAccessor<DataType,MemoryModel>::operator() ( int dx, int dy )
{
	//errors
	assert(ptr != NULL);
	
	int id = MemoryModel::getCellId(position.x + dx,position.y + dy,memoryRect.width,memoryRect.height);
	return ((DataType*)ptr) + (id);
}

/*******************  FUNCTION  *********************/
template <class DataType,class MemoryModel>
const DataType* CMRCellAccessor<DataType,MemoryModel>::operator() ( int dx, int dy ) const
{
	//errors
	assert(ptr != NULL);
	
	int id = MemoryModel::getCellId(position.x + dx,position.y + dy,memoryRect.width,memoryRect.height);
	return ((DataType*)ptr)+(id);
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
