/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <typeinfo>
#include "CMRTransformationBasic.h"
#include "../definitions/CMRProjectCode.h"

/*******************  FUNCTION  *********************/
CMRTransformationBasic::CMRTransformationBasic(const std::string& name)
	: CMRTransformation(name)
{
	
}

/*******************  FUNCTION  *********************/
void CMRTransformationBasic::run(CMRProjectCodeEntry& entry)
{
	CMRProjectCodeEntry * cur = &entry;
	assert(cur != NULL);
	
	if (dynamic_cast<CMRProjectCodeEquation*>(cur) != NULL)
	{
		this->transform(*dynamic_cast<CMRProjectCodeEquation*>(cur));
	} else if (dynamic_cast<CMRProjectCodeIteratorLoop*>(cur) != NULL) {
		this->transform(*dynamic_cast<CMRProjectCodeIteratorLoop*>(cur));
	} else if (dynamic_cast<CMRProjectCodeNode*>(cur) != NULL) {
		this->transform(*dynamic_cast<CMRProjectCodeNode*>(cur));
	} else if (dynamic_cast<CMRProjectCodeVarDecl*>(cur) != NULL) {
		//DO NOTHING
	} else {
		std::string tmp = std::string("Unknown code entry type : ") + typeid(*cur).name();
		throw CMRLatexException(tmp);
	}
}

/*******************  FUNCTION  *********************/
void CMRTransformationBasic::transform(CMRProjectCodeNode& node)
{
	CMRProjectCodeEntry * cur = node.getFirstChild();
	while (cur != NULL)
	{
		this->run(*cur);
		cur = cur->getNext();
	}
}

/*******************  FUNCTION  *********************/
void CMRTransformationBasic::transform(CMRProjectCodeIteratorLoop& loop)
{
	this->transform((CMRProjectCodeNode&)loop);
}

/*******************  FUNCTION  *********************/
void CMRTransformationBasic::transform(CMRProjectCodeEquation& equation)
{
	this->transform(equation,equation.getFormulas());
}

/*******************  FUNCTION  *********************/
void CMRTransformationBasic::transform(CMRProjectCodeEquation& equation, CMRLatexFormulas2& formula)
{
	for (CMRLatexEntityVector2::iterator it = formula.begin() ; it != formula.end() ; ++it)
		if (formula.hasInfo("cmrNoTranform") == false)
			this->transform(equation,**it);
}

/*******************  FUNCTION  *********************/
void CMRTransformationBasic::transform(CMRProjectCodeEquation& equation, CMRLatexEntity2& entity)
{
	CMRLatexFormulasVector2 & indices = entity.getIndices();
	for (CMRLatexFormulasVector2::iterator it = indices.begin() ; it != indices.end() ; ++it)
		if ((*it)->hasInfo("cmrNoTranform") == false)
			this->transform(equation,**it);
	
	CMRLatexFormulasVector2 & exponents = entity.getExponents();
	for (CMRLatexFormulasVector2::iterator it = exponents.begin() ; it != exponents.end() ; ++it)
		if ((*it)->hasInfo("cmrNoTranform") == false)
			this->transform(equation,**it);
	
	CMRLatexFormulasVector2 & params = entity.getParameters();
	for (CMRLatexFormulasVector2::iterator it = params.begin() ; it != params.end() ; ++it)
		if ((*it)->hasInfo("cmrNoTranform") == false)
			this->transform(equation,**it);
}
