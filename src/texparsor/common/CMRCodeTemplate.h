/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_CODE_TEMPLATE_H
#define CMR_CODE_TEMPLATE_H

/********************  HEADERS  *********************/
#include <string>
#include <vector>
#include <map>
#include <sstream>

/*********************  STRUCT  *********************/
struct CMRCodeTemplateEntry
{
	CMRCodeTemplateEntry(const std::string & value,bool replacedValue = false, int indent = -1);
	std::string value;
	bool replacedValue;
	int indent;
};

/*********************  CLASS  **********************/
class CMRCodeTemplateValue
{
	public:
		virtual void genCode(std::ostream & out ,int indent) const = 0;
};

/*********************  CLASS  **********************/
template <class T>
class CMRCodeTemplateValueCodeGen : public CMRCodeTemplateValue
{
	public:
		CMRCodeTemplateValueCodeGen(const T * obj) {this->obj = obj;};
		virtual void genCode(std::ostream & out ,int indent) const {obj->genCCode(out,indent);};
	private:
		const T * obj;
};

/*********************  CLASS  **********************/
template <class T>
class CMRCodeTemplateValueCodeGenWidthCodeType : public CMRCodeTemplateValue
{
	public:
		CMRCodeTemplateValueCodeGenWidthCodeType(const T * obj,const std::string & codeType) {this->obj = obj; this->codeType = codeType;};
		virtual void genCode(std::ostream & out ,int indent) const {obj->genCode(out,codeType,indent);};
	private:
		const T * obj;
		std::string codeType;
};

/*********************  TYPES  **********************/
typedef std::vector<CMRCodeTemplateEntry> CMRCodeTemplateEntries;
typedef std::map<std::string,CMRCodeTemplateValue *> CMRValueDicMap;

/*********************  CLASS  **********************/
class CMRCodeTemplateValueDic
{
	public:
		CMRCodeTemplateValueDic & set(const std::string & name,const char * value);
		CMRCodeTemplateValueDic & set(const std::string & name,const std::string & value);
		CMRCodeTemplateValueDic & set(const std::string & name,CMRCodeTemplateValue * value);
		template <class T> CMRCodeTemplateValueDic & setString(const std::string & name,const T & value);
		template <class T> CMRCodeTemplateValueDic & setCodeGen(const std::string & name,const T * value);
		template <class T> CMRCodeTemplateValueDic & setCodeGen(const std::string & name,const T * value,const std::string & codeType);
		CMRCodeTemplateValue & get(const std::string & name) const;
		bool has(const std::string name) const;
		std::string strKeyList(void) const;
	private:
		CMRValueDicMap values;
};

/*********************  CLASS  **********************/
class CMRCodeTemplate
{
	public:
		CMRCodeTemplate(const std::string & model);
		std::ostream & applyOn( std::ostream& out, const CMRCodeTemplateValueDic& dic, int indentLevel = 0 ) const;
		std::string strRequiredVars(void) const;
	private:
		void setModel(const std::string & model);
	private:
		CMRCodeTemplateEntries model;
};

/*******************  FUNCTION  *********************/
template <class T>
CMRCodeTemplateValueDic& CMRCodeTemplateValueDic::setString ( const std::string& name, const T& value )
{
	std::stringstream buffer;
	buffer << value;
	values[name] = value;
}

/*******************  FUNCTION  *********************/
template <class T>
CMRCodeTemplateValueDic& CMRCodeTemplateValueDic::setCodeGen ( const std::string& name, const T* value , const std::string & codeType)
{
	values[name] = new CMRCodeTemplateValueCodeGenWidthCodeType<T>(value,codeType);
}

/*******************  FUNCTION  *********************/
template <class T>
CMRCodeTemplateValueDic& CMRCodeTemplateValueDic::setCodeGen ( const std::string& name, const T* value)
{
	values[name] = new CMRCodeTemplateValueCodeGen<T>(value);
}


/*********************  CLASS  **********************/
class CMRCodeTemplateValueString : public CMRCodeTemplateValue
{
	public:
		CMRCodeTemplateValueString(const std::string & value);
		virtual void genCode ( std::ostream& out, int indent ) const;
	private:
		std::string value;
};

#endif //CMR_CODE_TEMPLATE_H
