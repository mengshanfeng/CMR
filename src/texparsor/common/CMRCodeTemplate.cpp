/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cassert>
#include "CMRDebug.h"
#include "CMRCommon.h"
#include "CMRCodeTemplate.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
CMRCodeTemplate::CMRCodeTemplate ( const std::string& model )
{
	this->setModel(model);
}

/*******************  FUNCTION  *********************/
void CMRCodeTemplate::setModel ( const std::string& model )
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
				this->model.push_back(CMRCodeTemplateEntry(buffer,capture,indent));
				buffer.clear();
				indent = -1;
			}
			capture = !capture;
		} else if (model[i] == '\n') {
			buffer += model[i];
			this->model.push_back(CMRCodeTemplateEntry(buffer,capture,indent));
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
	this->model.push_back(CMRCodeTemplateEntry(buffer,capture));
}

/*******************  FUNCTION  *********************/
ostream& CMRCodeTemplate::applyOn ( ostream& out, const CMRCodeTemplateValueDic& dic, int indentLevel ) const
{
	//loop on model parts
	for (CMRCodeTemplateEntries::const_iterator it = model.begin() ; it != model.end() ; ++it)
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
CMRCodeTemplateEntry::CMRCodeTemplateEntry ( const string& value, bool replacedValue, int indent )
{
	this->value = value;
	this->replacedValue = replacedValue;
	this->indent = indent;
}

/*******************  FUNCTION  *********************/
CMRCodeTemplateValue& CMRCodeTemplateValueDic::get ( const string& name ) const
{
	CMRValueDicMap::const_iterator it = values.find(name);
	cmrAssume(it != values.end(),"Request invalid key in current code value directonnary : '%s', list of keys is : %s",name.c_str(),strKeyList().c_str());
	return *(it->second);
}

/*******************  FUNCTION  *********************/
bool CMRCodeTemplateValueDic::has ( const string name ) const
{
	CMRValueDicMap::const_iterator it = values.find(name);
	return (it != values.end());
}

/*******************  FUNCTION  *********************/
CMRCodeTemplateValueDic& CMRCodeTemplateValueDic::set ( const string& name, const char* value )
{
	this->values[name] = new CMRCodeTemplateValueString(value);
}

/*******************  FUNCTION  *********************/
CMRCodeTemplateValueDic& CMRCodeTemplateValueDic::set ( const string& name, const string& value )
{
	this->values[name] = new CMRCodeTemplateValueString(value);
}

/*******************  FUNCTION  *********************/
CMRCodeTemplateValueDic& CMRCodeTemplateValueDic::set ( const string& name, CMRCodeTemplateValue* value )
{
	assert(value != NULL);
	this->values[name] = value;
}

/*******************  FUNCTION  *********************/
string CMRCodeTemplateValueDic::strKeyList ( void ) const
{
	//vars
	string res;
	
	//loop on all keys
	for (CMRValueDicMap::const_iterator it = values.begin() ; it != values.end() ; ++it)
	{
		if (res.empty() == false)
			res += ", ";
		res += it->first;
	}
	
	return res;
}

/*******************  FUNCTION  *********************/
string CMRCodeTemplate::strRequiredVars ( void ) const
{
	//var
	string res;
	
	//loop on model parts
	for (CMRCodeTemplateEntries::const_iterator it = model.begin() ; it != model.end() ; ++it)
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
CMRCodeTemplateValueString::CMRCodeTemplateValueString ( const string& value )
{
	this->value = value;
}

/*******************  FUNCTION  *********************/
void CMRCodeTemplateValueString::genCode ( ostream& out, int indent ) const
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
