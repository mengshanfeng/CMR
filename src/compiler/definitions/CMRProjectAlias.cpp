/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "../common/Debug.h"
#include "CMRProjectAlias.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
CMRProjectAlias::CMRProjectAlias ( const std::string& latexName, const std::string& result, bool captureAll ) 
	: CMRModelBasedReplacement ( latexName , result , captureAll )
{
}

/*******************  FUNCTION  *********************/
void CMRProjectAlias::genDefinitionCCode ( std::ostream& out, const CMRProjectContext& context, int padding ) const
{
	cmrFatal("CMRProjectAlias object are not aimed to be converted to C++ code, please run the alias transformation pass.");
}

/*******************  FUNCTION  *********************/
void CMRProjectAlias::genUsageCCode ( std::ostream& out, const CMRProjectContext& context, const LatexEntity& entity, bool write ) const
{
	cmrFatal("CMRProjectAlias object are not aimed to be converted to C++ code, please run the alias transformation pass.");
}

}
