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
namespace CMRCompiler{

}
struct CMRProjectContext;

/*******************  FUNCTION  *********************/
std::ostream& cmrGenEqCCode(std::ostream& out, const CMRProjectContext& context, CMRCompiler::LatexEntityOld& entity);
std::ostream& cmrGenEqCCode ( std::ostream& out, const CMRProjectContext& context, CMRCompiler::LatexFormulasOld& formula );
bool latexEntityIsInteger(CMRCompiler::LatexEntityOld & entity);

#endif //CMR_GEN_CODE_H
