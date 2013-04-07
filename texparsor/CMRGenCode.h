/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_GEN_CODE_H
#define CMR_GEN_CODE_H

/********************  HEADERS  *********************/
#include <ostream>

/*********************  TYPES  **********************/
struct CMRLatexFormulas;
struct CMRLatexEntity;
struct CMRProjectContext;

/*******************  FUNCTION  *********************/
std::ostream& cmrGenEqCCode(std::ostream& out, CMRProjectContext& context, CMRLatexEntity& entity);
std::ostream& cmrGenEqCCode ( std::ostream& out, CMRProjectContext& context, CMRLatexFormulas& formula );
bool latexEntityIsInteger(CMRLatexEntity & entity);

#endif //CMR_GEN_CODE_H
