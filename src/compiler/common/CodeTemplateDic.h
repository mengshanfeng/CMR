/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMPILER_CODE_TEMPLATE_DIC_H
#define CMR_COMPILER_CODE_TEMPLATE_DIC_H

/********************  HEADERS  *********************/
#include <map>
#include <string>
#include <vector>
#include <sstream>

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  CLASS  **********************/
class CodeTemplateValue
{
	public:
		virtual void genCode(std::ostream & out ,int indent) const = 0;
};

/*********************  TYPES  **********************/
typedef std::map<std::string,CodeTemplateValue *> ValueDicMap;

/*********************  CLASS  **********************/
class CodeTemplateValueDic
{
	public:
		CodeTemplateValueDic & set(const std::string & name,const char * value);
		CodeTemplateValueDic & set(const std::string & name,const std::string & value);
		CodeTemplateValueDic & set(const std::string & name,CodeTemplateValue * value);
		template <class T> CodeTemplateValueDic & setString(const std::string & name,const T & value);
		template <class T> CodeTemplateValueDic & setCodeGen(const std::string & name,const T * value);
		template <class T> CodeTemplateValueDic & setCodeGen(const std::string & name,const T * value,const std::string & codeType);
		CodeTemplateValue & get(const std::string & name) const;
		bool has(const std::string name) const;
		std::string strKeyList(void) const;
	private:
		ValueDicMap values;
};

/*********************  CLASS  **********************/
template <class T>
class CodeTemplateValueCodeGen : public CodeTemplateValue
{
	public:
		CodeTemplateValueCodeGen(const T * obj) {this->obj = obj;};
		virtual void genCode(std::ostream & out ,int indent) const {obj->genCCode(out,indent);};
	private:
		const T * obj;
};

/*********************  CLASS  **********************/
template <class T>
class CodeTemplateValueCodeGenWidthCodeType : public CodeTemplateValue
{
	public:
		CodeTemplateValueCodeGenWidthCodeType(const T * obj,const std::string & codeType) {this->obj = obj; this->codeType = codeType;};
		virtual void genCode(std::ostream & out ,int indent) const {obj->genCode(out,codeType,indent);};
	private:
		const T * obj;
		std::string codeType;
};

/*********************  CLASS  **********************/
class CodeTemplateValueString : public CodeTemplateValue
{
	public:
		CodeTemplateValueString(const std::string & value);
		virtual void genCode ( std::ostream& out, int indent ) const;
	private:
		std::string value;
};

}

/********************  HEADERS  *********************/
#include "CodeTemplateDic_impl.h"

#endif //CMR_COMPILER_CODE_TEMPLATE_DIC_H
