/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cstdio>
#include <cstdlib>
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

/*******************  FUNCTION  *********************/
bool cmrIsInteger(const std::string & value)
{
	if (value.empty())
		return false;

	for (int i = 0 ; i < value.size() ; i++)
		if (cmrIsNum(value[i]) == false)
			return false;
		
	return true;
}

/*******************  FUNCTION  *********************/
std::vector< std::string> cmrStringSplit ( const std::string& value, const std::string& separator )
{
	//vars
	CMRStringVector res;
	size_t index = 0;
	size_t last_index = 0;
	
	while (index != std::string::npos && index < value.size())
	{
		index = value.find(separator, index+separator.size());
		res.push_back(value.substr(last_index,index - last_index));
		last_index = index + separator.size();
	}
	
	return res;
}

/*******************  FUNCTION  *********************/
float cmrSafeStringToFloat ( const std::string& value )
{
	/** TODO : implement **/
	return atof(value.c_str());
}
