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
#include "CMRTexParsor.h"
#include "CMRLatexEntity2.h"
#include "CMRParsorBasics.h"
#include "CMRLatexFormula.h"
#include "CMRLatexParsorContext.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
CMRLatexEntity2::CMRLatexEntity2 ( void )
{
}

/*******************  FUNCTION  *********************/
CMRLatexEntity2::CMRLatexEntity2 ( const std::string& value )
{
	this->parse(value);
}

/*******************  FUNCTION  *********************/
CMRLatexEntity2::CMRLatexEntity2(const CMRLatexEntity2& orig)
{
	this->copy(orig);
}

/*******************  FUNCTION  *********************/
int CMRLatexEntity2::countExponents ( void ) const
{
	return exponents.size();
}

/*******************  FUNCTION  *********************/
int CMRLatexEntity2::countIndices ( void ) const
{
	return indices.size();
}

/*******************  FUNCTION  *********************/
int CMRLatexEntity2::countParameters ( void ) const
{
	return parameters.size();
}

/*******************  FUNCTION  *********************/
CMRLatexFormulas2* CMRLatexEntity2::getExponent ( int id )
{
	assert(id >= 0);
	if (id < 0 || id >= exponents.size())
		return NULL;
	else
		return exponents[id];
}

/*******************  FUNCTION  *********************/
CMRLatexFormulas2* CMRLatexEntity2::getIndice ( int id )
{
	assert(id >= 0);
	if (id < 0 || id >= indices.size())
		return NULL;
	else
		return indices[id];
}

/*******************  FUNCTION  *********************/
CMRLatexFormulas2* CMRLatexEntity2::getParameter ( int id )
{
	assert(id >= 0);
	if (id < 0 || id >= parameters.size())
		return NULL;
	else
		return parameters[id];
}

/*******************  FUNCTION  *********************/
const CMRLatexFormulas2* CMRLatexEntity2::getExponent ( int id ) const
{
	assert(id >= 0);
	if (id < 0 || id >= exponents.size())
		return NULL;
	else
		return exponents[id];
}

/*******************  FUNCTION  *********************/
const CMRLatexFormulas2* CMRLatexEntity2::getIndice ( int id ) const
{
	assert(id >= 0);
	if (id < 0 || id >= indices.size())
		return NULL;
	else
		return indices[id];
}

/*******************  FUNCTION  *********************/
const CMRLatexFormulas2* CMRLatexEntity2::getParameter ( int id ) const
{
	assert(id >= 0);
	if (id < 0 || id >= parameters.size())
		return NULL;
	else
		return parameters[id];
}

/*******************  FUNCTION  *********************/
const std::string& CMRLatexEntity2::getName ( void ) const
{
	return name;
}

/*******************  FUNCTION  *********************/
std::string CMRLatexEntity2::getString ( void ) const
{
	stringstream tmp;
	tmp << *this;
	return tmp.str();
}

