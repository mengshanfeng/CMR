/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_PROJECT_ALIAS_H
#define CMR_PROJECT_ALIAS_H

/********************  HEADERS  *********************/
#include "CMRProjectEntity.h"
#include "../transformations/CMRModelBasedReplacement.h"

/*********************  CLASS  **********************/
class CMRProjectAlias : public CMRModelBasedReplacement
{
	public:
		CMRProjectAlias ( const std::string& latexName, const std::string & result, bool captureAll = true );
		virtual void genDefinitionCCode ( std::ostream& out, const CMRProjectContext& context, int padding = 0 ) const;
		virtual void genUsageCCode ( std::ostream& out, const CMRProjectContext& context, const CMRLatexEntity2& entity, bool write = false ) const;
};

#endif //CMR_PROJECT_ALIAS_H
