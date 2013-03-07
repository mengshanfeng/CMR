/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_DEBUG_H
#define CMR_DEBUG_H

/********************  ENUM  ************************/
/**
 * Define some debugging level for future verbosity control.
**/
enum CMRDebugMessageLevel
{
	CMR_DEBUG_DEBUG   = 0,
	CMR_DEBUG_INFO    = 1,
	CMR_DEBUG_NORMAL  = 2,
	CMR_DEBUG_WARNING = 3,
	CMR_DEBUG_ERROR   = 4,
};

/********************  MACRO  ***********************/
/** @TODO rename this with cmr.... prefix **/
#define warning(...) cmrDebugMessage(CMR_DEBUG_WARNING,"WARNING",NULL,__FILE__,__LINE__,__VA_ARGS__)
/** @TODO rename this with cmr.... prefix **/
#define wassume(test,...) if (!(test)) cmrDebugMessage(CMR_DEBUG_WARNING,"WARNING",#test,__FILE__,__LINE__,__VA_ARGS__)
/** @TODO rename this with cmr.... prefix **/
#define fatal(...) cmrDebugMessage(CMR_DEBUG_ERROR,"FATAL",NULL,__FILE__,__LINE__,__VA_ARGS__)
/** @TODO rename this with cmr.... prefix **/
#define info(...) cmrDebugMessage(CMR_DEBUG_INFO,"INFO",NULL,__FILE__,__LINE__,__VA_ARGS__)
/** @TODO rename this with cmr.... prefix **/
#define info_on_master(...) if(cmrIsMPIMaster()) cmrDebugMessage(CMR_DEBUG_INFO,"INFO",NULL,__FILE__,__LINE__,__VA_ARGS__)
/** @TODO rename this with cmr.... prefix **/
#define assume(test,...) if (!(test)) cmrDebugMessage(CMR_DEBUG_ERROR,"ERROR",#test,__FILE__,__LINE__,__VA_ARGS__)

/********************  MACRO  ***********************/
#ifndef NDEBUG
#define debug(...) cmrDebugMessage(CMR_DEBUG_DEBUG,"DEBUG",NULL,__FILE__,__LINE__,__VA_ARGS__)
#define debug_on_master(...) if(cmrIsMPIMaster()) cmrDebugMessage(CMR_DEBUG_DEBUG,"DEBUG",NULL,__FILE__,__LINE__,__VA_ARGS__)
#define debug_warning(test,...) if (!(test)) cmrDebugMessage(CMR_DEBUG_WARNING,"WARNING",#test,__FILE__,__LINE__,__VA_ARGS__)
#else
#define debug(...) /*cmrDebugMessage(CMR_DEBUG_DEBUG,"DEBUG",NULL,__FILE__,__LINE__,__VA_ARGS__)*/
#define debug_on_master(...) /*if(cmrIsMPIMaster()) cmrDebugMessage(CMR_DEBUG_DEBUG,"DEBUG",NULL,__FILE__,__LINE__,__VA_ARGS__)*/
#define debug_warning(test,...) /*if (!(test)) cmrDebugMessage(CMR_DEBUG_WARNING,"WARNING",#test,__FILE__,__LINE__,__VA_ARGS__)*/
#endif

/*******************  FUNCTION  *********************/
void cmrDebugMessage(CMRDebugMessageLevel level,const char * title,const char * condition,const char * fname,int line,const char * format,...);

#endif // CMR_DEBUG_H
