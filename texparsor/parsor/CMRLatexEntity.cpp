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
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include "CMRLatexEntity.h"
#include "CMRTexParsor.h"
#include "CMRParsorBasics.h"

using namespace std;

/*******************  FUNCTION  *********************/
CMRLatexEntity::CMRLatexEntity ( void )
{
	this->from = 0;
	this->to = 0;
}

/*******************  FUNCTION  *********************/
CMRLatexEntity::~CMRLatexEntity ( void )
{
// 	for(CMRLatexEntityVector::iterator it = subscript.childs.begin();it!=subscript.childs.end();++it)
// 		delete *it;
// 	for(CMRLatexEntityVector::iterator it = superscript.childs.begin();it!=superscript.childs.end();++it)
// 		delete *it;
// 	for(CMRLatexFormulasList::iterator it = params.begin();it!=params.end();++it)
// 		delete *it;
}

/*******************  FUNCTION  *********************/
void cmrPrintFormula(const CMRLatexFormulasList & formulas,int depth)
{
	for(CMRLatexFormulasList::const_iterator it = formulas.begin();it!=formulas.end();++it)
	{
		if (it != formulas.begin())
			printf(",");
		cmrPrintFormula(**it,depth);
	}
}

/*******************  FUNCTION  *********************/
void cmrPrintFormula(const CMRLatexFormulas & formula,int depth)
{
	int pos = 0;
	for(CMRLatexEntityVector::const_iterator it = formula.childs.begin();it!=formula.childs.end();++it)
		(*it)->print(depth,pos++);
}

/*******************  FUNCTION  *********************/
void CMRLatexEntity::print ( int depth, int pos )
{
	printf(" - Entity [depth = %d, pos = %d]\n",depth,pos);
	printf("       * name        : %s\n",name.c_str());
	printf("       * totalValue  : %s\n",totalValue.c_str());
	printf("       * pos         : [%d - %d]\n",from,to);
	printf("       * subscript   : %s\n",subscriptTotalValue.c_str());
	printf("       * superscript : %s\n",superscriptTotalValue.c_str());
	printf("       * args        : %s\n",argsTotalValue.c_str());
	printf("       * parent      : %s\n",parent.c_str());
	cmrPrintFormula(subscript,depth+1);
	cmrPrintFormula(superscript,depth+1);
	for(CMRLatexFormulasList::const_iterator it = params.begin();it!=params.end();++it)
		cmrPrintFormula(**it,depth+1);
}

/*******************  FUNCTION  *********************/
std::string CMRLatexEntity::getString ( void ) const
{
	return parent.substr(from,to-from);
}

/*******************  FUNCTION  *********************/
int CMRLatexEntity::countIndices ( void ) const
{
	return subscript.size();
}

/*******************  FUNCTION  *********************/
CMRLatexFormulasList CMRLatexEntity::getIndices ( void )
{
	return subscript;
}












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
void CMRLatexEntity2::writeFormulaList ( ostream& out, const CMRLatexFormulasVector2& value, const string& sep, const string& open, const string& close, bool forceOpenClose )
{
	//trivial
	if (value.empty())
		return;
	
	//open
	if (forceOpenClose || value.size() > 1)
		out << open;
	
	//print elements
	for (CMRLatexFormulasVector2::const_iterator it = value.begin() ; it != value.end() ; ++it)
	{
		if (it != value.begin())
			out << sep;
		out << **it;
	}

	//close
	if (forceOpenClose || value.size() > 1)
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
		
		//parameters
		CMRLatexEntity2::writeFormulaList(out,value.parameters,"}{","{","}",true);
		
		//indices and exponent
		if (value.exponents.empty() == false)
		{
			out << "^";
			CMRLatexEntity2::writeFormulaList(out,value.exponents);
		}
		if (value.indices.empty() == false)
		{
			out << "_";
			CMRLatexEntity2::writeFormulaList(out,value.indices);
		}
	}
	return out;
}

/*******************  FUNCTION  *********************/
CMRLatexEntity2& CMRLatexEntity2::operator= ( const string& value )
{
	this->parse(value);
	return *this;
}

/*******************  FUNCTION  *********************/
void CMRLatexEntity2::parse ( const string& value )
{
	//trivial
	if (value.empty())
		return;

	//parse
	CMRLatexParsorContext context(value);
	this->parse(context);
	
	//check errors
	assert(context.isEnd());
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
	//if (context.startBy('_') || context.startBy('^'))
	//	extractSubAndSuperScript(context);
	if (context.startBy('^'))
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
	this->indices.clear();
	this->exponents.clear();
	this->parameters.clear();
	this->name.clear();
}






/*******************  FUNCTION  *********************/
CMRLatexFormulas2::CMRLatexFormulas2 ( void )
{

}

/*******************  FUNCTION  *********************/
CMRLatexFormulas2::CMRLatexFormulas2 ( const string& value )
{
	this->parse(value);
}

/*******************  FUNCTION  *********************/
void CMRLatexFormulas2::parse ( const string& value )
{
	//vars
	CMRLatexParsorContext context(value);
	
	//do it
	parse(context);
}

/*******************  FUNCTION  *********************/
void CMRLatexFormulas2::parse ( CMRLatexParsorContext& context )
{
	//reset content
	clear();

	//loop on all
	while (!context.isEnd())
	{
		CMRLatexEntity2 * e = new CMRLatexEntity2;
		e->parse(context);
		this->push_back(e);
	}
}

