/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "CMRParsorBasics.h"

/*******************  FUNCTION  *********************/
bool cmrIsAlphaNum(char value)
{
	return (value >= 'a' && value <= 'z')
		|| (value >= 'A' && value <= 'Z')
		|| (value >= '0' && value <= '9') || value == '.';
}

/*******************  FUNCTION  *********************/
bool cmrIsNum(char value)
{
	return (value >= '0' && value <= '9');
}

/*******************  FUNCTION  *********************/
bool cmrIsNumFloat(char value)
{
	return (value >= '0' && value <= '9') || value == '.';
}

/*******************  FUNCTION  *********************/
bool cmrIsOperator(char value)
{
	return (value == '=' || value == '*' || value == '+' || value == '-' || value == '/' || value == ',');
}

/*******************  FUNCTION  *********************/
bool cmrIsWhiteSpace(char value)
{
	return (value == ' ' || value == '\t');
}

/*******************  FUNCTION  *********************/
bool cmrIsSimpleOperator(const std::string & value)
{
	return (value == "+" || value == "-");
}
