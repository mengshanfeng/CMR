/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <assert.h>
#include "CMRAbstractDomain.h"

/*******************  FUNCTION  *********************/
CMRAbstractDomain::CMRAbstractDomain ( int typeSize, int width, int height, int ghostDepth , int originX , int originY )
{
	//errors
	assert(typeSize > 0);
	assert(width > 0);
	assert(height > 0);
	assert(ghostDepth > 0);
	assert(ghostDepth < width && ghostDepth < height);

	//init values
	this->typeSize = typeSize;
	this->sizes[CMR_AXIS_X] = width;
	this->sizes[CMR_AXIS_Y] = height;
	this->dimensions = 2;
	this->origin[CMR_AXIS_X] = originX;
	this->origin[CMR_AXIS_Y] = originY;
	this->ghostDepth = ghostDepth;

	//setup default communicators and ghost status
	for (int x = 0 ; x < 3 ; x++)
	{
		for (int y = 0 ; y < 3 ; y++)
		{
			ghostStaus[x][y] = CMR_UPDATE_STATUS_UPDATED;
			communicators[x][y] = NULL;
		}
	}
}
