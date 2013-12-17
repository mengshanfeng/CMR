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
#include <cstdlib>
#include "TexParsor.h"
#include "LatexEntity.h"
#include "ParsorBasics.h"
#include "LatexFormula.h"
#include "LatexParsorContext.h"

/**********************  USING  *********************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
LatexEntity::LatexEntity ( void )
{
}

/*******************  FUNCTION  *********************/
LatexEntity::LatexEntity ( const std::string& value )
{
	this->parse(value);
}

/*******************  FUNCTION  *********************/
LatexEntity::LatexEntity(const LatexEntity& orig)
{
	this->copy(orig);
}

/*******************  FUNCTION  *********************/
int LatexEntity::countExponents ( void ) const
{
	return exponents.size();
}

/*******************  FUNCTION  *********************/
int LatexEntity::countIndices ( void ) const
{
	return indices.size();
}

/*******************  FUNCTION  *********************/
int LatexEntity::countParameters ( void ) const
{
	return parameters.size();
}

/*******************  FUNCTION  *********************/
LatexFormulas* LatexEntity::getExponent ( int id )
{
	assert(id >= 0);
	if (id < 0 || id >= exponents.size())
		return NULL;
	else
		return exponents[id];
}

/*******************  FUNCTION  *********************/
LatexFormulas* LatexEntity::getIndice ( int id )
{
	assert(id >= 0);
	if (id < 0 || id >= indices.size())
		return NULL;
	else
		return indices[id];
}

/*******************  FUNCTION  *********************/
LatexFormulas* LatexEntity::getParameter ( int id )
{
	assert(id >= 0);
	if (id < 0 || id >= parameters.size())
		return NULL;
	else
		return parameters[id];
}

/*******************  FUNCTION  *********************/
const LatexFormulas* LatexEntity::getExponent ( int id ) const
{
	assert(id >= 0);
	if (id < 0 || id >= exponents.size())
		return NULL;
	else
		return exponents[id];
}

/*******************  FUNCTION  *********************/
const LatexFormulas* LatexEntity::getIndice ( int id ) const
{
	assert(id >= 0);
	if (id < 0 || id >= indices.size())
		return NULL;
	else
		return indices[id];
}

/*******************  FUNCTION  *********************/
const LatexFormulas* LatexEntity::getParameter ( int id ) const
{
	assert(id >= 0);
	if (id < 0 || id >= parameters.size())
		return NULL;
	else
		return parameters[id];
}

/*******************  FUNCTION  *********************/
const std::string& LatexEntity::getName ( void ) const
{
	return name;
}

/*******************  FUNCTION  *********************/
std::string LatexEntity::getString ( void ) const
{
	stringstream tmp;
	tmp << *this;
	return tmp.str();
}

/*******************  FUNCTION  *********************/
bool LatexEntity::needProtection ( const LatexFormulasVector& value )
{
	//error
	assert(value.empty() == false);
	
	//cases
	if (value.size() > 1)
		return true;
	else if (value[0]->size() > 1)
		return true;
	else
		return false;
}

/*******************  FUNCTION  *********************/
void LatexEntity::writeFormulaList ( ostream& out, const LatexFormulasVector& value, const string& sep, const string& open, const string& close, bool forceOpenClose )
{
	//trivial
	if (value.empty())
		return;
	
	//open
	if (forceOpenClose || needProtection(value))
		out << open;
	
	//print elements
	for (LatexFormulasVector::const_iterator it = value.begin() ; it != value.end() ; ++it)
	{
		if (it != value.begin())
			out << sep;
		out << **it;
	}

	//close
	if (forceOpenClose || needProtection(value))
		out << close;
}

/*******************  FUNCTION  *********************/
std::ostream& operator<< ( std::ostream& out, const LatexEntity& value )
{
	if (value.name.empty())
	{
		//in case of error
		out << "!!! UNDEFINED LATEX ENTITY !!!" << endl;
		//warning("Got undefined latex entity");
	} else if(value.name == "()") {
		LatexEntity::writeFormulaList(out,value.parameters,",","(",")",true);
	} else {
		//print the name
		out << value.name;
	}
		
	//indices and exponent
	if (value.indices.empty() == false)
	{
		out << "_";
		LatexEntity::writeFormulaList(out,value.indices);
	}
	if (value.exponents.empty() == false)
	{
		out << "^";
		LatexEntity::writeFormulaList(out,value.exponents);
	}
	
	//parameters
	if (value.name != "()" && value.name.empty() ==false)
		LatexEntity::writeFormulaList(out,value.parameters,"}{","{","}",true);

	return out;
}

