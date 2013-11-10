/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "Debug.h"
#include "Exception.h"
#include "MsgFormat.h"

/**********************  USING  *********************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
MsgFormat::MsgFormat ( const string& format )
{
	this->value = format;
	this->cntArg = 0;
}

/*******************  FUNCTION  *********************/
MsgFormat& MsgFormat::arg ( const string& arg )
{
	char buffer[30];
	size_t size = sprintf(buffer,"%%%d",++cntArg);
	size_t pos = value.find(buffer);
	if (pos != string::npos)
		this->value = value.replace(pos, size, arg);
	return *this;
}

/*******************  FUNCTION  *********************/
void MsgFormat::debug ( void ) const
{
	cmrDebug("%s",value.c_str());
}

/*******************  FUNCTION  *********************/
void MsgFormat::exception ( void ) const
{
	throw Exception(value);
}

/*******************  FUNCTION  *********************/
void MsgFormat::fatal ( void ) const
{
	cmrFatal("%s",value.c_str());
}

/*******************  FUNCTION  *********************/
void MsgFormat::warning ( void ) const
{
	cmrWarning("%s",value.c_str());
}

/*******************  FUNCTION  *********************/
ostream& operator<< ( ostream& out, const MsgFormat& msg )
{
	out << msg.value;
	return out;
}

/*******************  FUNCTION  *********************/
const string& MsgFormat::str ( void ) const
{
	return value;
}

};
