/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMPILER_LANG_DEF_H
#define CMR_COMPILER_LANG_DEF_H

/********************  HEADERS  *********************/
#include <map>
#include <string>

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  TYPES  **********************/	
class CodeTemplate;
class CodeTemplateValueDic;

/*********************  TYPES  **********************/
typedef std::map<std::string,CodeTemplate *> CodeTemplateMap;

/*********************  CLASS  **********************/
//TODO make this class generic and inherit for "cpp/cmr" output language
class LangDef
{
	public:
		LangDef(void);
		void applyOn(std::ostream& out,const std::string & templateName, const CodeTemplateValueDic& dic, int indentLevel) const;
		const CodeTemplate & getCodeTemplate(const std::string & name) const;
	protected:
		void setup(const std::string & name,const char * value);
	private:
		CodeTemplateMap templates;
};

};

#endif //CMR_COMPILER_LANG_DEF_H
