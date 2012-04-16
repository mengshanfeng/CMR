/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMRGEOMETRY_H
#define CMRGEOMETRY_H

/********************  HEADERS  *********************/
#include <ostream>

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
bool operator == (const CMRRect2D & rect1 , const CMRRect2D & rect2);

#endif // CMRGEOMETRY_H
