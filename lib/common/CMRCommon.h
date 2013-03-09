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
#define CMR_MPI_ALL -1
#define CMR_ALL -1

/********************  ENUM  ************************/
/**
 * Synchronisation types.
**/
enum CMRCommType
{
	/** Send the data to the remote entity. **/
	CMR_COMM_SEND,
	/** Receive the data from the remote entity. **/
	CMR_COMM_RECV
};

/*******************  FUNCTION  *********************/
bool cmrIsMPIMaster(void);
int cmrGetMPIRank(void);
int cmrGetMPISize(void);
int cmrPGCD(int a, int b);

/*******************  FUNCTION  *********************/
/**
 * Quick template function to permute two values.
 * It require support of '=' operator and copy constructor.
 * @TODO move implementation in _impl.h file
**/
template<class T>
void cmrPermut(T & v1,T & v2)
{
	T tmp(v1);
	v1 = v2;
	v2 = tmp;
}

#endif //CMR_COMMON_H
