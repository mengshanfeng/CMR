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
CMRAbstractDomain::CMRAbstractDomain ( size_t typeSize, const CMRRect& localDomain, int ghostDepth, int globalWidth, int globalHeight )
{
	//errors
	assert(typeSize > 0);
	assert(ghostDepth > 0);
	assert(globalWidth >= -1);
	assert(globalHeight >= -1);
	
	//params
	if (globalWidth == -1) globalWidth = localDomain.width;
	if (globalHeight == -1) globalHeight = localDomain.height;

	//init values
	this->typeSize = typeSize;
	this->dimensions = 2;
	this->ghostDepth = ghostDepth;
	
	//rects
	this->localRect = localDomain;
	this->globalRect.set(0,0,globalWidth,globalHeight);
	this->memoryRect.set(localDomain.x-ghostDepth,localDomain.y-ghostDepth,localDomain.width + 2*ghostDepth,localDomain.height + 2*ghostDepth);
	assert(globalRect.contains(localDomain));
	assert(memoryRect.contains(localRect));

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
	CMRRect rect;

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
CMRRect CMRAbstractDomain::computeGhostCommRect ( int x, int y, int requestedDepth, CMRCommType commType ) const
{
	//vars
	CMRRect rect;

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
			rect.x = localRect.width - requestedDepth;
		else if( x == 0 )
			rect.x = 0;
	} else if(commType == CMR_COMM_RECV) {
		if (x == -1)// left receive
			rect.x = - requestedDepth;
		else if (x == 1)// right receive
			rect.x = localRect.width;
		else if( x == 0 )
			rect.x = 0;
	}

	//ON Y
	if (commType == CMR_COMM_SEND)
	{
		if (y == -1)// left send
			rect.y = 0;
		else if (y == 1)// right send
			rect.y = localRect.height - requestedDepth;
		else if( y == 0 )
			rect.y = 0;
	} else if(commType == CMR_COMM_RECV) {
		if (y == -1)// left receive
			rect.y = - requestedDepth;
		else if (y == 1)// right receive
			rect.y = localRect.height;
		else if( y == 0 )
			rect.y = 0;
	}

	//setup size
	if( x == 0 )
	{
		rect.width = localRect.width;
	} else {
		rect.width = requestedDepth;
	}

	if( y == 0 )
	{
		rect.height = localRect.height;
	} else {
		rect.height = requestedDepth;
	}

	return rect;
}

/*******************  FUNCTION  *********************/
CMRVect2D CMRAbstractDomain::getAbsPos ( int x, int y ) const
{
	CMRVect2D res(localRect.x + x,localRect.y + y);
	return res;
}

/*******************  FUNCTION  *********************/
CMRVect2D CMRAbstractDomain::getAbsPos ( const CMRVect2D& vect ) const
{
	return localRect.point1() + vect;
}

/*******************  FUNCTION  *********************/
const CMRRect& CMRAbstractDomain::getGlobalRect ( void ) const
{
	return globalRect;
}

/*******************  FUNCTION  *********************/
const CMRRect& CMRAbstractDomain::getLocalDomainRect ( void ) const
{
	return localRect;
}

/*******************  FUNCTION  *********************/
const CMRRect& CMRAbstractDomain::getMemoryRect ( void ) const
{
	return memoryRect;
}

/*******************  FUNCTION  *********************/
bool CMRAbstractDomain::isFullyInDomainMemory ( const CMRRect& rect ) const
{
	return memoryRect.contains(rect);
}

/*******************  FUNCTION  *********************/
bool CMRAbstractDomain::isFullyInLocalDomain ( const CMRRect& rect ) const
{
	return localRect.contains(rect);
}
