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
#include "ParsorBasics.h"
#include "LatexParsorContext.h"
#include "TexParsor.h"

/**********************  USING  *********************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
void cmrParsorError(const string & value,int pos,const string & message)
{
	if ( ! message.empty() )
		fprintf(stderr,"PARSING ERROR : %s\n",message.c_str());
	else
		fprintf(stderr,"PARGINS ERROR : \n");
	fprintf(stderr,"%s\n",value.c_str());
	for (int i = 0 ; i < value.size(); i++)
		fprintf(stderr,"%c",((i==pos)?'^':' '));
	fprintf(stderr,"\n");
	abort();
	
}

/*******************  FUNCTION  *********************/
string cmrExtractName(const string & value,int & start)
{
	//vars
	string res;

	//errors
	assert(start >= 0);
	assert(start < value.size());
	
	//long names
	if (value[start] == '\\')
	{
		res+=value[start++];
		#warning cleanup this way to do escape
		while(cmrIsAlphaNum(value[start]) || (value[start] == '\\' && value[start] == '_'))
		{
			if (value[start] == '\\')
				start++;
			res+=value[start++];
		}
	} else if (cmrIsNum(value[start])) {
		while(cmrIsNumFloat(value[start]))
			res+=value[start++];
	} else if (cmrIsAlphaNum(value[start])){
		res = value[start++];
		while(value[start] == '\'')
			res+=value[start++];
	} else if (cmrIsOperator(value[start])) {
		res = value[start++];
	} else {
		cmrParsorError(value,start,"Invalid character while searching name, must be [a-zA-Z0-9.] or [+-=*/\\,].");
	}
	
	return res;
}

/*******************  FUNCTION  *********************/
string cmrExtractSubZone(const string & value, int & start,char open,char close)
{
	//vars
	string res;
	int cnt = 1;
	int curStart = start;
	
	//errors
	assert(start >= 0);
	assert(start < value.size());

	if (value[start] == open)
	{
		start++;
		while(cnt > 0)
		{
			if (value[start] == open)
				cnt++;
			else if (value[start] == close)
				cnt --;
			if (cnt > 0 && !cmrIsWhiteSpace(value[start]))
				res += value[start++];
			else
				start++;
			if (start > value.size())
				cmrParsorError(value,curStart,"Missing close.");
		}
	} else {
		res = cmrExtractName(value,start);
	}
	
	return res;
}

/*******************  FUNCTION  *********************/
void cmrExtractSubAndSuperScript(const string & value,int & start,LatexEntityOld * entity)
{
	//vars
	string res;
	char mode = value[start];
	
	//errors
	assert(start >= 0);
	assert(start < value.size());
	assert(entity != NULL);
	
	//check it
	if (mode == '_')
	{
		start++;
		res = cmrExtractSubZone(value,start);
		entity->subscriptTotalValue = res;
		LatexFormulasOld * f = new LatexFormulasOld;
		cmrParseLatexFormula(*f,res);
		entity->subscript.push_back(f);
	} else if (mode == '^') {
		start++;
		res = cmrExtractSubZone(value,start);
		entity->superscriptTotalValue = res;
		LatexFormulasOld * f = new LatexFormulasOld;
		cmrParseLatexFormula(*f,res);
		entity->superscript.push_back(f);
	} else {
		assert(false);
	}
}

/*******************  FUNCTION  *********************/
int cmrRequireParameters(const string & name,const string & value,int pos)
{
	if (name[0] != '\\')
	{
		return 0;
	} else if (name == "\\frac") {
		return 2;
	} else if (name == "\\sum") {
		return 1;
	} else if (name == "\\CMRTMP") {
		return 1;
	} else if (name == "\\cmr"){
		return 2;
	} else if (name == "\\cmrsubaction") {
		return 1;
	} else if (name == "\\sqrt") {
		return 1;
	} else {
		cmrParsorError(value,pos,"Invalid latex command.");
		return -1;
	}
}

/*******************  FUNCTION  *********************/
int cmrRequireParameters(const string & name,LatexParsorContext & context)
{
	if (name[0] != '\\')
	{
		return 0;
	} else if (name == "\\frac") {
		return 2;
	} else if (name == "\\sum") {
		return 1;
	} else if (name == "\\CMRTMP") {
		return 1;
	} else if (name == "\\cmr"){
		return 2;
	} else if (name == "\\cmrsubaction") {
		return 1;
	} else if (name == "\\sqrt") {
		return 1;
	} else if (name == "\\gamma") {
		return 0;
	} else if (name == "\\rho") {
		return 0;
	} else if (name == "\\overline") {
		return 1;
	} else if (name == "\\Delta") {
		return 0;
	} else if (name == "\\varepsilon") {
		return 0;
	} else if (name == "\\partial") {
		return 0;
	} else if (name == "\\cmropexp") {
		return 0;
	} else {
		context.fatal("Invalid latex command.");
		return -1;
	}
}

