/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "CMRTransformationExpandExpo.h"
#include "CMRTransformationReplace.h"
#include "../parsor/LatexEntity.h"
#include "../definitions/CMRProjectCode.h"

using namespace CMRCompiler;

/*******************  FUNCTION  *********************/
CMRTransformationExpandExpo::CMRTransformationExpandExpo ( void ) 
	: CMRTransformationBasic("CMRTransformationExpandExpo")
{

}

/*******************  FUNCTION  *********************/
void CMRTransformationExpandExpo::transform ( CMRProjectCodeEquation& equation, LatexEntity& entity )
{
	//vars
	const LatexFormulas * exponent = NULL;
	
	//trivial if no exponents or more than 1
	if (entity.countExponents() != 1)
		return;
	
	//find the definition
	const CMRProjectEntity * projectEntity = equation.getContext().find(entity);
	if (projectEntity == NULL)
		exponent = entity.exponents[0];
	else if (projectEntity->getExponents().empty())
		exponent = entity.exponents[0];	
	else 
		return;
	
	//convert to string for easer conpare
	std::string tmp = exponent->getString();
	//gen an entity without the exponent
	LatexFormulas innerPartF;
	LatexEntity * innerPart = new LatexEntity(entity);
	innerPartF.push_back(innerPart);
	innerPart->exponents.erase(innerPart->exponents.begin() + (innerPart->exponents.size()-1));
	CMRProjectCaptureMap values;
	values["x"] = &innerPartF;
	values["e"] = exponent;
	
	//manage special cases
	if (tmp == "2")
	{
		//build the new format
		LatexFormulas * f = new LatexFormulas("x*x");
		CMRTransformationReplace::replaceAll(values,*f);
		
		//change entity
		entity.clear();
		entity.name = "()";
		entity.parameters.push_back(f);
	} else if (tmp == "3") {
		//build the new format
		LatexFormulas * f = new LatexFormulas("x*x*x");
		CMRTransformationReplace::replaceAll(values,*f);
		
		//change entity
		entity.clear();
		entity.name = "()";
		entity.parameters.push_back(f);
	} else if (tmp == "1/2") {
		//build the new format
		LatexFormulas * f = new LatexFormulas("x");
		CMRTransformationReplace::replaceAll(values,*f);
			
		//change entity
		entity.clear();
		entity.name = "sqrt";
		entity.parameters.push_back(f);
	} else {
		//build the new format
		LatexFormulas * f = new LatexFormulas("x,e");
		CMRTransformationReplace::replaceAll(values,*f);
		
		//change entity
		entity.clear();
		entity.name = "pow";
		entity.parameters.push_back(f);
	}

	//loop on childs
	CMRTransformationBasic::transform ( equation, entity );
}
