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


////////////////////////////////////////////////////////////
CMRModelBasedReplacement::CMRModelBasedReplacement(const std::string& model, const std::string result)
	:CMRProjectEntity(model,"model")
{
	//errors
	assert(model.empty() == false);
	assert(result.empty() == false);

	//setup
	this->result = result;
	
	//mark all as capture
	this->markAllCaptureAs(CMR_CAPTURE_REQUIRED);
}

bool CMRModelBasedReplacement::apply(CMRLatexEntity2& entity)
{
	//try to match
	if (this->match(entity) == false)
		return false;
	
	//capture fields
	CMRProjectCaptureMap capture;
	this->capture(entity,capture);
	
	//create the new instance
	CMRLatexFormulas2 * newEntity = new CMRLatexFormulas2(result);
	
	//replace values
	CMRTransformationReplace::replaceAll(capture,*newEntity);
	
	//replace
	if (newEntity->size() == 1)
	{
		entity = *(*newEntity)[0];
	} else {
		entity.clear();
		entity.parameters.push_back(newEntity);
		entity.name = "()";
	}
}
////////////////////////////////////////////////////////////

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
