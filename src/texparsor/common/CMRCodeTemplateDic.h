/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_CODE_TEMPLATE_DIC_H
#define CMR_CODE_TEMPLATE_DIC_H

/********************  HEADERS  *********************/
#include <string>
#include <vector>
#include <map>
#include <sstream>

/*********************  CLASS  **********************/
class CMRCodeTemplateValue
{
	public:
		virtual void genCode(std::ostream & out ,int indent) const = 0;
};

/*********************  TYPES  **********************/
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

/*********************  CLASS  **********************/
class CMRCodeTemplateValueString : public CMRCodeTemplateValue
{
	public:
		CMRCodeTemplateValueString(const std::string & value);
		virtual void genCode ( std::ostream& out, int indent ) const;
	private:
		std::string value;
};

/********************  HEADERS  *********************/
#include "CMRCodeTemplateDic_impl.h"

#endif //CMR_CODE_TEMPLATE_DIC_H
