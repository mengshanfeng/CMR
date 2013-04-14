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
#include "CMRLatexException.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
CMRLatexException::CMRLatexException ( void ) throw()
{

}

/*******************  FUNCTION  *********************/
CMRLatexException::CMRLatexException ( std::string message ) throw()
{
	this->message = message;
}

/*******************  FUNCTION  *********************/
CMRLatexException::~CMRLatexException ( void ) throw()
{

}

/*******************  FUNCTION  *********************/
std::string CMRLatexException::getMessage ( void ) const
{
	return message;
}

/*******************  FUNCTION  *********************/
const char* CMRLatexException::what() const  throw()
{
	return message.c_str();
}

/*******************  FUNCTION  *********************/
void CMRLatexException::fatal()
{
	cerr << message << endl;
	abort();
}
