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
static const char * CELL_ACTION_BODY_OUTPLACE_WITHPOS = "//@descr@\n\
struct Action@name@\n\
{\n\
	void cellAction(const VarSystem::CellAccessor & in,VarSystem::CellAccessor& out,const CMRCellPosition & pos,int x,int y) const\n\
	{\n\
		@code@\n\
	}\n\
	@storage@\n\
	typedef @looptype@<VarSystem,Action@name@> LoopType;\n\
};\n\n";
static const char * CELL_ACTION_BODY_OUTPLACE_NOPOS = "//@descr@\n\
struct Action@name@\n\
{\n\
	void cellAction(const VarSystem::CellAccessor & in,VarSystem::CellAccessor& out,int x,int y) const\n\
	{\n\
		@code@\n\
	}\n\
	@storage@\n\
	typedef @looptype@<VarSystem,Action@name@> LoopType;\n\
};\n\n";
static const char * CELL_ACTION_BODY_INPLACE_NOPOS = "//@descr@\n\
struct Action@name@\n\
{\n\
	void cellAction(VarSystem::CellAccessor & cell,int x,int y) const\n\
	{\n\
		@code@\n\
	}\n\
	@storage@\n\
	typedef @looptype@<VarSystem,Action@name@> LoopType;\n\
};\n\n";
static const char * CELL_ACTION_BODY_INPLACE_WITHPOS = "//@descr@\n\
struct Action@name@\n\
{\n\
	void cellAction(VarSystem::CellAccessor & cell,const CMRCellPosition & pos,int x,int y) const\n\
	{\n\
		@code@\n\
	}\n\
	@storage@\n\
	typedef @looptype@<VarSystem,Action@name@> LoopType;\n\
};\n\n";

/*******************  FUNCTION  *********************/
LangDef::LangDef ( void )
{
	setup("cellActionBodyCodeCMRMeshOperationSimpleLoopWithPos",CELL_ACTION_BODY_OUTPLACE_WITHPOS);
	setup("cellActionBodyCodeCMRMeshOperationSimpleLoop",CELL_ACTION_BODY_OUTPLACE_NOPOS);
	setup("cellActionBodyCodeCMRMeshOperationSimpleLoopInPlaceWithPos",CELL_ACTION_BODY_INPLACE_WITHPOS);
	setup("cellActionBodyCodeCMRMeshOperationSimpleLoopInPlace",CELL_ACTION_BODY_INPLACE_NOPOS);
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

}