/*******************  FUNCTION  *********************/
bool CMRLatexEntity2::needProtection ( const CMRLatexFormulasVector2& value )
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
void CMRLatexEntity2::writeFormulaList ( ostream& out, const CMRLatexFormulasVector2& value, const string& sep, const string& open, const string& close, bool forceOpenClose )
{
	//trivial
	if (value.empty())
		return;
	
	//open
	if (forceOpenClose || needProtection(value))
		out << open;
	
	//print elements
	for (CMRLatexFormulasVector2::const_iterator it = value.begin() ; it != value.end() ; ++it)
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
std::ostream& operator<< ( std::ostream& out, const CMRLatexEntity2& value )
{
	if (value.name.empty())
	{
		//in case of error
		out << "!!! UNDEFINED LATEX ENTITY !!!" << endl;
		//warning("Got undefined latex entity");
	} else if(value.name == "()") {
		CMRLatexEntity2::writeFormulaList(out,value.parameters,",","(",")",true);
	} else {
		//print the name
		out << value.name;
	}
		
	//indices and exponent
	if (value.indices.empty() == false)
	{
		out << "_";
		CMRLatexEntity2::writeFormulaList(out,value.indices);
	}
	if (value.exponents.empty() == false)
	{
		out << "^";
		CMRLatexEntity2::writeFormulaList(out,value.exponents);
	}
	
	//parameters
	if (value.name != "()" && value.name.empty() ==false)
		CMRLatexEntity2::writeFormulaList(out,value.parameters,"}{","{","}",true);

	return out;
}

/*******************  FUNCTION  *********************/
CMRLatexEntity2& CMRLatexEntity2::operator= ( const string& value )
{
	this->parse(value);
	return *this;
}

/*******************  FUNCTION  *********************/
void CMRLatexEntity2::parse ( const string& value ) throw(CMRLatexException)
{
	//trivial
	if (value.empty())
		return;

	//parse
	CMRLatexParsorContext context(value);
	this->parse(context);
	
	//check errors
	if (!context.isEnd())
		context.fatal("Expect a uniq entity, but get a composed one !");
}

/*******************  FUNCTION  *********************/
void CMRLatexEntity2::parse ( CMRLatexParsorContext& context )
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
void CMRLatexEntity2::parseSubGroup ( CMRLatexParsorContext& context )
{
	//errors
	assert(context.startBy('('));
	
	//setup name
	this->name = "()";
	
	//start to update current
	CMRLatexParsorContext subcontext = context.extractSubZone('(',')');
	CMRLatexFormulas2 * f = new CMRLatexFormulas2;
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
std::string CMRLatexEntity2::extractName ( CMRLatexParsorContext& context )
{
	//vars
	string res;

	//errors
	assert(context.isEnd() == false);
	
	//long names
	if (context.startBy('\\'))
	{
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
CMRLatexFormulas2* CMRLatexEntity2::extractSubFormula ( CMRLatexParsorContext& context )
{
	//vars
	CMRLatexFormulas2* f = new CMRLatexFormulas2;

	//move
	context.skipWhiteSpace();
	
	//extract
	if (context.startBy('{'))
	{
		CMRLatexParsorContext subcontext = context.extractSubZone('{','}');
		f->parse(subcontext);
	} else {
		CMRLatexEntity2 * tmp = new CMRLatexEntity2();
		tmp->parse(context);
		f->push_back(tmp);
	}
	
	//move
	context.skipWhiteSpace();
	
	return f;
}

/*******************  FUNCTION  *********************/
void CMRLatexEntity2::extractSubAndSuperScript ( CMRLatexParsorContext& context )
{
	char mode = context.getCurAndMove();
	
	//check
	assert(mode == '_' || mode == '^');
	
	//extract formula
	CMRLatexFormulas2 * f = extractSubFormula(context);
	
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
void CMRLatexEntity2::extractParameters ( CMRLatexParsorContext& context )
{
	//skip white spaces
	context.skipWhiteSpace();
	
	//errors
	if (context.isEnd())
		context.fatal("Missing function parameter.");
	
	//extract formula
	CMRLatexFormulas2 * f = extractSubFormula(context);
	
	//apply
	parameters.push_back(f);
	
	//skip white spaces
	context.skipWhiteSpace();
}

/*******************  FUNCTION  *********************/
void CMRLatexEntity2::parseStandard ( CMRLatexParsorContext& context )
{
	this->name = this->extractName(context);
	
	//extract sub and superscript
	if (context.startBy('_') || context.startBy('^'))
		extractSubAndSuperScript(context);
	if (context.startBy('_') || context.startBy('^'))
		extractSubAndSuperScript(context);
	
	//parameters
	int params = cmrRequireParameters(this->name,context);
	for (int i = 0 ; i < params ; i++)
		extractParameters(context);
}

/*******************  FUNCTION  *********************/
void CMRLatexEntity2::clear ( void )
{
	//cleanup memory
	for(CMRLatexFormulasVector2::iterator it = indices.begin();it!=indices.end();++it)
		delete *it;
	for(CMRLatexFormulasVector2::iterator it = exponents.begin();it!=exponents.end();++it)
		delete *it;
	for(CMRLatexFormulasVector2::iterator it = parameters.begin();it!=parameters.end();++it)
		delete *it;
	
	//local values
	this->indices.clear();
	this->exponents.clear();
	this->parameters.clear();
	this->name.clear();
}

/*******************  FUNCTION  *********************/
void CMRLatexEntity2::dumpAsXml ( ostream& out, int depth, const string& name, const CMRLatexFormulasVector2& list )
{
	if (list.empty() == false)
	{
		out << cmrIndent(depth) << "<" << name << ">" << endl;
		for (CMRLatexFormulasVector2::const_iterator it = list.begin() ; it != list.end() ; ++it)
			(*it)->dumpAsXml(out,depth+1);
		out << cmrIndent(depth) << "</" << name << ">" << endl;
	}
}

/*******************  FUNCTION  *********************/
void CMRLatexEntity2::dumpAsXml ( ostream& out, int depth ) const
{
	out << cmrIndent(depth) << "<entity>" << endl;
	out << cmrIndent(depth+1) << "<name>"<< name << "</name>" << endl;
	dumpAsXml(out,depth+1,"indices",indices);
	dumpAsXml(out,depth+1,"exponents",exponents);
	dumpAsXml(out,depth+1,"parameters",parameters);
	out << cmrIndent(depth) << "</entity>" << endl;
}

/*******************  FUNCTION  *********************/
void CMRLatexEntity2::dumpAsTree ( ostream& out, int depth, const string& name, const CMRLatexFormulasVector2& list )
{
	if (list.empty() == false)
	{
		if (list.size() == 1 && list[0]->isOnlyOneName())
		{
			out << cmrIndent(depth) << name << ": " << *list[0] << endl;
		} else {
			out << cmrIndent(depth) << name << ":" << endl;
			for (CMRLatexFormulasVector2::const_iterator it = list.begin() ; it != list.end() ; ++it)
				(*it)->dumpAsTree(out,depth+1);
		}
	}
}

/*******************  FUNCTION  *********************/
void CMRLatexEntity2::dumpAsTree ( ostream& out, int depth ) const
{
	out << cmrIndent(depth) << "entity: " << name << endl;
	dumpAsTree(out,depth+1,"indices",indices);
	dumpAsTree(out,depth+1,"exponents",exponents);
	dumpAsTree(out,depth+1,"parameters",parameters);
}

/*******************  FUNCTION  *********************/
CMRLatexEntity2::~CMRLatexEntity2 ( void )
{
	clear();
}

/*******************  FUNCTION  *********************/
bool CMRLatexEntity2::isOnlyOneName ( void ) const
{
	return (name.empty() == false && indices.empty() && parameters.empty() && exponents.empty());
}

/*******************  FUNCTION  *********************/
CMRLatexFormulasVector2& CMRLatexEntity2::getExponents ( void )
{
	return exponents;
}

/*******************  FUNCTION  *********************/
CMRLatexFormulasVector2& CMRLatexEntity2::getIndices ( void )
{
	return indices;
}

/*******************  FUNCTION  *********************/
CMRLatexFormulasVector2& CMRLatexEntity2::getParameters ( void )
{
	return parameters;
}

/*******************  FUNCTION  *********************/
void CMRLatexEntity2::setExtraInfo(const string& key, void* value, bool allowOverride)
{
	//errors
	if (allowOverride == false && hasInfo(key))
		throw CMRLatexException("Invalid override of information key on LatexEntity.");
	
	extraInfos[key] = value;
}

/*******************  FUNCTION  *********************/
void CMRLatexEntity2::deleteInfo(const string& key, bool throwOnError)
{
	bool status = hasInfo(key);

	//errors
	if (throwOnError && status == false)
		throw CMRLatexException("Invalid delete of information key on LatexEntity.");
	else if (status == false)
		return;
	
	extraInfos.erase(key);
}

/*******************  FUNCTION  *********************/
void* CMRLatexEntity2::getExtraInfo(const string& key, bool throwOnError)
{
	bool status = hasInfo(key);

	//errors
	if (throwOnError && status == false)
		throw CMRLatexException("Invalid read of information key on LatexEntity.");
	else if (status == false)
		return NULL;
	
	return extraInfos[key];
}

/*******************  FUNCTION  *********************/
bool CMRLatexEntity2::hasInfo(const string& key) const
{
	return extraInfos.find(key) != extraInfos.end();
}

/*******************  FUNCTION  *********************/
CMRLatexEntity2& CMRLatexEntity2::operator=(const CMRLatexEntity2& value)
{
	this->copy(value);
	return *this;
}

/*******************  FUNCTION  *********************/
void CMRLatexEntity2::copy(const CMRLatexEntity2& value)
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
void CMRLatexEntity2::copy(CMRLatexFormulasVector2& vectorOut, const CMRLatexFormulasVector2& vectorOrig)
{
	//clear vector
	vectorOut.clear();
	
	//copy all chilsd
	for (int i = 0 ; i < vectorOrig.size() ; i++)
		vectorOut.push_back(new CMRLatexFormulas2(*vectorOrig[i]));
}
