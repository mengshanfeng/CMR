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
#include "common/Debug.h"
#include "FormulaMatcher.h"
#include "ParsorBasics.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  CONSTS  *********************/
static const std::string CAPTURE_TAG = "FormulaMatcher::captureFlag";

/*******************  FUNCTION  *********************/
FormulaMatcherResult::FormulaMatcherResult ( void )
{
	this->cntCaptureMatch = 0;
	this->cntExactMatch = 0;
	this->cntCompared = 0;
}

/*******************  FUNCTION  *********************/
FormulaMatcherResult::~FormulaMatcherResult ( void )
{

}

/*******************  FUNCTION  *********************/
FormulaMatcher::FormulaMatcher ( const std::string& model )
	:formula(model)
{

}

/*******************  FUNCTION  *********************/
void FormulaMatcher::markForCapture ( const std::string& value,const FormulaMatcherFilter * filter )
{
	//errors
	assert(value.empty() == false);
	
	//compile as entity
	LatexEntity entity(value);
	cmrAssume(entity.isOnlyOneName(),"Caution, you provide a complex entity (with exponent, indices...) for capture. Not supported up to now. ('%s')",value.c_str());
	
	//put capture flags on formula childs
	setupCaptureFlag(formula,entity,filter);
}

/*******************  FUNCTION  *********************/
void FormulaMatcher::setupCaptureFlag ( LatexEntity& entity, const LatexEntity& what,const FormulaMatcherFilter * filter )
{
	//check if entity is equal
	if (entity.getName() == what.getName())
	{
		entity.setExtraInfo(CAPTURE_TAG,(void*)filter);
	}
	//loop on childs
	setupCaptureFlag(entity.getIndices(),what,filter);
	setupCaptureFlag(entity.getExponents(),what,filter);
	setupCaptureFlag(entity.getParameters(),what,filter);
}

/*******************  FUNCTION  *********************/
void FormulaMatcher::setupCaptureFlag ( LatexFormulasVector& formula, const LatexEntity& what,const FormulaMatcherFilter * filter )
{
	for (LatexFormulasVector::iterator it = formula.begin() ; it != formula.end() ; ++it)
		setupCaptureFlag(**it,what,filter);
}

/*******************  FUNCTION  *********************/
void FormulaMatcher::setupCaptureFlag ( LatexFormulas& formula, const LatexEntity& what,const FormulaMatcherFilter * filter )
{
	for (LatexFormulas::iterator it = formula.begin() ; it != formula.end() ; ++it)
		setupCaptureFlag(**it,what,filter);
}

/*******************  FUNCTION  *********************/
void FormulaMatcher::dumpAsTree ( std::ostream& out, int indent ) const
{
	formula.dumpAsTree( out,indent);
}

/*******************  FUNCTION  *********************/
void FormulaMatcher::dumpAsXml ( std::ostream& out, int depth ) const
{
	formula.dumpAsXml(out,depth);
}

/*******************  FUNCTION  *********************/
bool FormulaMatcher::match ( const LatexFormulas& f, unsigned int mode ) const
{
	FormulaMatcherResult result;
	return match(f,result,mode);
}

/*******************  FUNCTION  *********************/
bool FormulaMatcher::match ( const LatexFormulas& f, FormulaMatcherResult& result, unsigned int mode ) const
{
	return internalMatch(formula,f,result,mode);
}

/*******************  FUNCTION  *********************/
bool FormulaMatcher::internalMatch ( const LatexFormulas& ref, const LatexFormulas& f, FormulaMatcherResult& result, unsigned int mode ) const
{
	//check for root partial
	bool rootPartial = mode & FORMULA_MATCHER_ROOT_PARTIAL;
	mode &= ~FORMULA_MATCHER_ROOT_PARTIAL;

	//loop and copare all elemnts
	LatexFormulas::const_iterator itRef = ref.begin();
	LatexFormulas::const_iterator itF = f.begin();
	while (itRef != ref.end() && itF != f.end())
	{
		//compare current entity, otherwise fail
		if ( ! internalMatch(**itRef,**itF,result,mode))
			return false;
		//move
		++itRef;
		++itF;
	}

	//check final status
	if (itRef == ref.end() && itF == f.end())
		return true;
	else if (itRef == ref.end())
		return ((result.cntExactMatch + result.cntCaptureMatch) > 0 && rootPartial);
	else 
		return false;
}

