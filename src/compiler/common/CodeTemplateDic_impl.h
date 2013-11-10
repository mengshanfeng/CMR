/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_CODE_TEMPLATE_DIC_IMPL_H
#define CMR_CODE_TEMPLATE_DIC_IMPL_H

/********************  HEADERS  *********************/
#include "CodeTemplateDic.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
template <class T>
CodeTemplateValueDic& CodeTemplateValueDic::setString ( const std::string& name, const T& value )
{
	std::stringstream buffer;
	buffer << value;
	values[name] = value;
}

/*******************  FUNCTION  *********************/
template <class T>
CodeTemplateValueDic& CodeTemplateValueDic::setCodeGen ( const std::string& name, const T* value , const std::string & codeType)
{
	values[name] = new CodeTemplateValueCodeGenWidthCodeType<T>(value,codeType);
}

/*******************  FUNCTION  *********************/
template <class T>
CodeTemplateValueDic& CodeTemplateValueDic::setCodeGen ( const std::string& name, const T* value)
{
	values[name] = new CodeTemplateValueCodeGen<T>(value);
}

};

#endif //CMR_CODE_TEMPLATE_DIC_IMPL_H