/*******************  FUNCTION  *********************/
LatexEntity& LatexEntity::operator= ( const string& value )
{
	this->parse(value);
	return *this;
}

/*******************  FUNCTION  *********************/
void LatexEntity::parse ( const string& value ) throw(LatexException)
{
	//trivial
	if (value.empty())
		return;

	//parse
	LatexParsorContext context(value);
	this->parse(context);
	
	//check errors
	if (!context.isEnd())
		context.fatal("Expect a uniq entity, but get a composed one !");
}

/*******************  FUNCTION  *********************/
void LatexEntity::parse ( LatexParsorContext& context )
{
	//clear current
	this->clear();
	
	//skip white spaces
	context.skipWhiteSpace();
	
	//check type
	if (context.startBy('('))
		this->parseSubGroup(context);
	else
		this->parseStandard(context);
	
	//skip white spaces
	context.skipWhiteSpace();
}

/*******************  FUNCTION  *********************/
void LatexEntity::parseSubGroup ( LatexParsorContext& context )
{
	//errors
	assert(context.startBy('('));
	
	//setup name
	this->name = "()";
	
	//start to update current
	LatexParsorContext subcontext = context.extractSubZone('(',')');
	LatexFormulas * f = new LatexFormulas;
	f->parse(subcontext);
	this->parameters.push_back(f);
	
	
	//extract sub and superscript
	//extract sub and superscript
	if (context.startBy('_') || context.startBy('^'))
		extractSubAndSuperScript(context);
	if (context.startBy('_') || context.startBy('^'))
		extractSubAndSuperScript(context);
}

/*******************  FUNCTION  *********************/
std::string LatexEntity::extractName ( LatexParsorContext& context )
{
	//vars
	string res;

	//errors
	assert(context.isEnd() == false);
	
	//long names
	if (context.startBy("\\_"))
	{
		res += "_";
		context.move(2);
	} else if (context.startBy('\\')) {
		res+=context.getCurAndMove();
		while(cmrIsAlphaNum(context.getCurrent()))
			res+=context.getCurAndMove();
	} else if (cmrIsNum(context.getCurrent())) {
		while(cmrIsNumFloat(context.getCurrent()))
			res+=context.getCurAndMove();
	} else if (cmrIsAlphaNum(context.getCurrent())){
		res = context.getCurAndMove();
		while(context.getCurrent() == '\'')
			res+=context.getCurAndMove();
	} else if (cmrIsOperator(context.getCurrent())) {
		res = context.getCurAndMove();
	} else {
		context.fatal("Invalid character while searching name, must be [a-zA-Z0-9.] or [+-=*/\\,].");
	}
	
	return res;
}

/*******************  FUNCTION  *********************/
LatexFormulas* LatexEntity::extractSubFormula ( LatexParsorContext& context )
{
	//vars
	LatexFormulas* f = new LatexFormulas;

	//move
	context.skipWhiteSpace();
	
	//extract
	if (context.startBy('{'))
	{
		LatexParsorContext subcontext = context.extractSubZone('{','}');
		f->parse(subcontext);
	} else {
		LatexEntity * tmp = new LatexEntity();
		tmp->parse(context);
		f->push_back(tmp);
	}
	
	//move
	context.skipWhiteSpace();
	
	return f;
}

/*******************  FUNCTION  *********************/
void LatexEntity::extractSubAndSuperScript ( LatexParsorContext& context )
{
	char mode = context.getCurAndMove();
	
	//check
	assert(mode == '_' || mode == '^');
	
	//extract formula
	LatexFormulas * f = extractSubFormula(context);
	
	//split on ',' and merge in local sub lists
	if (mode == '_')
		f->split(indices,",");
	else if (mode == '^')
		f->split(exponents,",");
	else
		abort();
	
	//clear tmp formula
	delete f;
}

/*******************  FUNCTION  *********************/
void LatexEntity::extractParameters ( LatexParsorContext& context )
{
	//skip white spaces
	context.skipWhiteSpace();
	
	//errors
	if (context.isEnd())
		context.fatal("Missing function parameter.");
	
	//extract formula
	LatexFormulas * f = extractSubFormula(context);
	
	//apply
	parameters.push_back(f);
	
	//skip white spaces
	context.skipWhiteSpace();
}

