/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMPILER_GEN_CODE_H
#define CMR_COMPILER_GEN_CODE_H

/********************  HEADERS  *********************/
#include <ostream>

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  TYPES  **********************/
class CMRProjectContext;
class LatexFormulas;
class LatexEntity;

/*******************  FUNCTION  *********************/
std::ostream& cmrGenEqCCode(std::ostream& out, const CMRProjectContext& context, const LatexEntity& entity);
std::ostream& cmrGenEqCCode ( std::ostream& out, const CMRProjectContext& context, const LatexFormulas& formula );
bool latexEntityIsInteger(const LatexEntity & entity);

}

#endif //CMR_COMPILER_GEN_CODE_H
