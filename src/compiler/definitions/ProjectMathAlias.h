/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMPILER_PROJECT_ALIAS_H
#define CMR_COMPILER_PROJECT_ALIAS_H

/********************  HEADERS  *********************/
#include "transformations/CMRModelBasedReplacement.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  CLASS  **********************/
class ProjectMathAlias : public CMRModelBasedReplacement
{
	public:
		ProjectMathAlias ( const std::string& latexName, const std::string & result, bool captureAll = true );
		virtual void genDefinitionCCode ( std::ostream& out, const ProjectContext& context, int padding = 0 ) const;
		virtual void genUsageCCode ( std::ostream& out, const ProjectContext& context, const LatexEntity& entity, bool write = false ) const;
};

}

#endif //CMR_COMPILER_PROJECT_ALIAS_H