/*******************  FUNCTION  *********************/
void LatexEntity::extractMathRmParameter ( LatexParsorContext& context )
{
	//errors
	assert(this->name == "\\mathrm");

	//remove padding
	context.skipWhiteSpace();
	
	//search {
	if (context.startBy("{") == false)
		context.fatal("\\mathrm must be follow by value in {}");

	//move
	context.move(1);
	
	//eat until end
	string tmp;
	char prev='\0';
	while (!context.startBy("}") || prev == '\\')
	{
		prev = context.getCurAndMove(1);
		tmp += prev;
	}
	
	//check end and move
	if (context.startBy("}"))
		context.move(1);
	else
		context.fatal("Missing closure of \\mathrm{}");
	
	//create the special formula
	LatexFormulas * f = new LatexFormulas();
	LatexEntity * e = new LatexEntity();
	e->name = tmp;
	f->push_back(e);

	parameters.push_back(f);
}

/*******************  FUNCTION  *********************/
void LatexEntity::parseStandard ( LatexParsorContext& context )
{
	this->name = this->extractName(context);
	
	//extract sub and superscript
	if (context.startBy('_') || context.startBy('^'))
		extractSubAndSuperScript(context);
	if (context.startBy('_') || context.startBy('^'))
		extractSubAndSuperScript(context);
	
	//check for mathrm
	if (this->name == "\\mathrm")
	{
		extractMathRmParameter(context);
	} else {
		//parameters
		int params = cmrRequireParameters(this->name,context);
		for (int i = 0 ; i < params ; i++)
			extractParameters(context);
	}
}

/*******************  FUNCTION  *********************/
void LatexEntity::clear ( void )
{
	//cleanup memory
	for(LatexFormulasVector::iterator it = indices.begin();it!=indices.end();++it)
		delete *it;
	for(LatexFormulasVector::iterator it = exponents.begin();it!=exponents.end();++it)
		delete *it;
	for(LatexFormulasVector::iterator it = parameters.begin();it!=parameters.end();++it)
		delete *it;
	
	//local values
	this->indices.clear();
	this->exponents.clear();
	this->parameters.clear();
	this->name.clear();
	this->extraInfos.clear();
}

/*******************  FUNCTION  *********************/
void LatexEntity::dumpAsXml ( ostream& out, int depth, const string& name, const LatexFormulasVector& list )
{
	if (list.empty() == false)
	{
		out << cmrIndent(depth) << "<" << name << ">" << endl;
		for (LatexFormulasVector::const_iterator it = list.begin() ; it != list.end() ; ++it)
			(*it)->dumpAsXml(out,depth+1);
		out << cmrIndent(depth) << "</" << name << ">" << endl;
	}
}

/*******************  FUNCTION  *********************/
void LatexEntity::dumpAsXml ( ostream& out, int depth ) const
{
	out << cmrIndent(depth) << "<entity>" << endl;
	out << cmrIndent(depth+1) << "<name>"<< name << "</name>" << endl;
	dumpAsXml(out,depth+1,"indices",indices);
	dumpAsXml(out,depth+1,"exponents",exponents);
	dumpAsXml(out,depth+1,"parameters",parameters);
	out << cmrIndent(depth) << "</entity>" << endl;
}

/*******************  FUNCTION  *********************/
void LatexEntity::dumpAsTree ( ostream& out, int depth, const string& name, const LatexFormulasVector& list )
{
	if (list.empty() == false)
	{
		if (list.size() == 1 && list[0]->isOnlyOneName())
		{
			out << cmrIndent(depth) << name << ": " << *list[0] << endl;
		} else {
			out << cmrIndent(depth) << name << ":" << endl;
			for (LatexFormulasVector::const_iterator it = list.begin() ; it != list.end() ; ++it)
				(*it)->dumpAsTree(out,depth+1);
		}
	}
}

/*******************  FUNCTION  *********************/
void LatexEntity::dumpAsTree ( ostream& out, int depth ) const
{
	out << cmrIndent(depth) << "entity: " << name << endl;
	dumpAsTree(out,depth+1,"indices",indices);
	dumpAsTree(out,depth+1,"exponents",exponents);
	dumpAsTree(out,depth+1,"parameters",parameters);
}

/*******************  FUNCTION  *********************/
LatexEntity::~LatexEntity ( void )
{
	clear();
}

/*******************  FUNCTION  *********************/
bool LatexEntity::isOnlyOneName ( void ) const
{
	return (name.empty() == false && indices.empty() && parameters.empty() && exponents.empty());
}

