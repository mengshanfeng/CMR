/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "CMRGeometry.h"

/*******************  FUNCTION  *********************/
CMRRect2D::CMRRect2D ( int x, int y, int width, int height )
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}
