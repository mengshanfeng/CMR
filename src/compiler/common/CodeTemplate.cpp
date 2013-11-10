/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cassert>
#include "Debug.h"
#include "Common.h"
#include "CodeTemplate.h"
#include "CodeTemplateDic.h"

/**********************  USING  *********************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
CodeTemplate::CodeTemplate ( const std::string& model )
{
	this->setModel(model);
}

/*******************  FUNCTION  *********************/
void CodeTemplate::setModel ( const std::string& model )
{
	//vars
	string buffer;
	bool capture = false;
	int indent = 0;
	char prev = '\0';
	
	//split on @@
	for (int i = 0 ; i < model.size() ; i++)
	{
		if (model[i] == '@' && prev != '\\')
		{
			if (buffer.empty() == false)
			{
				this->model.push_back(CodeTemplateEntry(buffer,capture,indent));
				buffer.clear();
				indent = -1;
			}
			capture = !capture;
		} else if (model[i] == '\n') {
			buffer += model[i];
			this->model.push_back(CodeTemplateEntry(buffer,capture,indent));
			indent = 0;
			buffer.clear();
		} else if (model[i] == '\t' && indent != -1) {
			indent++;
		} else {
			buffer += model[i];
		}
		prev = model[i];
	}
	
	//check status
	cmrAssume(capture == false,"Missing end of variable name in current code model, please check your @ splitting : '%s'",model.c_str());
	
	//flush last
	this->model.push_back(CodeTemplateEntry(buffer,capture));
}

/*******************  FUNCTION  *********************/
ostream& CodeTemplate::applyOn ( ostream& out, const CodeTemplateValueDic& dic, int indentLevel ) const
{
	//loop on model parts
	for (CodeTemplateEntries::const_iterator it = model.begin() ; it != model.end() ; ++it)
	{
		if (it->replacedValue == true)
		{
			cmrAssume(dic.has(it->value) == true, "The code model require variable '%s' which is not defined in value directonnary which has : %s.",it->value.c_str(),dic.strKeyList().c_str());
			if (it->indent == -1 || indentLevel == -1)
				dic.get(it->value).genCode(out,-1);
			else
				dic.get(it->value).genCode(out,indentLevel + it->indent);
		} else {
			if (it->indent != -1 && indentLevel != -1)
				doIndent(out,indentLevel + it->indent);
			out << it->value;
		}
	}
	
	return out;
}

/*******************  FUNCTION  *********************/
CodeTemplateEntry::CodeTemplateEntry ( const string& value, bool replacedValue, int indent )
{
	this->value = value;
	this->replacedValue = replacedValue;
	this->indent = indent;
}

/*******************  FUNCTION  *********************/
CodeTemplateValue& CodeTemplateValueDic::get ( const string& name ) const
{
	ValueDicMap::const_iterator it = values.find(name);
	cmrAssume(it != values.end(),"Request invalid key in current code value directonnary : '%s', list of keys is : %s",name.c_str(),strKeyList().c_str());
	return *(it->second);
}

/*******************  FUNCTION  *********************/
bool CodeTemplateValueDic::has ( const string name ) const
{
	ValueDicMap::const_iterator it = values.find(name);
	return (it != values.end());
}

/*******************  FUNCTION  *********************/
CodeTemplateValueDic& CodeTemplateValueDic::set ( const string& name, const char* value )
{
	assert(value != NULL);
	this->values[name] = new CodeTemplateValueString(value);
	return *this;
}

/*******************  FUNCTION  *********************/
CodeTemplateValueDic& CodeTemplateValueDic::set ( const string& name, const string& value )
{
	this->values[name] = new CodeTemplateValueString(value);
	return *this;
}

/*******************  FUNCTION  *********************/
CodeTemplateValueDic& CodeTemplateValueDic::set ( const string& name, CodeTemplateValue* value )
{
	assert(value != NULL);
	this->values[name] = value;
	return *this;
}

/*******************  FUNCTION  *********************/
string CodeTemplateValueDic::strKeyList ( void ) const
{
	//vars
	string res;
	
	//loop on all keys
	for (ValueDicMap::const_iterator it = values.begin() ; it != values.end() ; ++it)
	{
		if (res.empty() == false)
			res += ", ";
		res += it->first;
	}
	
	return res;
}

/*******************  FUNCTION  *********************/
string CodeTemplate::strRequiredVars ( void ) const
{
	//var
	string res;
	
	//loop on model parts
	for (CodeTemplateEntries::const_iterator it = model.begin() ; it != model.end() ; ++it)
	{
		if (it->replacedValue == true)
		{
			if (res.empty() == false)
				res += ", ";
			res += it->value;
		}
	}
	
	return res;
}

/*******************  FUNCTION  *********************/
CodeTemplateValueString::CodeTemplateValueString ( const string& value )
{
	this->value = value;
}

/*******************  FUNCTION  *********************/
void CodeTemplateValueString::genCode ( ostream& out, int indent ) const
{
	if (indent <= 0 || value.empty())
	{
		out << value;
	} else {
		if (indent > 0)
			doIndent(out,indent);
		for (int i = 0 ; i < value.size() ; i++)
		{
			out << value[i];
			if (value[i] == '\n' && value[i+1] != '\0' && indent > 0)
				doIndent(out,indent);
		}
	}
}

};
