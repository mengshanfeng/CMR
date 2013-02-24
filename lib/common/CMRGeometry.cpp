/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cstdio>
#include <cassert>
#include "CMRGeometry.h"

/*******************  FUNCTION  *********************/
CMRVect2D::CMRVect2D ( int x, int y )
{
	this->x = x;
	this->y = y;
}

/*******************  FUNCTION  *********************/
std::ostream& operator<< ( std::ostream& out, const CMRVect2D& vect )
{
	char buffer[256];
	int res = sprintf(buffer,"CMRVect2D [ %5d , %5d ]",vect.x,vect.y);
	assert(res < 256);
	out << buffer;
	return out;
}

/*******************  FUNCTION  *********************/
bool operator== ( const CMRVect2D& v1, const CMRVect2D& v2 )
{
	return ((v1.x == v2.x) && (v1.y == v2.y));
}

/*******************  FUNCTION  *********************/
CMRRect2D::CMRRect2D ( int x, int y, int width, int height )
{
	//errors
	assert(width >= 0);
	assert(height >= 0);

	//set default
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

/*******************  FUNCTION  *********************/
std::ostream& operator<< ( std::ostream& out, const CMRRect2D& rect )
{
	char buffer[256];
	int res = sprintf(buffer,"CMRRect2D [ %5d , %5d , %5d , %5d ]",rect.x,rect.y,rect.width,rect.height);
	assert(res < 256);
	out << buffer;
	return out;
}

/*******************  FUNCTION  *********************/
bool operator== ( const CMRRect2D& rect1, const CMRRect2D& rect2 )
{
	return (rect1.x == rect2.x)
		&& (rect1.y == rect2.y)
		&& (rect1.width == rect2.width)
		&& (rect1.height == rect2.height);
}