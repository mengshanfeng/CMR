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
static const std::string OPTIONAL_TAG = "FormulaMatcher::optionalFlag";

/*******************  FUNCTION  *********************/
FormulaMatcherResult::FormulaMatcherResult ( void )
{
	this->exponent = NULL;
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
FormulaMatcher::FormulaMatcher ( const string& model, bool allowMultipleRootElmts )
	:formula(model)
	
{
	this->allowExponent = false;
	if (! allowMultipleRootElmts  && formula.size() > 1)
		throw LatexException(string("Caution, multiple elements are not allowed in root element and you provide : ") + model);
}

/*******************  FUNCTION  *********************/
void FormulaMatcher::markForCapture ( const string& value, const CMRCompiler::FormulaMatcherFilter* filter, bool optional )
{
	//errors
	assert(value.empty() == false);
	
	//compile as entity
	LatexEntity entity(value);
	cmrAssume(entity.isOnlyOneName(),"Caution, you provide a complex entity (with exponent, indices...) for capture. Not supported up to now. ('%s')",value.c_str());
	
	//put capture flags on formula childs
	if ( ! setupCaptureFlag(formula,entity,filter,optional) )
		throw LatexException(string("Invalid name to change capture type (") + value + string(") in : ") + this->toString());
}

/*******************  FUNCTION  *********************/
bool FormulaMatcher::setupCaptureFlag ( LatexEntity& entity, const LatexEntity& what, const FormulaMatcherFilter* filter, bool optional )
{
	bool res = false;
	//check if entity is equal
	if (entity.getName() == what.getName())
	{
		entity.setExtraInfo(CAPTURE_TAG,(void*)filter);
		if (optional)
			entity.setExtraInfo(OPTIONAL_TAG,(void*)filter);
		res = true;
	}
	//loop on childs
	res = setupCaptureFlag(entity.getIndices(),what,filter,optional) || res;
	res = setupCaptureFlag(entity.getExponents(),what,filter,optional) || res;
	res = setupCaptureFlag(entity.getParameters(),what,filter,optional) || res;
	return res;
}

/*******************  FUNCTION  *********************/
bool FormulaMatcher::setupCaptureFlag ( LatexFormulasVector& formula, const LatexEntity& what, const FormulaMatcherFilter* filter, bool optional )
{
	bool res = false;
	for (LatexFormulasVector::iterator it = formula.begin() ; it != formula.end() ; ++it)
		res = setupCaptureFlag(**it,what,filter,optional) || res;
	return res;
}

/*******************  FUNCTION  *********************/
bool FormulaMatcher::setupCaptureFlag ( LatexFormulas& formula, const LatexEntity& what, const FormulaMatcherFilter* filter, bool optional )
{
	bool res = false;
	for (LatexFormulas::iterator it = formula.begin() ; it != formula.end() ; ++it)
		res = setupCaptureFlag(**it,what,filter,optional) || res;

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
		{
			//if optional, move forward only on ref, otherwise it's failure
			if ((*itRef)->hasInfo(OPTIONAL_TAG))
				++itRef;
			else
				return false;
		} else {
			//move all
			++itRef;
			++itF;
		}
	}
	
	//skip optional ref if have some
	while (itRef != ref.end() && (*itRef)->hasInfo(OPTIONAL_TAG))
		itRef++;

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
	unsigned int modeForExp = mode;
	//remove flag for childs, this is only for root element
	mode &= ~FORMULA_MATCHER_ROOT_OP_EXPO;
	
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
			cmrAssume(!result.hasExtract(ref.getName()),"Try to capture two time the same parameter ! (%s => %s)",
			          ref.getName().c_str(),f.getString().c_str());
			//check for () sub grouping or direct value
			if (f.getName() == "()")
				result.captures[ref.getName()] = new LatexFormulas(f.getParameter(0)->getString());
			else
				result.captures[ref.getName()] = new LatexFormulas(f.getName());
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
	if (!internalMatch(ref.getExponents(),f.getExponents(),result,modeForExp))
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
	bool optionalExpo = (mode & FORMULA_MATCHER_ROOT_OP_EXPO) && allowExponent;
	mode &= ~FORMULA_MATCHER_ROOT_OP_EXPO;

	//loop and copare all elemnts
	LatexFormulasVector::const_iterator itRef = ref.begin();
	LatexFormulasVector::const_iterator itF = f.begin();
	while (itRef != ref.end() && itF != f.end())
	{
		//compare current entity, otherwise fail
		LatexFormulas::const_iterator it = (*itF)->begin();
		if ( ! internalMatch(**itRef,**itF,it,result,mode))
		{
			//if optional, move forward only on ref, otherwise it's failure
			if (isOptionalFormula (**itRef))
				++itRef;
			else
				return false;
		} else {
			//move all
			++itRef;
			++itF;
		}
	}
	
	//skip optional ref if have some
	while (itRef != ref.end() && isOptionalFormula(**itRef))
		itRef++;
	
	//manage optional exponent
	if (optionalExpo && itRef == ref.end() && itF != f.end())
	{
		if (mode & FORMULA_MATCHER_DO_CAPTURE)
			result.exponent = new LatexFormulas((*itF)->getString());
		result.cntCaptureMatch++;
		result.cntCompared++;
		itF++;
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
void FormulaMatcher::printDebug ( std::ostream& out, const LatexFormulasVector& childs, const std::string& sep ) const
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
string FormulaMatcher::toDebugString ( void ) const
{
	stringstream out;
	printDebug(out);
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
	bool res = match(f,result,mode|FORMULA_MATCHER_DO_CAPTURE);
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

/*******************  FUNCTION  *********************/
void FormulaMatcher::setOptionalExponent ( bool force )
{
	//check is simple entity, not complex one
	if (this->formula.size() > 1)
	{
		if (force)
			throw LatexException(string("You cannot use setOptionalExponent() on complexe entity definitions (multiple sub-entities in name without ()), here it is : ") + this->toString());
		else
			return;
	} else {
		this->allowExponent = true;
	}
}

/*******************  FUNCTION  *********************/
bool FormulaMatcherResult::hasExtract ( const string& key ) const
{
	return (captures.find(key) != captures.end());
}

/*******************  FUNCTION  *********************/
bool FormulaMatcher::isOptionalFormula ( const LatexFormulas& f ) const
{
	for (LatexFormulas::const_iterator it = f.begin() ; it != f.end() ; ++it)
		if ( ! (*it)->hasInfo(OPTIONAL_TAG) )
			return false;
	return true;
}

/*******************  FUNCTION  *********************/
void FormulaMatcherResult::printDebug ( ostream& out ) const
{
	if (captures.empty())
		out << " - NO CAPTURES" << endl;
	else
		for (FormulaMatcherCaptureMap::const_iterator it = captures.begin() ; it != captures.end() ; ++it)
			out << " - " << it->first << " : " << it->second->getString() << endl;
}

/*******************  FUNCTION  *********************/
string FormulaMatcherResult::toString ( void ) const
{
	stringstream buffer;
	printDebug(buffer);
	return buffer.str();
}

/*******************  FUNCTION  *********************/
bool FormulaMatcherResult::hasExponent ( void ) const
{
	return exponent != NULL;
}

};
