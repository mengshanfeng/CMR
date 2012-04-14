/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMRGEOMETRY_H
#define CMRGEOMETRY_H

/*********************  CLASS  **********************/
struct CMRRect2D
{
	CMRRect2D(int x = 0,int y = 0,int width = 0,int height = 0);
	int x;
	int y;
	int width;
	int height;
};

#endif // CMRGEOMETRY_H
