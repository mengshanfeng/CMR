/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "CMRCommon.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
ostream& doIndent ( ostream& out, int depth )
{
	for (int i = 0 ; i < depth ; i++)
		out << "\t";
	return out;
}