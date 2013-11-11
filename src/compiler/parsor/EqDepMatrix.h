/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_EQ_DEP_MATRIX_H
#define CMR_EQ_DEP_MATRIX_H

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
struct EqDepMatrix
{
	EqDepMatrix(void);
	void markAccess(int dx,int dy,bool write);
	void printDebug(void);
	unsigned char access[11][11];
	int maxDepth;
};

}

#endif //CMR_EQ_DEP_MATRIX_H
