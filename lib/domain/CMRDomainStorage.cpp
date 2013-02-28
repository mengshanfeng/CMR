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
	this->data = new char[memoryRect.surface() * typeSize];
}

/*******************  FUNCTION  *********************/
bool CMRDomainStorage::isContiguousGhost ( const CMRRect& rect ) const
{
	//warnings
	wassume(memoryRect.contains(rect),"You request a memory rect which didn't fit with current domain storage memory : %d , %d ; %d , %d",rect.x,rect.y,rect.width,rect.height);
	assert(dimensions == 2);
	//contiguous only if get one line
	return (rect.height == 1);
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
	//vars
	int w;
	int ghostDept = this->ghostDepth;

	//errors
	assert(memoryRect.contains(CMRVect2D(x,y)));
	assert(dimensions == 2);

	//fix coords for ghost
	x -= memoryRect.x;
	y -= memoryRect.y;
	w = memoryRect.width;

	//return
	return (x + y * w) * typeSize;
}

/*******************  FUNCTION  *********************/
void* CMRDomainStorage::getCell ( int x, int y )
{
	//compute memo loc and return
	return &this->data[ getMemoryCoord(x,y) ];
}

/*******************  FUNCTION  *********************/
int CMRDomainStorage::copyGhostFromBuffer ( const void* buffer, size_t size, const CMRRect& rect )
{
	//errors
	assert(dimensions == 2);
	assume(memoryRect.contains(rect),"You request a memory rect which didn't fit with current domain storage memory : %d , %d ; %d , %d",rect.x,rect.y,rect.width,rect.height);
	assume(size >= rect.width * rect.height * typeSize,"Invalid buffer size, too small : %lu.",size);

	for (int y = 0 ; y < rect.height ; y++)
	{
		for (int x = 0 ; x < rect.width ; x++)
		{
			char * buffer_base = ((char*)buffer) + ( x + y * rect.width ) * typeSize;
			char * data_base = data + getMemoryCoord(x,y);
			for (int k = 0 ; k < typeSize ; k++)
				data_base[k] = buffer_base[k];
		}
	}
	
	return rect.width * rect.height * typeSize;
}

/*******************  FUNCTION  *********************/
int CMRDomainStorage::copyGhostToBuffer ( void* buffer, size_t size, const CMRRect& rect ) const
{
	//errors
	assert(dimensions == 2);
	assume(memoryRect.contains(rect),"You request a memory rect which didn't fit with current domain storage memory : %d , %d ; %d , %d",rect.x,rect.y,rect.width,rect.height);
	assume(size >= rect.width * rect.height * typeSize,"Invalid buffer size, too small : %lu.",size);

	for (int y = 0 ; y < rect.height ; y++)
	{
		for (int x = 0 ; x < rect.width ; x++)
		{
			char * buffer_base = ((char*)buffer) + ( x + y * rect.width ) * typeSize;
			char * data_base = data + getMemoryCoord(x,y);
			for (int k = 0 ; k < typeSize ; k++)
				buffer_base[k] = data_base[k];
		}
	}
	
	return rect.width * rect.height * typeSize;
}

/*******************  FUNCTION  *********************/
size_t CMRDomainStorage::getGhostSize ( const CMRRect& rect ) const
{
	//errors
	assert(dimensions == 2);
	assume(memoryRect.contains(rect),"You request a memory rect which didn't fit with current domain storage memory : %d , %d ; %d , %d",rect.x,rect.y,rect.width,rect.height);

	//return
	return rect.height * rect.width * typeSize;
}

/*******************  FUNCTION  *********************/
bool CMRDomainStorage::isContiguous ( int directionID ) const
{
	return (directionID == 0);
}
