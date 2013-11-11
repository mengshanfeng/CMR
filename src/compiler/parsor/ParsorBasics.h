/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMPILER_PARSOR_BASICS_H
#define CMR_COMPILER_PARSOR_BASICS_H

/********************  HEADERS  *********************/
#include <string>
#include <vector>

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  TYPES  **********************/
typedef std::vector<std::string> StringVector;

/*******************  FUNCTION  *********************/
bool cmrIsAlphaNum(char value);
bool cmrIsNum(char value);
bool cmrIsNumFloat(char value);
bool cmrIsOperator(char value);
bool cmrIsWhiteSpace(char value);
bool cmrIsSimpleOperator(const std::string & value);
StringVector cmrStringSplit(const std::string & value,const std::string & separator);
float cmrSafeStringToFloat(const std::string & value);
std::string cmrIndent(int depth);
bool cmrIsInteger(const std::string & value);

}

#endif //CMR_COMPILER_PARSOR_BASICS_H
