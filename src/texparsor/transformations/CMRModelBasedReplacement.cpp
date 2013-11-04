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
#include "CMRModelBasedReplacement.h"
#include "CMRTransformationReplace.h"
#include "../parsor/CMRLatexFormula.h"
#include "../common/CMRDebug.h"

/*******************  FUNCTION  *********************/
CMRModelBasedReplacement::CMRModelBasedReplacement(const std::string& model, const std::string result, bool captureAll)
	:CMRProjectEntity(model,"model")
{
	//errors
	assert(model.empty() == false);
	assert(result.empty() == false);

	//setup
	this->result = result;
	
	//mark all as capture
	if (captureAll)
		this->markAllCaptureAs(CMR_CAPTURE_REQUIRED);
}

/*******************  FUNCTION  *********************/
bool CMRModelBasedReplacement::apply( CMRLatexEntity2& entity ) const
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
	
	return true;
}

/*******************  FUNCTION  *********************/
void CMRModelBasedReplacement::genDefinitionCCode ( std::ostream& out, const CMRProjectContext& context, int indent ) const
{
	cmrFatal("Caution, CMRModelBasedReplacement objects cannot be translated to C++ code.");
}

/*******************  FUNCTION  *********************/
void CMRModelBasedReplacement::genUsageCCode ( std::ostream& out, const CMRProjectContext& context, const CMRLatexEntity2& entity, bool write ) const
{
	cmrFatal("Caution, CMRModelBasedReplacement objects cannot be translated to C++ code.");
}
