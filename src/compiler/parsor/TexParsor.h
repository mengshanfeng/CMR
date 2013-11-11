/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMPILER_TEX_PARSOR_H
#define CMR_COMPILER_TEX_PARSOR_H

/********************  HEADERS  *********************/
#include <string>
#include "LatexEntityOld.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  TYPES  **********************/
class LatexParsorContext;

/*******************  FUNCTION  *********************/
std::string cmrExtractName(const std::string & value,int & start);
void cmrParsorError(const std::string & value,int pos,const std::string & message="");
std::string cmrExtractSubZone(const std::string & value, int & start,char open = '{',char close = '}');
void cmrExtractSubAndSuperScript(const std::string & value,int & start,LatexEntityOld * entity);
void cmrParseLatexFormula(LatexFormulasOld & formula,const std::string & value);
int cmrRequireParameters(const std::string & name,const std::string & value,int pos);
void cmrReplaceByCommaGroup(LatexFormulasListOld & formulas);
LatexEntityOld * cmrParseLatexEntitySimple(const std::string & value,int & start);
LatexEntityOld * cmrExtractSubGroup(const std::string & value,int & start);
LatexEntityOld * cmrParseLatexEntity(const std::string & value,int & start);
void cmrSkipWhiteSpace(const std::string & value,int & cur);
void cmrParseLatexFormula(LatexFormulasOld & formula,const std::string & value);
int cmrRequireParameters(const std::string & name,LatexParsorContext & context);

}

#endif //CMR_COMPILER_TEX_PARSOR_H
