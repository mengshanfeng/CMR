/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <string>
#include "CMRTransformationMarkNoTransf.h"
#include "../definitions/CMRProjectEntity.h"
#include "../definitions/CMRProjectCode.h"

/*******************  FUNCTION  *********************/
CMRTransformationMarkNoTransf::CMRTransformationMarkNoTransf(void )
	: CMRTransformationBasic("CMRTransformationMarkNoTransf")
{
}

/*******************  FUNCTION  *********************/
void CMRTransformationMarkNoTransf::transform(CMRProjectCodeEquation& equation, CMRLatexEntity2& entity)
{
	//find entity
	const CMRProjectEntity * definition = equation.getContext().find(entity);
	
	//do it for all childs if def is OK
	if(definition != NULL)
	{
		markChildElements(entity.getIndices()   ,definition->getIndices());
		markChildElements(entity.getExponents() ,definition->getExponents());
		markChildElements(entity.getParameters(),definition->getParameters());
	}
	
	CMRTransformationBasic::transform(equation, entity);
}

/*******************  FUNCTION  *********************/
bool CMRTransformationMarkNoTransf::markChildElements(CMRLatexFormulasVector2& childs, const CMRProjectCaptureDefMap& defMap)
{
	//errors
	assert(childs.size() == defMap.size());
	
	for (int i = 0 ; i < childs.size() ; i++)
		if (defMap[i].captureType == CMR_CAPTURE_NONE)
			childs[i]->setExtraInfo("cmrNoTranform",NULL);

	/** @TODO also transform bu setting all the string a name of a uniq element, it may speedup next recongnitions **/
}
