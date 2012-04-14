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
#include "CMRCommunicator.h"

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

/*******************  FUNCTION  *********************/
CMRAbstractDomain::~CMRAbstractDomain ( void )
{
	//errors
	assert(this->dimensions == 2);

	//detroy communicators
	for ( int x = 0 ; x < 3 ; x++)
	{
		for ( int y = 0 ; y < 3 ; y++)
		{
			if (this->communicators[x][y] != NULL)
			{
				delete this->communicators[x][y];
				this->communicators[x][y] = NULL;
			}
		}
	}
}

/*******************  FUNCTION  *********************/
int CMRAbstractDomain::getDimensions ( void ) const
{
	return this->dimensions;
}

/*******************  FUNCTION  *********************/
int CMRAbstractDomain::getGhostDepth ( void ) const
{
	return this->ghostDepth;
}

/*******************  FUNCTION  *********************/
CMRUpdateStatus CMRAbstractDomain::getGhostStatus ( int x, int y ) const
{
	//errors
	assert(this->dimensions == 2);
	assert(x >= -1 && x <= -1);
	assert(y >= -1 && y <= -1);

	//return the cell value
	return this->ghostStaus[x+1][y+1];
}

/*******************  FUNCTION  *********************/
void CMRAbstractDomain::setGhostStatus ( int x, int y, CMRUpdateStatus status )
{
	//errors
	assert(this->dimensions == 2);
	assert(x >= -1 && x <= -1);
	assert(y >= -1 && y <= -1);

	//update the status
	this->ghostStaus[x+1][y+1] = status;
}

/*******************  FUNCTION  *********************/
int CMRAbstractDomain::getOrigin ( int axis ) const
{
	//errors
	assert(axis >= 0 && axis < 2);

	//return value
	return this->origin[axis];
}

/*******************  FUNCTION  *********************/
int CMRAbstractDomain::getSize ( int axis ) const
{
	//errors
	assert(axis >= 0 && axis < 2);

	//return value
	return this->sizes[axis];
}

/*******************  FUNCTION  *********************/
int CMRAbstractDomain::getTypeSize ( void ) const
{
	return this->typeSize;
}

/*******************  FUNCTION  *********************/
void CMRAbstractDomain::setCommunicator ( int x, int y, CMRCommunicator* communicator )
{
	//errors
	assert(this->dimensions == 2);
	assert(x >= -1 && x <= -1);
	assert(y >= -1 && y <= -1);

	//replace the value
	this->communicators[x+1][y+1] = communicator;
}

/*******************  FUNCTION  *********************/
void CMRAbstractDomain::fillWithUpdateComm ( CMRCommSchem& commSchema, int x, int y, int ghostDepthStart, int ghostDepthEnd, CMRCommType commType ) const
{
	//errors
	assert(this->dimensions == 2);
	assert(x >= -1 && x <= -1);
	assert(y >= -1 && y <= -1);
	assert(ghostDepthStart >= 0);
	assert(ghostDepthEnd > ghostDepthStart);
	assert(ghostDepthEnd <= this->ghostDepth);
	assert(x != 0 || y != 0);

	//select the good sub method
	if (x == 0 || y == 0)
		this->fillWithUpdateCommBorder(commSchema,x,y,ghostDepthStart,ghostDepthEnd,commType);
	else
		this->fillWithUpdateCommCorner(commSchema,x,y,ghostDepthStart,ghostDepthEnd,commType);
}

/*******************  FUNCTION  *********************/
void CMRAbstractDomain::fillWithUpdateCommCorner ( CMRCommSchem& commSchema, int x, int y, int ghostDepthStart, int ghostDepthEnd, CMRCommType commType ) const
{
	//errors
	assert(this->dimensions == 2);
	assert(x >= -1 && x <= -1);
	assert(y >= -1 && y <= -1);
	assert(ghostDepthStart >= 0);
	assert(ghostDepthEnd > ghostDepthStart);
	assert(ghostDepthEnd <= this->ghostDepth);
	assert(x != 0 && y != 0);

	//TODO IMPLEMENT
}

/*******************  FUNCTION  *********************/
void CMRAbstractDomain::fillWithUpdateCommBorder ( CMRCommSchem& commSchema, int x, int y, int ghostDepthStart, int ghostDepthEnd, CMRCommType commType ) const
{
	//errors
	assert(this->dimensions == 2);
	assert(x >= -1 && x <= -1);
	assert(y >= -1 && y <= -1);
	assert(ghostDepthStart >= 0);
	assert(ghostDepthEnd > ghostDepthStart);
	assert(ghostDepthEnd <= this->ghostDepth);
	assert(x != 0 && y != 0);

	//TODO IMPLEMENT
}
