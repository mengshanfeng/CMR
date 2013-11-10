/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "Debug.h"
#include "LangDef.h"
#include "CodeTemplate.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  CONSTS  *********************/
static const char * CELL_ACTION_BODY = "//@descr@\n\
struct Action@name@\n\
{\n\
	static void cellAction(const VarSystem::CellAccessor & in,VarSystem::CellAccessor& out,const CMRCellPosition & pos,int x,int y@params@)\n\
	{\n\
		@code@\n\
	}\n\
	@storage@\n\
};\n\n";

/*******************  FUNCTION  *********************/
LangDef::LangDef ( void )
{
	setup("cellActionBodyCode",CELL_ACTION_BODY);
}

/*******************  FUNCTION  *********************/
void LangDef::setup ( const std::string& name, const char* value )
{
	this->templates[name] = new CodeTemplate(value);
}

/*******************  FUNCTION  *********************/
const CodeTemplate& LangDef::getCodeTemplate ( const std::string& name ) const
{
	CodeTemplateMap::const_iterator it = templates.find(name);
	cmrAssume(it != templates.end(),"Invalid langauge definition '%s'.",name.c_str());
	return *(it->second);
}

/*******************  FUNCTION  *********************/
void LangDef::applyOn ( std::ostream& out, const std::string& templateName, const CodeTemplateValueDic& dic, int indentLevel ) const
{
	getCodeTemplate(templateName).applyOn(out,dic,indentLevel);
}

};
