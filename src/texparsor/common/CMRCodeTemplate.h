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

/*********************  TYPES  **********************/
class CMRCodeTemplateValueDic;

/*********************  STRUCT  *********************/
struct CMRCodeTemplateEntry
{
	CMRCodeTemplateEntry(const std::string & value,bool replacedValue = false, int indent = -1);
	std::string value;
	bool replacedValue;
	int indent;
};

/*********************  TYPES  **********************/
typedef std::vector<CMRCodeTemplateEntry> CMRCodeTemplateEntries;

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


#endif //CMR_CODE_TEMPLATE_H
