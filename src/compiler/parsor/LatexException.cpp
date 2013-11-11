/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <iostream>
#include <cstdlib>
#include "LatexException.h"

/**********************  USING  *********************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
LatexException::LatexException ( void ) throw()
{

}

/*******************  FUNCTION  *********************/
LatexException::LatexException ( std::string message ) throw()
{
	this->message = message;
}

/*******************  FUNCTION  *********************/
LatexException::~LatexException ( void ) throw()
{

}

/*******************  FUNCTION  *********************/
std::string LatexException::getMessage ( void ) const
{
	return message;
}

/*******************  FUNCTION  *********************/
const char* LatexException::what() const  throw()
{
	return message.c_str();
}

/*******************  FUNCTION  *********************/
void LatexException::fatal()
{
	cerr << message << endl;
	abort();
}

};
