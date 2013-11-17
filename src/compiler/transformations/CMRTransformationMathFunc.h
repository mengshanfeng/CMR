/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_TRANSFORMATION_MATH_FUNC_H
#define CMR_TRANSFORMATION_MATH_FUNC_H

/********************  HEADERS  *********************/
#include "CMRTransformationBasic.h"
#include "CMRModelBasedReplacement.h"
#include "../definitions/ProjectEntity.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  CLASS  **********************/
class CMRTransformationMathFunc : public CMRTransformationBasic
{
	public:
		CMRTransformationMathFunc(void);
	protected:
		virtual void transform(CMRProjectCodeEquation & equation,CMRCompiler::LatexEntity & entity);
		CMRModelBasedReplacement sqrtRule;
};

}

#endif //CMR_TRANSFORMATION_EXPAND_FRAC_H
