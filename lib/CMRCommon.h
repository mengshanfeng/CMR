#ifndef CMR_COMMON_H
#define CMR_COMMON_H

/********************  MACRO  ***********************/
//used to make fiend classes to unit test private/protected members
#ifdef UNITTEST
#define ASSIST_UNIT_TEST( __class__ ) friend class __class__
#else
#define ASSIST_UNIT_TEST( __class__ )
#endif

/********************  ENUM  ************************/
enum CMRCommType
{
	CMR_COMM_SEND,
	CMR_COMM_RECV
};

#endif //CMR_COMMON_H
