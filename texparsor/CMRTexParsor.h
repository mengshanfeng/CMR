/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_TEX_PARSOR_H
#define CMR_TEX_PARSOR_H

/********************  HEADERS  *********************/
#include <string>
#include "CMRLatexEntity.h"

/*******************  FUNCTION  *********************/
std::string cmrExtractName(const std::string & value,int & start);
void cmrParsorError(const std::string & value,int pos,const std::string & message="");
std::string cmrExtractSubZone(const std::string & value, int & start,char open = '{',char close = '}');
void cmrExtractSubAndSuperScript(const std::string & value,int & start,CMRLatexEntity * entity);
void cmrParseLatexFormula(CMRLatexFormulas & formula,const std::string & value);
int cmrRequireParameters(const std::string & name,const std::string & value,int pos);
void cmrReplaceByCommaGroup(CMRLatexFormulas & formula);
CMRLatexEntity * cmrParseLatexEntitySimple(const std::string & value,int & start);
CMRLatexEntity * cmrExtractSubGroup(const std::string & value,int & start);
CMRLatexEntity * cmrParseLatexEntity(const std::string & value,int & start);
void cmrSkipWhiteSpace(const std::string & value,int & cur);
void cmrParseLatexFormula(CMRLatexFormulas & formula,const std::string & value);

#endif //CMR_TEX_PARSOR_H
