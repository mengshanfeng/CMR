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

/********************  MACRO  ***********************/
//used to make fiend classes to unit test private/protected members
#ifdef UNITTEST
	#ifdef UNIT_TEST_USER_FRIENDS
		#define ASSIST_UNIT_TEST( __class__ ) friend class __class__; UNIT_TEST_USER_FRIENDS
	#else
		#define ASSIST_UNIT_TEST( __class__ ) friend class __class__;
	#endif
#else
#define ASSIST_UNIT_TEST( __class__ )
#endif

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
std::ostream & doIndent(std::ostream & out,int depth);

}

#endif //CMR_COMPILER_COMMON_H
