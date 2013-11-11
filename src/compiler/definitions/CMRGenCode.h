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
namespace CMRCompiler
{
	class LatexFormulas;
	class LatexEntity;
}
class CMRProjectContext;

/*******************  FUNCTION  *********************/
std::ostream& cmrGenEqCCode(std::ostream& out, const CMRProjectContext& context, const CMRCompiler::LatexEntity& entity);
std::ostream& cmrGenEqCCode ( std::ostream& out, const CMRProjectContext& context, const CMRCompiler::LatexFormulas& formula );
bool latexEntityIsInteger(const CMRCompiler::LatexEntity & entity);

#endif //CMR_GEN_CODE_H
