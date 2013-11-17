/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cassert>
#include "CMRTransformationMathFunc.h"
#include "CMRTransformationReplace.h"
#include "../parsor/LatexFormula.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
CMRTransformationMathFunc::CMRTransformationMathFunc(void )
	:CMRTransformationBasic("CMRTransformationMathFunc")
	,sqrtRule("\\sqrt{a}","\\mathrm{sqrt}(a)")
{

}

/*******************  FUNCTION  *********************/
void CMRTransformationMathFunc::transform(CMRProjectCodeEquation& equation, LatexEntity& entity)
{
	sqrtRule.apply(entity);
	
	CMRTransformationBasic::transform(equation, entity);
}

}
