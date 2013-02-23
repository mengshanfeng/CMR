/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_GEOMETRY_H
#define CMR_GEOMETRY_H

/********************  HEADERS  *********************/
#include <ostream>

/*********************  CLASS  **********************/
struct CMRVect2D
{
	CMRVect2D(int x = 0, int y = 0);
	int x;
	int y;
};

/*********************  CLASS  **********************/
struct CMRRect2D
{
	CMRRect2D(int x = 0,int y = 0,int width = 0,int height = 0);
	int x;
	int y;
	int width;
	int height;
};

/*******************  FUNCTION  *********************/
std::ostream & operator << (std::ostream & out,const CMRRect2D & rect);
std::ostream & operator << (std::ostream & out,const CMRVect2D & vect);
bool operator == (const CMRRect2D & rect1 , const CMRRect2D & rect2);
bool operator == (const CMRVect2D & v1 , const CMRVect2D & v2);

#endif // CMR_GEOMETRY_H
