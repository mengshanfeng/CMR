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
#include "../parsor/LatexFormula.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
CMRTransformationReplace::CMRTransformationReplace(const ProjectCaptureMap* replaceMap)
	: CMRTransformationBasic("CMRTransformationReplace")
{
	//errors
	assert(replaceMap != NULL);
	
	//setup
	this->replaceMap = replaceMap;
}

/*******************  FUNCTION  *********************/
void CMRTransformationReplace::transform(CMRProjectCodeEquation& equation, LatexEntity& entity)
{
	replaceAll(*replaceMap,entity);
	
	CMRTransformationBasic::transform(equation, entity);
}

/*******************  FUNCTION  *********************/
const LatexFormulas* CMRTransformationReplace::searchReplaceValue(const ProjectCaptureMap & replaceMap,const LatexEntity& entity)
{
	//Vars
	const std::string entityString = entity.getString();

	//search
	for (ProjectCaptureMap::const_iterator it = replaceMap.begin() ; it != replaceMap.end() ; ++it)
		if (it->first == entityString)
			return it->second;
	
	return NULL;
}

/*******************  FUNCTION  *********************/
void CMRTransformationReplace::replaceAll ( const ProjectCaptureMap& replaceMap, LatexFormulasVector& formulas )
{
	for (LatexFormulasVector::iterator it = formulas.begin() ; it != formulas.end() ; ++it)
		replaceAll(replaceMap,**it);
}

/*******************  FUNCTION  *********************/
void CMRTransformationReplace::replaceAll(const ProjectCaptureMap & replaceMap, LatexEntity& entity)
{
	//Vars
	const LatexFormulas * value = searchReplaceValue(replaceMap,entity);
	
	//replace if not null
	if (value != NULL)
	{
		if (value->size() == 1)
		{
			entity = *(*value)[0];
		} else {
			LatexFormulas * f = new LatexFormulas(*value);
			entity.clear();
			entity.parameters.push_back(f);
			entity.name = "()";
		}
	}

	//recurse in childs
	replaceAll(replaceMap,entity.getExponents());
	replaceAll(replaceMap,entity.getIndices());
	replaceAll(replaceMap,entity.getParameters());
}

/*******************  FUNCTION  *********************/
void CMRTransformationReplace::replaceAll(const ProjectCaptureMap& replaceMap, LatexFormulas& formula)
{
	for (int i = 0 ; i < formula.size() ; i++)
		replaceAll(replaceMap,*formula[i]);
}

}
