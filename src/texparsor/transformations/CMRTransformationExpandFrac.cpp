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
#include "CMRTransformationExpandFrac.h"
#include "CMRTransformationReplace.h"
#include "../parsor/CMRLatexFormula.h"

/*******************  FUNCTION  *********************/
CMRTransformationExpandFrac::CMRTransformationExpandFrac(void )
	:CMRTransformationBasic("CMRTransformationExpandFrac")
	,rules("\\frac{a}{b}","a/b")
{

}

/*******************  FUNCTION  *********************/
void CMRTransformationExpandFrac::transform(CMRProjectCodeEquation& equation, CMRLatexEntity2& entity)
{
	rules.apply(entity);
	
	CMRTransformationBasic::transform(equation, entity);
}
