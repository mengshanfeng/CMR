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
CMRDomainStorage::CMRDomainStorage ( size_t typeSize, int width, int height, int ghostDepth, int originX, int originY )
	: CMRAbstractDomain ( typeSize, width, height, ghostDepth, originX, originY )
{
	//errors
	assert(width > 0);
	assert(height > 0);
	assert(originX >= 0);
	assert(originY >= 0);

	//allocate the data
	this->data = new char[width * height * typeSize];
}

/*******************  FUNCTION  *********************/
bool CMRDomainStorage::isContiguousGhost ( const CMRRect2D& rect ) const
{
	//warnings
	wassume(isFullyInDomainMemory(rect),"You request a memory rect which didn't fit with current domain storage memory : %d , %d ; %d , %d",rect.x,rect.y,rect.width,rect.height);
	assert(dimensions == 2);
	//contiguous only if get one line
	return (rect.height == 1);
}

/*******************  FUNCTION  *********************/
void* CMRDomainStorage::getContiguousGhost ( const CMRRect2D& rect )
{
	//errors
	assume(isFullyInDomainMemory(rect),"You request a memory rect which didn't fit with current domain storage memory : %d , %d ; %d , %d",rect.x,rect.y,rect.width,rect.height);
	assert(dimensions == 2);
	assume(isContiguousGhost(rect),"You request a contiguous base address for a rect which cover non contiguous memory region : %d , %d ; %d , %d",rect.x,rect.y,rect.width,rect.height);
	//get offset
	return getCell(rect.x,rect.y);
}

/*******************  FUNCTION  *********************/
int CMRDomainStorage::getMemoryWidth ( void ) const
{
	return (sizes[CMR_AXIS_X] + 2*ghostDepth) * typeSize;
}

/*******************  FUNCTION  *********************/
int CMRDomainStorage::getCoord ( int x, int y ) const
{
	//vars
	int w;
	int ghostDept = this->ghostDepth;

	//errors
	assert(x >= - ghostDepth && x < this->sizes[CMR_AXIS_X] + 2 * ghostDepth);
	assert(y >= - ghostDepth && y < this->sizes[CMR_AXIS_Y] + 2 * ghostDepth);
	assert(dimensions == 2);

	//fix coords for ghost
	x += ghostDepth;
	y += ghostDepth;
	w = sizes[CMR_AXIS_X] + 2*ghostDepth;

	//return
	return (x + y * w) * typeSize;
}

/*******************  FUNCTION  *********************/
void* CMRDomainStorage::getCell ( int x, int y )
{
	//errors
	assume(x >= - this->ghostDepth && x < this->sizes[CMR_AXIS_X] + 2 * this->ghostDepth,"Invalid X coordinate, out of domain memory : %d",x);
	assume(y >= - this->ghostDepth && y < this->sizes[CMR_AXIS_Y] + 2 * this->ghostDepth,"Invalid Y coordinate, out of domain memory : %d",y);
	assert(dimensions == 2);

	//compute memo loc and return
	return &this->data[ getCoord(x,y) ];
}

/*******************  FUNCTION  *********************/
int CMRDomainStorage::copyGhostFromBuffer ( const void* buffer, size_t size, const CMRRect2D& rect )
{
	//errors
	assert(dimensions == 2);
	assume(isFullyInDomainMemory(rect),"You request a memory rect which didn't fit with current domain storage memory : %d , %d ; %d , %d",rect.x,rect.y,rect.width,rect.height);
	assume(size >= rect.width * rect.height * typeSize,"Invalid buffer size, too small : %lu.",size);

	for (int y = 0 ; y < rect.height ; y++)
	{
		for (int x = 0 ; x < rect.width ; x++)
		{
			char * buffer_base = ((char*)buffer) + ( x + y * rect.width ) * typeSize;
			char * data_base = data + getCoord(x,y);
			for (int k = 0 ; k < typeSize ; k++)
				data_base[k] = buffer_base[k];
		}
	}
	
	return rect.width * rect.height * typeSize;
}

/*******************  FUNCTION  *********************/
int CMRDomainStorage::copyGhostToBuffer ( void* buffer, size_t size, const CMRRect2D& rect ) const
{
	//errors
	assert(dimensions == 2);
	assume(isFullyInDomainMemory(rect),"You request a memory rect which didn't fit with current domain storage memory : %d , %d ; %d , %d",rect.x,rect.y,rect.width,rect.height);
	assume(size >= rect.width * rect.height * typeSize,"Invalid buffer size, too small : %lu.",size);

	for (int y = 0 ; y < rect.height ; y++)
	{
		for (int x = 0 ; x < rect.width ; x++)
		{
			char * buffer_base = ((char*)buffer) + ( x + y * rect.width ) * typeSize;
			char * data_base = data + getCoord(x,y);
			for (int k = 0 ; k < typeSize ; k++)
				buffer_base[k] = data_base[k];
		}
	}
	
	return rect.width * rect.height * typeSize;
}

/*******************  FUNCTION  *********************/
size_t CMRDomainStorage::getGhostSize ( const CMRRect2D& rect ) const
{
	//errors
	assert(dimensions == 2);
	assume(isFullyInDomainMemory(rect),"You request a memory rect which didn't fit with current domain storage memory : %d , %d ; %d , %d",rect.x,rect.y,rect.width,rect.height);

	//return
	return rect.height * rect.width * typeSize;
}

/*******************  FUNCTION  *********************/
bool CMRDomainStorage::isContiguous ( int directionID ) const
{
	return (directionID == 0);
}
