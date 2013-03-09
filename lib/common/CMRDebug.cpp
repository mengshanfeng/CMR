/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cstdarg>
#include <cstdlib>
#include <mpi.h>
#include "CMRDebug.h"
#include "CMRCommon.h"

/*********************  CONSTS  *********************/
/**
 * Define usage of shell colors if enabled.
**/
#ifdef CMR_ENABLE_COLOR
static const char * CMR_MESG_COLOR[5] = {
	"\033[40m\033[36m", //debug
	"\033[40m\033[1;37m", //info
	"\033[0m" , //normal
	"\033[40m\033[33m", //warning
	"\033[40m\033[31m", //error
};
#else
static const char * CMR_MESG_COLOR[5] = { "", "", "" , "" , "" };
#endif

/*******************  FUNCTION  *********************/
/**
 * Main function used by print and debugging system.
 * @param level Define the type of message.
 * @param title Define a short name to prefix the message (ERROR, WARNING....)
 * @param condition The conditions which generate the call of this method.
 * @param fname The source file which call this function.
 * @param line The line in source file which call this function.
 * @param format The string format of the message itself.
**/
void cmrDebugMessage(CMRDebugMessageLevel level,const char * title,const char * condition,const char * fname,int line,const char * format,...)
{
	//vars
	int size = 0;
	int res = 0;
	char * buffer = NULL;

	//errors
	assert(fname != NULL);
	assert(line > 0);
	assert(format != NULL);
	assert(title != NULL);

	//allocate buffer for message
	size = strlen(format) * 4 + 1024;
	buffer = (char*)malloc(size);
	va_list param;
	va_start (param, format);
	res = vsnprintf (buffer,size, format, param);
	va_end (param);
	if (res >= size)
	{
		size = res + 1;
		buffer = (char*)realloc(buffer,size);
		va_start (param, format);
		res = vsnprintf (buffer,size, format, param);
		va_end (param);
	}
	assert(res <= size);

	//print
	if (condition == NULL)
		fprintf(stderr,"%s%-7s [rank=%d] (%s:%d) : %s%s\n",CMR_MESG_COLOR[level],title,cmrGetMPIRank(),basename(fname),line,buffer,CMR_MESG_COLOR[CMR_DEBUG_NORMAL]);
	else
		fprintf(stderr,"%s%-7s [rank=%d] (%s:%d) : %s\n        %s%s\n",CMR_MESG_COLOR[level],title,cmrGetMPIRank(),basename(fname),line,condition,buffer,CMR_MESG_COLOR[CMR_DEBUG_NORMAL]);

	//free buffers
	free(buffer);

	//if error, abort
	if (level >= CMR_DEBUG_ERROR)
		MPI_Abort(MPI_COMM_WORLD,-1);
}


