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
#include "../definitions/ProjectEntityOld.h"
#include "../definitions/CMRProjectCode.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
CMRTransformationMarkNoTransf::CMRTransformationMarkNoTransf(void )
	: CMRTransformationBasic("CMRTransformationMarkNoTransf")
{
}

/*******************  FUNCTION  *********************/
void CMRTransformationMarkNoTransf::transform(CMRProjectCodeEquation& equation, LatexEntity& entity)
{
	//find entity
	const IProjectEntity * definition = equation.getContext().find(entity);
	
#pragma TODO rewrite this
// 	//do it for all childs if def is OK
// 	if(definition != NULL)
// 	{
// 		markChildElements(entity.getIndices()   ,definition->getIndices());
// 		markChildElements(entity.getExponents() ,definition->getExponents());
// 		markChildElements(entity.getParameters(),definition->getParameters());
// 	}
	
	CMRTransformationBasic::transform(equation, entity);
}

/*******************  FUNCTION  *********************/
void CMRTransformationMarkNoTransf::markChildElements(LatexFormulasVector& childs, const ProjectCaptureDefMap& defMap)
{
	//errors
	assert(childs.size() >= defMap.size());
	
	for (int i = 0 ; i < defMap.size() ; i++)
		if (defMap[i].captureType == CAPTURE_NONE)
			childs[i]->setExtraInfo("cmrNoTranform",NULL);

	/** @TODO also transform bu setting all the string a name of a uniq element, it may speedup next recongnitions **/
}

}
