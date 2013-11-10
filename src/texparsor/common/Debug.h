/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMPILER_DEBUG_H
#define CMR_COMPILER_DEBUG_H

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/********************  ENUM  ************************/
/**
 * Define some debugging level for future verbosity control.
**/
enum DebugMessageLevel
{
	DEBUG_DEBUG   = 0,
	DEBUG_INFO    = 1,
	DEBUG_NORMAL  = 2,
	DEBUG_WARNING = 3,
	DEBUG_ERROR   = 4,
};

/********************  MACRO  ***********************/
/** @TODO rename this with cmr.... prefix **/
#define cmrWarning(...) cmrDebugMessage(CMRCompiler::DEBUG_WARNING,"WARNING",NULL,__FILE__,__LINE__,__VA_ARGS__)
/** @TODO rename this with cmr.... prefix **/
#define cmrWAssume(test,...) if (!(test)) cmrDebugMessage(CMRCompiler::DEBUG_WARNING,"WARNING",#test,__FILE__,__LINE__,__VA_ARGS__)
/** @TODO rename this with cmr.... prefix **/
#define cmrFatal(...) cmrDebugMessage(CMRCompiler::DEBUG_ERROR,"FATAL",NULL,__FILE__,__LINE__,__VA_ARGS__)
/** @TODO rename this with cmr.... prefix **/
#define cmrInfo(...) cmrDebugMessage(CMRCompiler::DEBUG_INFO,"INFO",NULL,__FILE__,__LINE__,__VA_ARGS__)
/** @TODO rename this with cmr.... prefix **/
#define cmrAssume(test,...) if (!(test)) cmrDebugMessage(CMRCompiler::DEBUG_ERROR,"ERROR",#test,__FILE__,__LINE__,__VA_ARGS__)
#define cmrNotImplemented(...) cmrDebugMessage(CMRCompiler::DEBUG_ERROR,"NOIMPL",NULL,__FILE__,__LINE__,__VA_ARGS__)

/********************  MACRO  ***********************/
#ifndef NDEBUG
#define cmrDebug(...) cmrDebugMessage(CMRCompiler::DEBUG_DEBUG,"DEBUG",NULL,__FILE__,__LINE__,__VA_ARGS__)
#define cmrDebugWarning(test,...) if (!(test)) cmrDebugMessage(CMRCompiler::DEBUG_WARNING,"WARNING",#test,__FILE__,__LINE__,__VA_ARGS__)
#else
#define cmrDebug(...) /*cmrDebugMessage(DEBUG_DEBUG,"DEBUG",NULL,__FILE__,__LINE__,__VA_ARGS__)*/
#define cmrDebugWarning(test,...) /*if (!(test)) cmrDebugMessage(DEBUG_WARNING,"WARNING",#test,__FILE__,__LINE__,__VA_ARGS__)*/
#endif

/*******************  FUNCTION  *********************/
void cmrDebugMessage(DebugMessageLevel level,const char * title,const char * condition,const char * fname,int line,const char * format,...);

};

#endif // CMR_COMPILER_DEBUG_H
