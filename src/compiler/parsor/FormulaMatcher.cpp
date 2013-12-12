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
#include <sstream>
#include "common/Debug.h"
#include "FormulaMatcher.h"
#include "ParsorBasics.h"

/**********************  USING  *********************/
using namespace std;

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
FormulaMatcher::~FormulaMatcher ( void )
{
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
	if ( ! setupCaptureFlag(formula,entity,filter) )
		throw LatexException(string("Invalid name to change capture type (") + value + string(") in : ") + this->toString());
}

/*******************  FUNCTION  *********************/
bool FormulaMatcher::setupCaptureFlag ( LatexEntity& entity, const LatexEntity& what,const FormulaMatcherFilter * filter )
{
	bool res = false;
	//check if entity is equal
	if (entity.getName() == what.getName())
	{
		entity.setExtraInfo(CAPTURE_TAG,(void*)filter);
		res = true;
	}
	//loop on childs
	res = setupCaptureFlag(entity.getIndices(),what,filter) || res;
	res = setupCaptureFlag(entity.getExponents(),what,filter) || res;
	res = setupCaptureFlag(entity.getParameters(),what,filter) || res;
	return res;
}

/*******************  FUNCTION  *********************/
bool FormulaMatcher::setupCaptureFlag ( LatexFormulasVector& formula, const LatexEntity& what,const FormulaMatcherFilter * filter )
{
	bool res = false;
	for (LatexFormulasVector::iterator it = formula.begin() ; it != formula.end() ; ++it)
		res = setupCaptureFlag(**it,what,filter) || res;
	return res;
}

/*******************  FUNCTION  *********************/
bool FormulaMatcher::setupCaptureFlag ( LatexFormulas& formula, const LatexEntity& what,const FormulaMatcherFilter * filter )
{
	bool res = false;
	for (LatexFormulas::iterator it = formula.begin() ; it != formula.end() ; ++it)
		res = setupCaptureFlag(**it,what,filter) || res;

	return res;
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
bool FormulaMatcher::match ( const LatexFormulas& f, LatexFormulas::const_iterator& startIt, unsigned int mode ) const
{
	FormulaMatcherResult result;
	return match(f,startIt,result,mode);
}

/*******************  FUNCTION  *********************/
bool FormulaMatcher::match ( const LatexFormulas& f, FormulaMatcherResult& result, unsigned int mode ) const
{
	LatexFormulas::const_iterator it = f.begin();
	return match(f,it,result,mode);
}

/*******************  FUNCTION  *********************/
bool FormulaMatcher::match ( const LatexFormulas& f, LatexFormulas::const_iterator& startIt, FormulaMatcherResult& result, unsigned int mode ) const
{
	return internalMatch(formula,f,startIt,result,mode);
}

/*******************  FUNCTION  *********************/
bool FormulaMatcher::internalMatch ( const LatexFormulas& ref, const LatexFormulas& f, LatexFormulas::const_iterator & startIt, FormulaMatcherResult& result, unsigned int mode ) const
{
	//check for root partial
	bool rootPartial = mode & FORMULA_MATCHER_ROOT_PARTIAL;
	mode &= ~FORMULA_MATCHER_ROOT_PARTIAL;
	bool res;

	//loop and copare all elemnts
	LatexFormulas::const_iterator itRef = ref.begin();
	LatexFormulas::const_iterator itF = startIt;
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
		res = true;
	else if (itRef == ref.end())
		res = ((result.cntExactMatch + result.cntCaptureMatch) > 0 && rootPartial);
	else 
		res = false;
	
	//update startit
	if (res)
		startIt = itF;

	return res;
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
		LatexFormulas::const_iterator it = (*itF)->begin();
		if ( ! internalMatch(**itRef,**itF,it,result,mode))
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

/*******************  FUNCTION  *********************/
std::ostream& operator<< ( std::ostream & out,const FormulaMatcher& value )
{
	out << value.formula;
	return out;
}

/*******************  FUNCTION  *********************/
void FormulaMatcher::printDebug ( std::ostream& out ) const
{
	this->printDebug(out,this->formula);
}

/*******************  FUNCTION  *********************/
void FormulaMatcher::printDebug ( std::ostream& out, const LatexFormulas& f ) const
{
	for(LatexFormulas::const_iterator it = f.begin() ; it != f.end() ; ++it)
		printDebug(out,**it);
}

/*******************  FUNCTION  *********************/
void FormulaMatcher::printDebug ( std::ostream& out, const LatexEntity& entity ) const
{
	if (entity.hasInfo(CAPTURE_TAG))
	{
		out << "[";
		out << entity.getName();
		out << "]";
		this->printDebug(out,entity.getIndices(),"_");
		this->printDebug(out,entity.getExponents(),"^");
		this->printDebug(out,entity.getParameters(),"");
	} else if (entity.getName() == "()") {
		out << "(";
		for (LatexFormulas::const_iterator it = entity.getParameter(0)->begin() ; it != entity.getParameter(0)->end() ; ++it)
			printDebug(out,**it);
		out << ")";
	} else {
		out << entity.getName();
		this->printDebug(out,entity.getIndices(),"_");
		this->printDebug(out,entity.getExponents(),"^");
		this->printDebug(out,entity.getParameters(),"");
	}
}

/*******************  FUNCTION  *********************/
void FormulaMatcher::printDebug ( std::ostream& out, const CMRCompiler::LatexFormulasVector& childs, const std::string& sep ) const
{
	//trivial
	if (childs.empty())
		return;
	
	//setparator
	out << sep;
	
	//only one
	if ((sep == "_" || sep == "^") && childs.size() == 1)
	{
		printDebug(out,*childs[0]);
	} else if (sep == "_" || sep == "^") {
		out << "{";
		for (LatexFormulasVector::const_iterator it = childs.begin() ; it !=childs.end() ; ++it)
		{
			if (it != childs.begin())
				out << ",";
			printDebug(out,**it);
		}
		out << "}";
	} else {
		for (LatexFormulasVector::const_iterator it = childs.begin() ; it !=childs.end() ; ++it)
		{
			out << "{";
			printDebug(out,**it);
			out << "}";
		}
	}
}

/*******************  FUNCTION  *********************/
std::string FormulaMatcher::toString ( void ) const
{
	stringstream out;
	out << *this;
	return out.str();
}

/*******************  FUNCTION  *********************/
FormulaMatcher::operator std::string() const
{
	return toString();
}

/*******************  FUNCTION  *********************/
void FormulaMatcher::capture ( const LatexFormulas& f, FormulaMatcherResult& result, unsigned int mode ) const
{
	bool res = match(f,result,mode);
	if (res == false)
		throw LatexException(string("Try to capture values in non mathing entities : ")+this->toString()+ string(" != ")+f.getString());
}

/*******************  FUNCTION  *********************/
void FormulaMatcher::capture ( const LatexFormulas& f, LatexFormulas::const_iterator& startIt, FormulaMatcherResult& result, unsigned int mode ) const
{
	bool res = match(f,startIt,result,mode);
	if (res == false)
		throw LatexException(string("Try to capture values in non mathing entities : ")+this->toString()+ string(" != ")+f.getString());
}


};
