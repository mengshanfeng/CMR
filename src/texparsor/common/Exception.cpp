/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <string>
#include <exception>
#include "CMRDebug.h"
#include "Exception.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
Exception::Exception ( void ) throw()
{

}

/*******************  FUNCTION  *********************/
Exception::Exception ( std::string message ) throw()
{
	this->message = message;
}

/*******************  FUNCTION  *********************/
Exception::~Exception ( void ) throw()
{

}

/*******************  FUNCTION  *********************/
std::string Exception::getMessage ( void ) const
{
	return message;
}

/*******************  FUNCTION  *********************/
const char* Exception::what() const  throw()
{
	return message.c_str();
}

/*******************  FUNCTION  *********************/
void Exception::fatal()
{
	cmrFatal("%s",message.c_str());
}

};
