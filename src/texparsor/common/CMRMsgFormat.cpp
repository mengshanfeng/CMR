/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include "CMRDebug.h"
#include "CMRMsgFormat.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
CMRMsgException::CMRMsgException ( void ) throw()
{

}

/*******************  FUNCTION  *********************/
CMRMsgException::CMRMsgException ( std::string message ) throw()
{
	this->message = message;
}

/*******************  FUNCTION  *********************/
CMRMsgException::~CMRMsgException ( void ) throw()
{

}

/*******************  FUNCTION  *********************/
std::string CMRMsgException::getMessage ( void ) const
{
	return message;
}

/*******************  FUNCTION  *********************/
const char* CMRMsgException::what() const  throw()
{
	return message.c_str();
}

/*******************  FUNCTION  *********************/
void CMRMsgException::fatal()
{
	cerr << message << endl;
	abort();
}

/*******************  FUNCTION  *********************/
CMRMsgFormat::CMRMsgFormat ( const string& format )
{
	this->value = format;
	this->cntArg = 0;
}

/*******************  FUNCTION  *********************/
CMRMsgFormat& CMRMsgFormat::arg ( const string& arg )
{
	char buffer[30];
	size_t size = sprintf(buffer,"%%%d",++cntArg);
	size_t pos = value.find(buffer);
	if (pos != string::npos)
		this->value = value.replace(pos, size-1, arg);
	return *this;
}

/*******************  FUNCTION  *********************/
void CMRMsgFormat::debug ( void ) const
{
	cmrDebug("%s",value.c_str());
}

/*******************  FUNCTION  *********************/
void CMRMsgFormat::exception ( void ) const
{
	throw CMRMsgException(value);
}

/*******************  FUNCTION  *********************/
void CMRMsgFormat::fatal ( void ) const
{
	cmrFatal("%s",value.c_str());
}

/*******************  FUNCTION  *********************/
void CMRMsgFormat::warning ( void ) const
{
	cmrWarning("%s",value.c_str());
}

/*******************  FUNCTION  *********************/
ostream& operator<< ( ostream& out, const CMRMsgFormat& msg )
{
	out << msg.value;
	return out;
}

/*******************  FUNCTION  *********************/
const string& CMRMsgFormat::str ( void ) const
{
	return value;
}