/*******************  FUNCTION  *********************/
void cmrReplaceByCommaGroup(LatexFormulasListOld & formula)
{
	//vars
	bool hasComma = false;
	LatexEntityOld * entity = NULL;
	LatexFormulasOld * current;
	
	if (formula.empty())
		return;
	
	current = formula[0];

	for (LatexEntityVectorOld::const_iterator it = current->childs.begin() ; it != current->childs.end() ; ++it)
		if ((*it)->name == ",")
			hasComma = true;
		
	//nothing to do
	if (hasComma == false)
		return;
	
	//create entity
	entity = new LatexEntityOld;
	entity->name = "\\COMMA_GROUP";
	entity->totalValue = (*current->childs.begin())->parent;
	
	//create formulas
	LatexFormulasOld * f = new LatexFormulasOld;

	//fill
	formula.clear();
	for (LatexEntityVectorOld::const_iterator it = current->childs.begin() ; it != current->childs.end() ; ++it)
	{
		if ((*it)->name == ",")
		{
			formula.push_back(f);
			f = new LatexFormulasOld;
		} else {
			f->childs.push_back(*it);
			f->string += (*it)->getString();
		}
	}
	formula.push_back(f);
	delete current;
}

/*******************  FUNCTION  *********************/
LatexEntityOld * cmrParseLatexEntitySimple(const string & value,int & start)
{
	//vars
	int cur = start;
	LatexEntityOld * res = NULL;
	int params = 0;

	//errors
	assert(start >= 0);

	//trivial
	if (start >= value.size())
		return NULL;
	
	//setup entity
	res = new LatexEntityOld;
	//extract name
	res->parent = value;
	res->from = start;
	res->name = cmrExtractName(value,cur);
	//extract sub and superscript
	if (value[cur] == '_' || value[cur] == '^')
		cmrExtractSubAndSuperScript(value,cur,res);
	if (value[cur] == '_' || value[cur] == '^')
		cmrExtractSubAndSuperScript(value,cur,res);
	//form subgroups
	cmrReplaceByCommaGroup(res->subscript);
	//check for parameters
	params = cmrRequireParameters(res->name,value,cur);
	for (int i = 0 ; i < params ; i++)
	{
		string tmp = cmrExtractSubZone(value,cur);
		LatexFormulasOld * f = new LatexFormulasOld;
		cmrParseLatexFormula(*f,tmp);
		res->params.push_back(f);
		res->argsTotalValue += '{';
		res->argsTotalValue += tmp;
		res->argsTotalValue += '}';
		
	}
	
	res->to = cur;
	res->totalValue = value.substr(start,cur-start);
	
	//ok done
	start = cur;
	return res;
}

/*******************  FUNCTION  *********************/
LatexEntityOld * cmrExtractSubGroup(const string & value,int & start)
{
	//vars
	LatexEntityOld * res = NULL;

	//errors
	assert(value[start] == '(');
	
	//trivial
	if (start >= value.size())
		return NULL;
	
	//setup entity
	res = new LatexEntityOld;
	res->name = '(';
	res->from = start;
	res->parent = value;
	res->argsTotalValue = cmrExtractSubZone(value,start,'(',')');
	res->to = start;
	LatexFormulasOld * f = new LatexFormulasOld;
	cmrParseLatexFormula(*f,res->argsTotalValue);
	res->params.push_back(f);
	
	//skip spaces
	cmrSkipWhiteSpace(value,start);
	
	//if is '^' capture exponent
	if (value[start] == '^')
	{
		cmrExtractSubAndSuperScript(value,start,res);
	}
	
	return res;
}

/*******************  FUNCTION  *********************/
LatexEntityOld * cmrParseLatexEntity(const string & value,int & start)
{
	if (value[start] == '(')
		return cmrExtractSubGroup(value,start);
	else
		return cmrParseLatexEntitySimple(value,start);
}

/*******************  FUNCTION  *********************/
void cmrSkipWhiteSpace(const string & value,int & cur)
{
	while (cmrIsWhiteSpace(value[cur]))
		cur++;
}

/*******************  FUNCTION  *********************/
void cmrParseLatexFormula(LatexFormulasOld & formula,const string & value)
{
	LatexEntityOld * entity;
	int cur = 0;
	formula.string = value;
	cmrSkipWhiteSpace(value,cur);
	while ((entity = cmrParseLatexEntity(value,cur)) != NULL)
	{
		formula.childs.push_back(entity);
		cmrSkipWhiteSpace(value,cur);
	}
}

}
