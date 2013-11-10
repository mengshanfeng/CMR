/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cstring>
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include "CMREqDepMatrix.h"

/*******************  FUNCTION  *********************/
CMREqDepMatrix::CMREqDepMatrix(void)
{
	maxDepth = 0;
	memset(access,0,sizeof(access));
}

/*******************  FUNCTION  *********************/
void CMREqDepMatrix::markAccess ( int dx, int dy, bool write )
{
	assert(dx >= -5 && dx <= 5);
	assert(dy >= -5 && dy <= 5);
	if (write)
		access[dy+5][dx+5] |= 2;
	else
		access[dy+5][dx+5] |= 1;
	if (abs(dx) > maxDepth) maxDepth = abs(dx);
	if (abs(dy) > maxDepth) maxDepth = abs(dy);
}

/*******************  FUNCTION  *********************/
void CMREqDepMatrix::printDebug ( void )
{
	for (int j = 5-maxDepth ; j <= 5+maxDepth ; j++)
	{
		for (int i = 5-maxDepth ; i <= 5+maxDepth ; i++)
				printf("%d  ",access[j][i]);
		printf("\n");
	}		
}