/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "CMRDebug.h"
#include "CMRLangDef.h"
#include "CMRCodeTemplate.h"

/*********************  CONSTS  *********************/
static const char * CELL_ACTION_BODY = "//@descr@\n\
struct Action@name@\n\
{\n\
	static void cellAction(const VarSystem::CellAccessor & in,VarSystem::CellAccessor& out,const CMRCellPosition & pos,int x,int y)\n\
	{\n\
		@code@\n\
	}\n\
};\n\n";

/*******************  FUNCTION  *********************/
CMRLangDef::CMRLangDef ( void )
{
	setup("cellActionBodyCode",CELL_ACTION_BODY);
}

/*******************  FUNCTION  *********************/
void CMRLangDef::setup ( const std::string& name, const char* value )
{
	this->templates[name] = new CMRCodeTemplate(value);
}

/*******************  FUNCTION  *********************/
const CMRCodeTemplate& CMRLangDef::getCodeTemplate ( const std::string& name ) const
{
	CMRCodeTemplateMap::const_iterator it = templates.find(name);
	cmrAssume(it != templates.end(),"Invalid langauge definition '%s'.",name.c_str());
	return *(it->second);
}

/*******************  FUNCTION  *********************/
void CMRLangDef::applyOn ( std::ostream& out, const std::string& templateName, const CMRCodeTemplateValueDic& dic, int indentLevel ) const
{
	getCodeTemplate(templateName).applyOn(out,dic,indentLevel);
}
