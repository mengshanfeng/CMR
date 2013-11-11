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
#include "../definitions/CMRProjectAlias.h"
#include "CMRTransformationReplaceAlias.h"

using namespace CMRCompiler;

/*******************  FUNCTION  *********************/
CMRTransformationReplaceAlias::CMRTransformationReplaceAlias ( void ) 
	: CMRTransformationBasic("CMRTransformationReplaceAlias")
{
}

/*******************  FUNCTION  *********************/
void CMRTransformationReplaceAlias::transform ( CMRProjectCodeEquation& equation, LatexEntity& entity )
{
	//search entity
	const CMRProjectEntity * match = equation.getContext().find(entity);
	
	//if found, check if alias
	const CMRProjectAlias * alias = dynamic_cast<const CMRProjectAlias*>(match);
	if (alias != NULL)
	{
		bool res = alias->apply(entity);
		cmrAssume(res == true,"Fail to apply alias replacement.");
	}
	
	//apply transform on child
	CMRTransformationBasic::transform ( equation, entity );
}
