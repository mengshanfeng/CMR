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
#include "CMRCodeTemplateDic.h"

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

#endif //CMR_CODE_TEMPLATE_DIC_IMPL_H
