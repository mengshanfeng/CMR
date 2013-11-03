/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_MODEL_BASED_REPLACEMENT_H
#define CMR_MODEL_BASED_REPLACEMENT_H

/********************  HEADERS  *********************/
#include <string>
#include "../definitions/CMRProjectEntity.h"

/*********************  CLASS  **********************/
class CMRModelBasedReplacement : private CMRProjectEntity
{
	public:
		CMRModelBasedReplacement(const std::string & model,const std::string result);
		bool apply(CMRLatexEntity2 & entity);
	private:
		virtual void genDefinitionCCode(std::ostream& out, const CMRProjectContext& context, int indent = 0) const;
		virtual void genUsageCCode(std::ostream& out, const CMRProjectContext& context, const CMRLatexEntity2& entity, bool write = false) const;
	private:
		std::string result;
};

#endif //CMR_MODEL_BASED_REPLACEMENT_H
