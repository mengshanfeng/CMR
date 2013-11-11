/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMPILER_COMMON_H
#define CMR_COMPILER_COMMON_H

/********************  HEADERS  *********************/
#include <ostream>

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
std::ostream & doIndent(std::ostream & out,int depth);

}

#endif //CMR_COMPILER_COMMON_H
