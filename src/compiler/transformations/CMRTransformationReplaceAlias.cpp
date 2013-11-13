/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "../common/Debug.h"
#include "../definitions/CMRProjectCode.h"
#include "../definitions/ProjectMathAlias.h"
#include "CMRTransformationReplaceAlias.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
CMRTransformationReplaceAlias::CMRTransformationReplaceAlias ( void ) 
	: CMRTransformationBasic("CMRTransformationReplaceAlias")
{
}

/*******************  FUNCTION  *********************/
void CMRTransformationReplaceAlias::transform ( CMRProjectCodeEquation& equation, LatexEntity& entity )
{
	//search entity
	const ProjectEntity * match = equation.getContext().find(entity);
	
	//if found, check if alias
	const ProjectMathAlias * alias = dynamic_cast<const ProjectMathAlias*>(match);
	if (alias != NULL)
	{
		bool res = alias->apply(entity);
		cmrAssume(res == true,"Fail to apply alias replacement.");
	}
	
	//apply transform on child
	CMRTransformationBasic::transform ( equation, entity );
}

}
