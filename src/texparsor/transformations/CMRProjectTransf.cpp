/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "CMRProjectTransf.h"
#include "../definitions/CMRProjectCode.h"

/*******************  FUNCTION  *********************/
CMRProjectTransformation::CMRProjectTransformation(const std::string& name)
{
	this->name = name;
}

/*******************  FUNCTION  *********************/
CMRProjectTransformation::~CMRProjectTransformation(void)
{
}

/*******************  FUNCTION  *********************/
const std::string& CMRProjectTransformation::getName(void ) const
{
	return name;
}

/*******************  FUNCTION  *********************/
CMRProjectTransformationBasic::CMRProjectTransformationBasic(const std::string& name)
	: CMRProjectTransformation(name)
{
	
}

/*******************  FUNCTION  *********************/
void CMRProjectTransformationBasic::run(CMRProjectCodeEntry& entry)
{
	CMRProjectCodeEntry * cur = &entry;
	assert(cur != NULL);
	
	if (dynamic_cast<CMRProjectCodeEquation*>(cur) != NULL)
		this->transform(*dynamic_cast<CMRProjectCodeEquation*>(cur));
	else if (dynamic_cast<CMRProjectCodeIteratorLoop*>(cur) != NULL)
		this->transform(*dynamic_cast<CMRProjectCodeIteratorLoop*>(cur));
	else if (dynamic_cast<CMRProjectCodeNode*>(cur) != NULL)
		this->transform(*dynamic_cast<CMRProjectCodeNode*>(cur));
	else
		throw CMRLatexException("Unknown code entry type !");
}

/*******************  FUNCTION  *********************/
void CMRProjectTransformationBasic::transform(CMRProjectCodeNode& node)
{
	CMRProjectCodeEntry * cur = node.getFirstChild();
	while (cur != NULL)
	{
		this->run(*cur);
		cur = cur->getNext();
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectTransformationBasic::transform(CMRProjectCodeIteratorLoop& loop)
{
	this->transform((CMRProjectCodeNode&)loop);
}

/*******************  FUNCTION  *********************/
void CMRProjectTransformationBasic::transform(CMRProjectCodeEquation& equation)
{
	this->transform(equation,equation.getFormulas());
}

/*******************  FUNCTION  *********************/
void CMRProjectTransformationBasic::transform(CMRProjectCodeEquation& equation, CMRLatexFormulas2& formula)
{
	for (CMRLatexEntityVector2::iterator it = formula.begin() ; it != formula.end() ; ++it)
		this->transform(equation,**it);
}

/*******************  FUNCTION  *********************/
void CMRProjectTransformationBasic::transform(CMRProjectCodeEquation& equation, CMRLatexEntity2& entity)
{
	CMRLatexFormulasVector2 & indices = entity.getIndices();
	for (CMRLatexFormulasVector2::iterator it = indices.begin() ; it != indices.end() ; ++it)
		this->transform(equation,**it);
	
	CMRLatexFormulasVector2 & exponents = entity.getExponents();
	for (CMRLatexFormulasVector2::iterator it = exponents.begin() ; it != exponents.end() ; ++it)
		this->transform(equation,**it);
	
	CMRLatexFormulasVector2 & params = entity.getParameters();
	for (CMRLatexFormulasVector2::iterator it = params.begin() ; it != params.end() ; ++it)
		this->transform(equation,**it);
}
