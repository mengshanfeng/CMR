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
#include "CMRTransformationReplace.h"
#include "../parsor/CMRLatexFormula.h"

/*******************  FUNCTION  *********************/
CMRTransformationReplace::CMRTransformationReplace(const CMRProjectCaptureMap* replaceMap)
	: CMRTransformationBasic("CMRTransformationReplace")
{
	//errors
	assert(replaceMap != NULL);
	
	//setup
	this->replaceMap = replaceMap;
}

/*******************  FUNCTION  *********************/
void CMRTransformationReplace::transform(CMRProjectCodeEquation& equation, CMRLatexEntity2& entity)
{
	replaceAll(*replaceMap,entity);
	
	CMRTransformationBasic::transform(equation, entity);
}

/*******************  FUNCTION  *********************/
const CMRLatexFormulas2* CMRTransformationReplace::searchReplaceValue(const CMRProjectCaptureMap & replaceMap,const CMRLatexEntity2& entity)
{
	//Vars
	const std::string entityString = entity.getString();

	//search
	for (CMRProjectCaptureMap::const_iterator it = replaceMap.begin() ; it != replaceMap.end() ; ++it)
		if (it->first == entityString)
			return it->second;
	
	return NULL;
}

/*******************  FUNCTION  *********************/
void CMRTransformationReplace::replaceAll(const CMRProjectCaptureMap & replaceMap, CMRLatexEntity2& entity)
{
	//Vars
	const CMRLatexFormulas2 * value = searchReplaceValue(replaceMap,entity);
	
	//replace if not null
	if (value != NULL)
	{
		if (value->size() == 1)
		{
			entity = *(*value)[0];
		} else {
			CMRLatexFormulas2 * f = new CMRLatexFormulas2(*value);
			entity.clear();
			entity.parameters.push_back(f);
			entity.name = "()";
		}
	}
}

/*******************  FUNCTION  *********************/
void CMRTransformationReplace::replaceAll(const CMRProjectCaptureMap& replaceMap, CMRLatexFormulas2& formula)
{
	for (int i = 0 ; i < formula.size() ; i++)
		replaceAll(replaceMap,*formula[i]);
}