/*******************  FUNCTION  *********************/
LatexFormulasVector& LatexEntity::getExponents ( void )
{
	return exponents;
}

/*******************  FUNCTION  *********************/
LatexFormulasVector& LatexEntity::getIndices ( void )
{
	return indices;
}

/*******************  FUNCTION  *********************/
LatexFormulasVector& LatexEntity::getParameters ( void )
{
	return parameters;
}

/*******************  FUNCTION  *********************/
const LatexFormulasVector& LatexEntity::getExponents ( void ) const
{
	return exponents;
}

/*******************  FUNCTION  *********************/
const LatexFormulasVector& LatexEntity::getIndices ( void ) const
{
	return indices;
}

/*******************  FUNCTION  *********************/
const LatexFormulasVector& LatexEntity::getParameters ( void ) const
{
	return parameters;
}

/*******************  FUNCTION  *********************/
void LatexEntity::setExtraInfo(const string& key, void* value, bool allowOverride)
{
	//errors
	if (allowOverride == false && hasInfo(key))
		throw LatexException("Invalid override of information key on LatexEntity.");
	
	extraInfos[key] = value;
}

/*******************  FUNCTION  *********************/
void LatexEntity::deleteInfo(const string& key, bool throwOnError)
{
	bool status = hasInfo(key);

	//errors
	if (throwOnError && status == false)
		throw LatexException("Invalid delete of information key on LatexEntity.");
	else if (status == false)
		return;
	
	extraInfos.erase(key);
}

/*******************  FUNCTION  *********************/
void* LatexEntity::getExtraInfo( const string& key, bool throwOnError )
{
	bool status = hasInfo(key);

	//errors
	if (throwOnError && status == false)
		throw LatexException("Invalid read of information key on LatexEntity.");
	else if (status == false)
		return NULL;
	
	return extraInfos[key];
}

/*******************  FUNCTION  *********************/
const void* LatexEntity::getExtraInfo( const string& key, bool throwOnError ) const
{
	bool status = hasInfo(key);

	//errors
	if (throwOnError && status == false)
		throw LatexException("Invalid read of information key on LatexEntity.");
	else if (status == false)
		return NULL;
	
	return extraInfos.at(key);
}

/*******************  FUNCTION  *********************/
bool LatexEntity::hasInfo(const string& key) const
{
	return extraInfos.find(key) != extraInfos.end();
}

/*******************  FUNCTION  *********************/
LatexEntity& LatexEntity::operator=(const LatexEntity& value)
{
	this->copy(value);
	return *this;
}

/*******************  FUNCTION  *********************/
void LatexEntity::copy(const LatexEntity& value)
{
	//clea childs
	this->clear();

	//copy name
	this->name = value.name;
	
	//copy indices
	copy(indices,value.indices);
	copy(exponents,value.exponents);
	copy(parameters,value.parameters);
	
	//copy extainfo
	extraInfos.clear();
	extraInfos = value.extraInfos;
}

/*******************  FUNCTION  *********************/
void LatexEntity::copy(LatexFormulasVector& vectorOut, const LatexFormulasVector& vectorOrig)
{
	//clear vector
	vectorOut.clear();
	
	//copy all chilsd
	for (int i = 0 ; i < vectorOrig.size() ; i++)
		vectorOut.push_back(new LatexFormulas(*vectorOrig[i]));
}

/*******************  FUNCTION  *********************/
LatexEntity::operator LatexFormulas() const
{
	LatexFormulas f;
	f.push_back(new LatexEntity(this->getString()));
	return f;
}

/*******************  FUNCTION  *********************/
void LatexEntity::addExponent ( const string& value )
{
	LatexFormulas * f = new LatexFormulas(value);
	this->exponents.push_back(f);
}

/*******************  FUNCTION  *********************/
LatexFormulasVector& LatexEntity::getChilds ( LatexEntityChilds mode )
{
	switch(mode)
	{
		case LATEX_EXPONENTS:
			return exponents;
		case LATEX_INDICES:
			return indices;
		case LATEX_PARAMETERS:
			return parameters;
	}
}

/*******************  FUNCTION  *********************/
const LatexFormulasVector& LatexEntity::getChilds ( LatexEntityChilds mode ) const
{
	switch(mode)
	{
		case LATEX_EXPONENTS:
			return exponents;
		case LATEX_INDICES:
			return indices;
		case LATEX_PARAMETERS:
			return parameters;
	}
}

}
