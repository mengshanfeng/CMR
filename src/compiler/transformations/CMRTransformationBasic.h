/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_TRANFORMATION_BASIC_H
#define CMR_TRANFORMATION_BASIC_H

/********************  HEADERS  *********************/
#include <string>
#include "CMRTransformation.h"

/*********************  TYPES  **********************/
class CMRProjectCodeEntry;
class CMRProjectCodeNode;
class CMRProjectCodeIteratorLoop;
class CMRProjectCodeEquation;
namespace CMRCompiler 
{
	class LatexFormulas;
	class LatexEntity;
};

/*********************  CLASS  **********************/
class CMRTransformationBasic : public CMRTransformation
{
	public:
		CMRTransformationBasic(const std::string& name);
		virtual void run(CMRProjectCodeEntry& entry);
	protected:
		virtual void transform(CMRProjectCodeNode & node);
		virtual void transform(CMRProjectCodeIteratorLoop & loop);
		virtual void transform(CMRProjectCodeEquation & equation);
		virtual void transform(CMRProjectCodeEquation & equation,CMRCompiler::LatexFormulas & formula);
		virtual void transform(CMRProjectCodeEquation & equation,CMRCompiler::LatexEntity & entity);
};

#endif //CMR_TRANFORMATION_BASIC_H
