/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_LANG_DEF_H
#define CMR_LANG_DEF_H

/********************  HEADERS  *********************/
#include <map>
#include <string>

/*********************  TYPES  **********************/
class CMRCodeTemplate;
class CMRCodeTemplateValueDic;

/*********************  TYPES  **********************/
typedef std::map<std::string,CMRCodeTemplate *> CMRCodeTemplateMap;

/*********************  CLASS  **********************/
//TODO make this class generic and inherit for "cpp/cmr" output language
class CMRLangDef
{
	public:
		CMRLangDef(void);
		void applyOn(std::ostream& out,const std::string & templateName, const CMRCodeTemplateValueDic& dic, int indentLevel) const;
		const CMRCodeTemplate & getCodeTemplate(const std::string & name) const;
	protected:
		void setup(const std::string & name,const char * value);
	private:
		CMRCodeTemplateMap templates;
};

#endif //CMR_LANG_DEF_H
