/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "common/Debug.h"
#include "ProjectMathAlias.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
ProjectMathAlias::ProjectMathAlias ( const std::string& latexName, const std::string& result, bool captureAll ) 
	: CMRModelBasedReplacement ( latexName , result , captureAll )
{
}

/*******************  FUNCTION  *********************/
void ProjectMathAlias::genDefinitionCCode ( std::ostream& out, const ProjectContext& context, int padding ) const
{
	cmrFatal("CMRProjectAlias object are not aimed to be converted to C++ code, please run the alias transformation pass.");
}

/*******************  FUNCTION  *********************/
void ProjectMathAlias::genUsageCCode ( std::ostream& out, const ProjectContext& context, const LatexEntity& entity, bool write ) const
{
	cmrFatal("CMRProjectAlias object are not aimed to be converted to C++ code, please run the alias transformation pass.");
}

}
