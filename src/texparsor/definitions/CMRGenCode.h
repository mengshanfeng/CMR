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
class CMRLatexFormulas2;
class CMRLatexEntity2;
class CMRProjectContext;

/*******************  FUNCTION  *********************/
std::ostream& cmrGenEqCCode(std::ostream& out, const CMRProjectContext& context, CMRLatexEntity2& entity);
std::ostream& cmrGenEqCCode ( std::ostream& out, const CMRProjectContext& context, CMRLatexFormulas2& formula );
bool latexEntityIsInteger(const CMRLatexEntity2 & entity);

#endif //CMR_GEN_CODE_H
