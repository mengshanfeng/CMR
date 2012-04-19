/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMMON_H
#define CMR_COMMON_H

/********************  MACRO  ***********************/
//used to make fiend classes to unit test private/protected members
#ifdef UNITTEST
#define ASSIST_UNIT_TEST( __class__ ) friend class __class__;
#else
#define ASSIST_UNIT_TEST( __class__ )
#endif

/********************  MACRO  ***********************/
#define CMR_MPI_MASTER 0

/********************  ENUM  ************************/
enum CMRCommType
{
	CMR_COMM_SEND,
	CMR_COMM_RECV
};

/*******************  FUNCTION  *********************/
bool cmrIsMPIMaster(void);
int cmrGetMPIRank(void);
int cmrGetMPISize(void);

#endif //CMR_COMMON_H
