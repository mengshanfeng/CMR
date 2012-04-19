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
#include "CMRDebug.h"

/*********************  CONSTS  *********************/
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
	size = strlen(format) * 4;
	buffer = new char[size];
	va_list param;
	va_start (param, format);
	res = vsprintf (buffer, format, param);
	assert(res <= size);
	va_end (param);

	//print
	if (condition == NULL)
		fprintf(stderr,"%s%s (%s:%d) : %s%s\n",CMR_MESG_COLOR[level],title,basename(fname),line,buffer,CMR_MESG_COLOR[CMR_DEBUG_NORMAL]);
	else
		fprintf(stderr,"%s%s (%s:%d) : %s\n%s%s\n",CMR_MESG_COLOR[level],title,basename(fname),line,condition,buffer,CMR_MESG_COLOR[CMR_DEBUG_NORMAL]);

	//free buffers
	delete [] buffer;
}
