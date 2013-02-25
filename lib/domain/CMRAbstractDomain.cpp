/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <assert.h>
#include "CMRAbstractDomain.h"
#include "../communication/CMRCommFactory.h"
#include "../common/CMRGeometry.h"
#include "../communication/CMRCommSchem.h"

/*******************  FUNCTION  *********************/
CMRAbstractDomain::CMRAbstractDomain ( size_t typeSize, int width, int height, int ghostDepth , int originX , int originY ,int globalWidth,int globalHeight)
	:globalRect(0,0,globalWidth,globalHeight)
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
	
	//global size
	if (globalWidth == -1) this->globalRect.width = width;
	if (globalHeight== -1) this->globalRect.height = height;

	//setup default communicators and ghost status
	for (int x = 0 ; x < 3 ; x++)
	{
		for (int y = 0 ; y < 3 ; y++)
		{
			ghostStatus[x][y] = CMR_UPDATE_STATUS_UPDATED;
			commFactories[x][y] = NULL;
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
			if (this->commFactories[x][y] != NULL)
			{
				delete this->commFactories[x][y];
				this->commFactories[x][y] = NULL;
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
	return this->ghostStatus[x+1][y+1];
}

/*******************  FUNCTION  *********************/
void CMRAbstractDomain::setGhostStatus ( int x, int y, CMRUpdateStatus status )
{
	//errors
	assert(this->dimensions == 2);
	assert(x >= -1 && x <= -1);
	assert(y >= -1 && y <= -1);

	//update the status
	this->ghostStatus[x+1][y+1] = status;
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
size_t CMRAbstractDomain::getTypeSize ( void ) const
{
	return this->typeSize;
}

/*******************  FUNCTION  *********************/
void CMRAbstractDomain::setCommunicator ( int x, int y, CMRCommFactory * commFactory )
{
	//errors
	assert(this->dimensions == 2);
	assert(x >= -1 && x <= 1);
	assert(y >= -1 && y <= 1);

	//replace the value
	this->commFactories[x+1][y+1] = commFactory;
}

/*******************  FUNCTION  *********************/
void CMRAbstractDomain::fillWithUpdateComm ( CMRCommSchem& commSchema, int x, int y, int requestedDepth, CMRCommType commType )
{
	//vars
	CMRRect2D rect;

	//errors
	assert(this->dimensions == 2);
	assert(x >= -1 && x <= 1);
	assert(y >= -1 && y <= 1);
	assert(requestedDepth >= 0);
	assert(requestedDepth <= this->ghostDepth);
	assert(x != 0 || y != 0);

	//if no comm factory, no communication to add
	if (commFactories[x+1][y+1] != NULL)
	{
		rect = computeGhostCommRect(x,y,requestedDepth,commType);
		commSchema.addComm(commFactories[x+1][y+1]->createComm(this,rect,commType));
	}
}

/*******************  FUNCTION  *********************/
/*
  0                               	  0
0 r r # # # # # # u u--->X              0 # # i  i i i i i # #--->X
  r r # # # # # # u u                     # # i  i i i i i # #
  # # R R * * U U # #                     # r RI I I I I I # #
  # # R R * * U U # #                     # r RI I I I I I # #
  # # * * * * * * # #                     # r RI I I I I I # #
  # # * * * * * * # #                     # r RI I I I I I # #
  # # * * * * * * # #                     # r RI I I I I I # #
  # # # # # # # # # #                     # # #  # # # # # # #
  # # # # # # # # # #                     # # #  # # # # # # #
  Y                                       Y
*/
CMRRect2D CMRAbstractDomain::computeGhostCommRect ( int x, int y, int requestedDepth, CMRCommType commType ) const
{
	//vars
	CMRRect2D rect;

	//errors
	assert(this->dimensions == 2);
	assert(x >= -1 && x <= 1);
	assert(y >= -1 && y <= 1);
	assert(requestedDepth >= 0);
	assert(requestedDepth <= this->ghostDepth);
	assert(x != 0 || y != 0);

	//on X axis
	if (commType == CMR_COMM_SEND)
	{
		if (x == -1)// left send
			rect.x = 0;
		else if (x == 1)// right send
			rect.x = this->sizes[CMR_AXIS_X] - requestedDepth;
		else if( x == 0 )
			rect.x = 0;
	} else if(commType == CMR_COMM_RECV) {
		if (x == -1)// left receive
			rect.x = - requestedDepth;
		else if (x == 1)// right receive
			rect.x = this->sizes[CMR_AXIS_X];
		else if( x == 0 )
			rect.x = 0;
	}

	//ON Y
	if (commType == CMR_COMM_SEND)
	{
		if (y == -1)// left send
			rect.y = 0;
		else if (y == 1)// right send
			rect.y = this->sizes[CMR_AXIS_Y] - requestedDepth;
		else if( y == 0 )
			rect.y = 0;
	} else if(commType == CMR_COMM_RECV) {
		if (y == -1)// left receive
			rect.y = - requestedDepth;
		else if (y == 1)// right receive
			rect.y = this->sizes[CMR_AXIS_Y];
		else if( y == 0 )
			rect.y = 0;
	}

	//setup size
	if( x == 0 )
	{
		rect.width = this->sizes[CMR_AXIS_X];
	} else {
		rect.width = requestedDepth;
	}

	if( y == 0 )
	{
		rect.height = this->sizes[CMR_AXIS_Y];
	} else {
		rect.height = requestedDepth;
	}

	return rect;
}

/*******************  FUNCTION  *********************/
bool CMRAbstractDomain::isFullyInDomain ( const CMRRect2D& rect ) const
{
	return (rect.x >= 0 && rect.width + rect.x <= this->sizes[CMR_AXIS_X])
		&& (rect.y >= 0 && rect.height + rect.y <= this->sizes[CMR_AXIS_Y]);
}

/*******************  FUNCTION  *********************/
bool CMRAbstractDomain::isFullyInDomainMemory ( const CMRRect2D& rect ) const
{
	return (rect.x >= -this->ghostDepth && rect.width + rect.x <= this->sizes[CMR_AXIS_X] + this->ghostDepth)
		&& (rect.y >= -this->ghostDepth && rect.height + rect.y <= this->sizes[CMR_AXIS_Y] + this->ghostDepth);
}

/*******************  FUNCTION  *********************/
CMRVect2D CMRAbstractDomain::getAbsPos ( int x, int y ) const
{
	CMRVect2D res(origin[CMR_AXIS_X] + x,origin[CMR_AXIS_Y]+y);
	return res;
}

/*******************  FUNCTION  *********************/
CMRVect2D CMRAbstractDomain::getGlobalSize ( void ) const
{
	CMRVect2D res(globalRect.x,globalRect.y);
	return res;
}

/*******************  FUNCTION  *********************/
CMRRect2D CMRAbstractDomain::getLocalRect ( void ) const
{
	CMRRect2D rect(origin[CMR_AXIS_X],origin[CMR_AXIS_Y],sizes[CMR_AXIS_X],sizes[CMR_AXIS_Y]);
	return rect;
}

/*******************  FUNCTION  *********************/
CMRRect2D CMRAbstractDomain::getGlobalRect ( void ) const
{
	return globalRect;
}
