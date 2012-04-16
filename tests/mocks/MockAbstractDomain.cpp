/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <assert.h>
#include <cstring>
#include "MockAbstractDomain.h"

/*******************  FUNCTION  *********************/
MockAbstractDomain::MockAbstractDomain ( size_t typeSize, int width, int height, int ghostDepth, int originX, int originY )
	: CMRAbstractDomain ( typeSize, width, height, ghostDepth, originX, originY )
{
	this->buffer = NULL;
	this->contiguous = false;
	this->ghostSize = 0;
}

/*******************  FUNCTION  *********************/
bool MockAbstractDomain::isContiguousGhost ( const CMRRect2D& rect ) const
{
	return this->contiguous;
}

/*******************  FUNCTION  *********************/
void* MockAbstractDomain::getContiguousGhost ( const CMRRect2D& rect )
{
	return buffer;
}

/*******************  FUNCTION  *********************/
size_t MockAbstractDomain::getGhostSize ( const CMRRect2D& rect ) const
{
	return this->ghostSize;
}

/*******************  FUNCTION  *********************/
int MockAbstractDomain::copyGhostFromBuffer ( const void* buffer, size_t size, const CMRRect2D& rect )
{
	//errors
	assert(this->ghostSize <= size);
	
	if (this->buffer != NULL)
		memcpy(this->buffer,buffer,this->ghostSize);

	return this->ghostSize;
}

/*******************  FUNCTION  *********************/
int MockAbstractDomain::copyGhostToBuffer ( void* buffer, size_t size, const CMRRect2D& rect ) const
{
	//errors
	assert(this->ghostSize <= size);

	if (this->buffer != NULL)
		memcpy(buffer,this->buffer,this->ghostSize);

	return this->ghostSize;
}
