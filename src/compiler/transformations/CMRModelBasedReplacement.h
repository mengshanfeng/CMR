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
#include "../definitions/ProjectEntityOld.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  CLASS  **********************/
class CMRModelBasedReplacement : public ProjectEntityOld
{
	public:
		CMRModelBasedReplacement(const std::string & model,const std::string result, bool captureAll = true);
		bool apply(CMRCompiler::LatexEntity & entity) const;
	private:
		virtual void genDefinitionCCode(std::ostream& out, const ProjectContext& context, int indent = 0) const;
		virtual void genUsageCCode(std::ostream& out, const ProjectContext& context, const CMRCompiler::LatexEntity& entity, bool write = false) const;
	private:
		std::string result;
};

}

#endif //CMR_MODEL_BASED_REPLACEMENT_H