/*******************  FUNCTION  *********************/
string CMRLatexFormulas2::getString ( void ) const
{
	stringstream tmp;
	tmp << *this;
	return tmp.str();
}

/*******************  FUNCTION  *********************/
ostream& operator<< ( ostream& out, const CMRLatexFormulas2& value )
{
	if (value.empty())
	{
		//in case of error
		out << "!!! EMPTY LATEX FORMULA !!!" << endl;
		//warning("Got undefined latex entity");
	} else {
		for (CMRLatexFormulas2::const_iterator it = value.begin() ; it != value.end() ; ++it)
		{
			assert(*it != NULL);
			out << **it;
		}
	}

	return out;
}









/*******************  FUNCTION  *********************/
CMRLatexParsorContext::CMRLatexParsorContext ( const string& value )
{
	this->value = value;
	this->position = 0;
	this->parent = NULL;
	this->relToParent = 0;
}

/*******************  FUNCTION  *********************/
CMRLatexParsorContext::CMRLatexParsorContext ( const CMRLatexParsorContext* parent, int start, int end )
{
	this->parent = parent;
	this->value = parent->value.substr(start,end-start);
	this->position = 0;
	this->relToParent = start;
}

/*******************  FUNCTION  *********************/
char CMRLatexParsorContext::getCurrent ( void ) const
{
	//assert(this->position < value.size());
	if (this->position < value.size())
		return this->value[this->position];
	else
		return '\0';
}

/*******************  FUNCTION  *********************/
int CMRLatexParsorContext::getPosition ( void ) const
{
	return this->position;
}

/*******************  FUNCTION  *********************/
bool CMRLatexParsorContext::isEnd ( void ) const
{
	return this->position >= value.size();
}

/*******************  FUNCTION  *********************/
int CMRLatexParsorContext::move ( int delta )
{
	this->position += delta;
	assert(this->position >= 0);
	return this->position;
}

/*******************  FUNCTION  *********************/
CMRLatexParsorContext& CMRLatexParsorContext::operator++ ( void )
{
	this->move(1);
	return *this;
}

/*******************  FUNCTION  *********************/
void CMRLatexParsorContext::skipWhiteSpace ( void )
{
	while (cmrIsWhiteSpace(this->value[this->position]))
		this->position++;
}

/*******************  FUNCTION  *********************/
bool CMRLatexParsorContext::startBy ( const string& value ) const
{
	if (value.size() + this->position > this->value.size())
		return false;
	else
		return this->value.substr(0,value.size()) == value;
}

/*******************  FUNCTION  *********************/
bool CMRLatexParsorContext::startBy ( char value ) const
{
	if (this->position < this->value.size())
		return this->value[this->position] == value;
	else
		return false;
}

/*******************  FUNCTION  *********************/
CMRLatexParsorContext CMRLatexParsorContext::extractSubZone ( char delimOpen, char delimClose )
{
	//vars
	int start = position + 1;
	int cur = position;
	int cnt = 0;

	//errors
	assert(isEnd() == false);
	assert(value[position] == delimOpen);
	
	//search close
	do {
		//check delim
		if (value[cur] == delimOpen)
			cnt++;
		else if (value[cur] == delimClose)
			cnt--;
		
		//move
		if (cnt > 0)
			cur++;
		
		//error
		if (cur > value.size())
			this->fatal("Missing close.");
	} while (cnt > 0);
	
	//return
	this->position = cur+1;
	return CMRLatexParsorContext(this,start,cur);
}

/*******************  FUNCTION  *********************/
void CMRLatexParsorContext::fatal ( const string& message ) const
{
	//vars
	const CMRLatexParsorContext * cur = this;
	int curPos = position;
	
	//errors
	cerr << "LATEX PARSING ERROR : " << message << endl;
	
	//display position
	while (cur != NULL)
	{
		//print local info
		cerr << cur->value << endl;
		for (int i = 0 ; i < curPos ; i++)
			cerr << ' ';
		cerr << '^' << endl;
		
		//move to parent
		curPos += cur->relToParent;
		cur = parent;
	}
	
	//finish by exit
	abort();
}

/*******************  FUNCTION  *********************/
char CMRLatexParsorContext::getCurAndMove ( int delta )
{
	char tmp = '\0';
	
	assert(this->position >= 0);
	
	if (position < value.size())
		tmp = value[position++];

	return tmp;
}

/*******************  FUNCTION  *********************/
void CMRLatexFormulas2::split ( CMRLatexFormulasVector2& formulas, const string& sep )
{
	int pos = 0;
	CMRLatexFormulas2 * f = new CMRLatexFormulas2;
	
	//move until first one
// 	while (pos < size())
// 	{
// 		if ((*this)[pos]->getString() != sep)
// 			pos++;
// 		else
// 			break;
// 	}
	
	//cut
	for (int i = pos ; i < this->size() ; i++)
	{
		if ((*this)[i]->getString() == sep)
		{
			formulas.push_back(f);
			f = new CMRLatexFormulas2;
			//delete (*this)[pos];
		} else {
			f->push_back((*this)[i]);
		}
	}
	
	//finish
	formulas.push_back(f);
	
	//resize if need
// 	if (f != this)
// 		this->resize(pos);
}
