/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cassert>
#include "CMRDomainStorage.h"
#include "../common/CMRDebug.h"

/*******************  FUNCTION  *********************/
CMRDomainStorage::CMRDomainStorage ( size_t typeSize,const CMRRect & localDomain,int ghostDepth,int globalWidth,int globalHeight)
	: CMRAbstractDomain ( typeSize,localDomain,ghostDepth,globalWidth,globalHeight)
{
	//allocate the data
	this->data = new unsigned char[memoryRect.surface() * typeSize];
	this->acc = NULL;
}

/*******************  FUNCTION  *********************/
CMRDomainStorage::~CMRDomainStorage ( void )
{
	if (data != NULL)
		delete [] data;
	if (acc != NULL)
		delete acc;
}

/*******************  FUNCTION  *********************/
bool CMRDomainStorage::isContiguousGhost ( const CMRRect& rect ) const
{
	//warnings
	wassume(memoryRect.contains(rect),"You request a memory rect which didn't fit with current domain storage memory : %d , %d ; %d , %d",rect.x,rect.y,rect.width,rect.height);
	assert(dimensions == 2);
	assert(acc != NULL);
	//contiguous only if get one line
	return this->acc->isContiguous(rect);
}

/*******************  FUNCTION  *********************/
void* CMRDomainStorage::getContiguousGhost ( const CMRRect& rect )
{
	//errors
	assume(memoryRect.contains(rect),"You request a memory rect which didn't fit with current domain storage memory : %d , %d ; %d , %d",rect.x,rect.y,rect.width,rect.height);
	assert(dimensions == 2);
	assume(isContiguousGhost(rect),"You request a contiguous base address for a rect which cover non contiguous memory region : %d , %d ; %d , %d",rect.x,rect.y,rect.width,rect.height);
	//get offset
	return getCell(rect.x,rect.y);
}

/*******************  FUNCTION  *********************/
int CMRDomainStorage::getMemoryWidth ( void ) const
{
	return memoryRect.surface() * typeSize;
}

/*******************  FUNCTION  *********************/
int CMRDomainStorage::getMemoryCoord ( int x, int y ) const
{
	cmrNotImplemented("Need implementation of acc->getCellId().");
	//return this->acc->getCellId(x,y);
	return -1;
}

/*******************  FUNCTION  *********************/
void* CMRDomainStorage::getCell ( int x, int y )
{
	//errors
	assert(acc != NULL);
	return this->acc->getCell(x,y);
}

/*******************  FUNCTION  *********************/
int CMRDomainStorage::copyGhostFromBuffer ( const void* buffer, size_t size, const CMRRect& rect )
{
	//errors
	assert(dimensions == 2);
	assume(memoryRect.contains(rect),"You request a memory rect which didn't fit with current domain storage memory : %d , %d ; %d , %d",rect.x,rect.y,rect.width,rect.height);
	assume(size >= rect.width * rect.height * typeSize,"Invalid buffer size, too small : %lu.",size);
	assert(acc != NULL);
	
	return acc->copyFromBuffer(buffer,size,rect);
}

/*******************  FUNCTION  *********************/
int CMRDomainStorage::copyGhostToBuffer ( void* buffer, size_t size, const CMRRect& rect ) const
{
	//errors
	assert(dimensions == 2);
	assume(memoryRect.contains(rect),"You request a memory rect which didn't fit with current domain storage memory : %d , %d ; %d , %d",rect.x,rect.y,rect.width,rect.height);
	assume(size >= rect.width * rect.height * typeSize,"Invalid buffer size, too small : %lu.",size);
	assert(acc != NULL);

	return acc->copyToBuffer(buffer,size,rect);
}

/*******************  FUNCTION  *********************/
size_t CMRDomainStorage::getGhostSize ( const CMRRect& rect ) const
{
	//errors
	assert(dimensions == 2);
	assume(memoryRect.contains(rect),"You request a memory rect which didn't fit with current domain storage memory : %d , %d ; %d , %d",rect.x,rect.y,rect.width,rect.height);
	//need to rename the function maybe if no equal
	assert(rect.height * rect.width * typeSize == acc->getBufferSize(rect));
	//return
	return acc->getBufferSize(rect);
}

/*******************  FUNCTION  *********************/
CMRAbstractMemoryAccessor& CMRDomainStorage::getMemoryAccessor ( void )
{
	assert(acc != NULL);
	return *acc;
}

/*******************  FUNCTION  *********************/
bool CMRDomainStorage::hasMemoryAccessor ( void ) const
{
	return acc != NULL;
}

/*******************  FUNCTION  *********************/
void CMRDomainStorage::setMemoryAccessor ( CMRAbstractMemoryAccessor* acc )
{
	//errors
	assert(acc != NULL);
	
	//check compat
	assume(acc->getTypeSize() == typeSize,"Invalid type size for memory accessor, current is %lu and try to set new : %lu !",typeSize,acc->getTypeSize());
	warning("Need to check type compat here !");
	
	//delete current
	if (this->acc != NULL)
		delete this->acc;
	
	//setup
	this->acc = acc;
	
	//put data
	acc->set(data,memoryRect);
}
