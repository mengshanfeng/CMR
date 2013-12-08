#ifndef CMR_MOCK_TRANSF_BASIC_H
#define CMR_MOCK_TRANSF_BASIC_H

/********************  HEADERS  *********************/
#include "../CMRTransformationBasic.h"
#include <definitions/CMRProjectCode.h>

/*********************  CLASS  **********************/
class MockTransfBasic : public CMRCompiler::CMRTransformationBasic
{
	public:
		MockTransfBasic(const std::string & tag, const std::string marker);
		virtual void transform(CMRCompiler::CMRProjectCodeNode& node);
		virtual void transform(CMRCompiler::CMRProjectCodeIteratorLoop& loop);
		virtual void transform(CMRCompiler::CMRProjectCodeEquation& equation, CMRCompiler::LatexFormulas& formula);
		virtual void transform(CMRCompiler::CMRProjectCodeEquation& equation, CMRCompiler::LatexEntity& entity);
		virtual void transform(CMRCompiler::CMRProjectCodeEquation& equation);
		std::stringstream out;
	protected:
		std::string tag;
		std::string marker;
};

/*******************  FUNCTION  *********************/
MockTransfBasic::MockTransfBasic( const std::string& tag, const std::string marker )
	: CMRCompiler::CMRTransformationBasic("mock")
{
	this->marker = marker;
	this->tag = tag;
}

/*******************  FUNCTION  *********************/
void MockTransfBasic::transform(CMRCompiler::CMRProjectCodeNode& node)
{
	out << "NODE[ ";
	CMRCompiler::CMRTransformationBasic::transform(node);
	out << " ]"; 
}

/*******************  FUNCTION  *********************/
void MockTransfBasic::transform(CMRCompiler::CMRProjectCodeEquation& equation, CMRCompiler::LatexFormulas& formula)
{
	if (formula.hasInfo(tag))
		out << "FORMULA_" << marker << " [ ";
	else
		out << "FORMULA[ ";
	CMRCompiler::CMRTransformationBasic::transform(equation,formula);
	out << " ]"; 
}

/*******************  FUNCTION  *********************/
void MockTransfBasic::transform(CMRCompiler::CMRProjectCodeIteratorLoop& loop)
{
	out << "LOOP[ " << loop.getIterator().getLatexName() << " , ";
	CMRCompiler::CMRTransformationBasic::transform(loop);
	out << " ]";
}

/*******************  FUNCTION  *********************/
void MockTransfBasic::transform(CMRCompiler::CMRProjectCodeEquation& equation, CMRCompiler::LatexEntity& entity)
{
	if (entity.hasInfo(tag))
		out << "ENTITY_" << marker << " [ " << entity.getName() << " , " ;
	else
		out << "ENTITY [ " << entity.getName() << " , ";
	CMRCompiler::CMRTransformationBasic::transform(equation, entity);
	out << " ]";
}

/*******************  FUNCTION  *********************/
void MockTransfBasic::transform(CMRCompiler::CMRProjectCodeEquation& equation)
{
	out << "EQUATION [ " << equation.getOutput() << " , ";
	CMRCompiler::CMRTransformationBasic::transform(equation);
	out << " ]";
}

#endif //CMR_MOCK_TRANSF_BASIC_H
