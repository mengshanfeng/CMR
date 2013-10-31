/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_TRANSFORMATION_EXPAND_FRAC_H
#define CMR_TRANSFORMATION_EXPAND_FRAC_H

/********************  HEADERS  *********************/
#include <string>
#include "CMRTransformationBasic.h"
#include "../definitions/CMRProjectEntity.h"

/*********************  CLASS  **********************/
class CMRModelBasedReplacement : private CMRProjectEntity
{
	public:
		CMRModelBasedReplacement(const std::string & model,const std::string result);
		bool apply(CMRLatexEntity2 & entity);
	private:
		virtual void genDefinitionCCode(std::ostream& out, const CMRProjectContext& context, int indent = 0) const{};
		virtual void genUsageCCode(std::ostream& out, const CMRProjectContext& context, const CMRLatexEntity2& entity, bool write = false) const{};
	private:
		std::string result;
};

/*********************  CLASS  **********************/
class CMRTransformationExpandFrac : public CMRTransformationBasic
{
	public:
		CMRTransformationExpandFrac(void);
	protected:
		virtual void transform(CMRProjectCodeEquation & equation,CMRLatexEntity2 & entity);
		CMRModelBasedReplacement rules;
};

#endif //CMR_TRANSFORMATION_EXPAND_FRAC_H
