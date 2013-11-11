/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMPILER__CODE_TEMPLATE_H
#define CMR_COMPILER__CODE_TEMPLATE_H

/********************  HEADERS  *********************/
#include <map>
#include <string>
#include <vector>
#include <sstream>

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  TYPES  **********************/
class CodeTemplateValueDic;

/*********************  STRUCT  *********************/
struct CodeTemplateEntry
{
	CodeTemplateEntry(const std::string & value,bool replacedValue = false, int indent = -1);
	std::string value;
	bool replacedValue;
	int indent;
};

/*********************  TYPES  **********************/
typedef std::vector<CodeTemplateEntry> CodeTemplateEntries;

/*********************  CLASS  **********************/
class CodeTemplate
{
	public:
		CodeTemplate(const std::string & model);
		std::ostream & applyOn( std::ostream& out, const CodeTemplateValueDic& dic, int indentLevel = 0 ) const;
		std::string strRequiredVars(void) const;
	private:
		void setModel(const std::string & model);
	private:
		CodeTemplateEntries model;
};

}

#endif //CMR_COMPILER__CODE_TEMPLATE_H
