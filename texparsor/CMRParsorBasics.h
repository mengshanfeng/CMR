/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_PARSOR_BASICS_H
#define CMR_PARSOR_BASICS_H

/********************  HEADERS  *********************/
#include <string>

/*******************  FUNCTION  *********************/
bool cmrIsAlphaNum(char value);
bool cmrIsNum(char value);
bool cmrIsNumFloat(char value);
bool cmrIsOperator(char value);
bool cmrIsWhiteSpace(char value);
bool cmrIsSimpleOperator(const std::string & value);

#endif //CMR_PARSOR_BASICS_H