/*******************  FUNCTION  *********************/
bool FormulaMatcher::internalMatch ( const LatexEntity& ref, const LatexEntity& f, FormulaMatcherResult& result, unsigned int mode ) const
{
	bool skipParameters = false;
	bool okForCapture = false;
	
	//if capture check filter
	if (ref.hasInfo(CAPTURE_TAG))
	{
		const FormulaMatcherFilter * filter = (const FormulaMatcherFilter *)ref.getExtraInfo(CAPTURE_TAG);
		assert(filter != NULL);
		okForCapture = filter->accept(ref,f);
	}
	
	//check for capture
	if (okForCapture)
	{
		//if capture mode, extract the value
		if (mode & FORMULA_MATCHER_DO_CAPTURE)
		{
			cmrAssume(result.captures.find(ref.getName()) == result.captures.end(),"Try to capture two time the same parameter ! (%s => %s)",
			          ref.getName().c_str(),f.getString().c_str());
			//check for () sub grouping or direct value
			if (f.getName() == "()")
				result.captures[ref.getName()] = new LatexFormulas(f.getParameter(0)->getString());
			else
				result.captures[ref.getName()] = new LatexFormulas(f.getString());
		}
		
		//avoid to check parameters is () as by convention it contain the data
		if (f.getName() == "()")
			skipParameters = true;
	} else {
		//check for name
		if (ref.getName() != f.getName())
			return false;
	}

	//check for params
	if (!internalMatch(ref.getIndices(),f.getIndices(),result,mode))
		return false;
	if (!internalMatch(ref.getExponents(),f.getExponents(),result,mode))
		return false;
	if (!skipParameters && !internalMatch(ref.getParameters(),f.getParameters(),result,mode))
		return false;
	
	//update counts
	result.cntCompared++;
	if (ref.hasInfo(CAPTURE_TAG))
		result.cntCaptureMatch++;
	else
		result.cntExactMatch++;

	return true;
}

/*******************  FUNCTION  *********************/
bool FormulaMatcher::internalMatch ( const LatexFormulasVector& ref, const LatexFormulasVector& f, FormulaMatcherResult& result, unsigned int mode ) const
{
	//check for root partial
	bool rootPartial = mode & FORMULA_MATCHER_ROOT_PARTIAL;
	mode &= ~FORMULA_MATCHER_ROOT_PARTIAL;

	//loop and copare all elemnts
	LatexFormulasVector::const_iterator itRef = ref.begin();
	LatexFormulasVector::const_iterator itF = f.begin();
	while (itRef != ref.end() && itF != f.end())
	{
		//compare current entity, otherwise fail
		if ( ! internalMatch(**itRef,**itF,result,mode))
			return false;
		//move
		++itRef;
		++itF;
	}

	//check final status
	//check final status
	if (itRef == ref.end() && itF == f.end())
		return true;
	else if (itRef == ref.end())
		return ((result.cntExactMatch + result.cntCaptureMatch) > 0 && rootPartial);
	else
		return false;
}

/*******************  FUNCTION  *********************/
FormulaMatcherFilterDefault::FormulaMatcherFilterDefault ( unsigned int accepted )
{
	this->accepted = accepted;
}

/*******************  FUNCTION  *********************/
bool FormulaMatcherFilterDefault::accept ( const LatexEntity& model, const LatexEntity& entity ) const
{
	return getCategory(entity) & accepted;
}

/*******************  FUNCTION  *********************/
EntityCategory FormulaMatcherFilterDefault::getCategory ( const LatexEntity& entity ) const
{
	if (entity.getName().size() == 1 && cmrIsOperator(entity.getName()[0]))
	{
		assert(entity.isOnlyOneName());
		return ENTITY_CAT_OPERATOR;
	} else if (entity.getName() == "()") {
		return ENTITY_CAT_GROUP;
	} else {
		return ENTITY_CAT_MEMBER;
	}
}

};
