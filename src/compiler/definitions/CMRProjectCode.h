/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_PROJECT_CODE_H
#define CMR_PROJECT_CODE_H

/********************  HEADERS  *********************/
#include <ostream>
#include <list>
#include "ProjectCodeTree.h"
#include "ProjectIterator.h"
#include "../parsor/LatexFormula.h"
#include "../common/CodeTemplate.h"
#include "../common/CodeTemplateDic.h"


/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  TYPES  **********************/
class ProjectIterator;
class CMRProjectCodeNode;
class CMRProjectCodeEquation;
class CMRProjectCodeIteratorLoop;

/********************  ENUM  ************************/
enum CMRProjectCodeType
{
	//usefull to build the root node
	CMR_PROJECT_CODE_NODE,
	//for eqs
	CMR_PROJECT_CODE_EQUATION,
	//loop on iterators
	CMR_PROJECT_CODE_ITERATOR,
	//Var decl
	CMR_PROJECT_CODE_VAR_DECL,
	//C code simple
	CMR_PROJECT_CODE_CSIMPLE
};

/*********************  STRUCT  *********************/
struct ExtractionLocus
{
	bool isAutoEntry;
	int id;
	int position;
};

/*********************  TYPES  **********************/
typedef std::list<ExtractionLocus> ExtractionLocusList;

/*********************  CLASS  **********************/
class CMRProjectLocalVariable : public ProjectEntity
{
	public:
		CMRProjectLocalVariable( const std::string& latexName, const std::string& longName, const std::string& type, const std::string& defaultValue = "0");
		virtual void genDefinitionCCode ( std::ostream& out, const ProjectContext& context, int padding = 0 ) const;
		virtual void genUsageCCode ( std::ostream& out, const ProjectContext& context, const CMRCompiler::LatexEntity& entity, bool write = false ) const;
	private:
		std::string type;
		CMRCompiler::LatexFormulas defaultValue;
};

/*********************  TYPES  **********************/
class CMRProjectCodeEntry : public ProjectCodeTree<CMRProjectCodeEntry>
{
	public:
		CMRProjectCodeEntry(ProjectContext * context = NULL);
		virtual CMRProjectCodeType getType(void) const = 0;
		virtual void setParentContext(ProjectContext * parentContext);
		ProjectContext & getContext(void);
		CMRProjectCodeNode& addSubBlock( ProjectCodeTreeInsert location = CMR_INSERT_LAST_CHILD );
		CMRProjectCodeEquation & addEquation ( const std::string& eq );
		CMRProjectCodeEquation & addEquation(const std::string& latexName, const std::string& compute,const std::string & op = "=",ProjectCodeTreeInsert location = CMR_INSERT_LAST_CHILD);
		CMRProjectCodeIteratorLoop& addIteratorLoop( const std::string& iterator, ProjectCodeTreeInsert location = CMR_INSERT_LAST_CHILD );
		CMRProjectLocalVariable & addLocalVariable(const std::string & latexName, const std::string & longName,const std::string &type, const std::string & defaultValue, ProjectCodeTreeInsert	
		location = CMR_INSERT_FIRST_CHILD);
		ProjectIterator & addIterator(const std::string & latexName, const std::string & longName, int start,int end);
		virtual void genCCode(std::ostream & out,int padding = 0) const = 0;
		virtual void genChildCCode(std::ostream & out,int padding = 0) const;
		std::ostream & doIndent(std::ostream & out,int baseOffset = 0) const;
		void changeCaptureType(const std::string & name, CaptureType captureType) { assert(false);};
	protected:
		virtual void onParentChange ( CMRProjectCodeEntry * newParent );
	protected:
		ProjectContext context;
};

/*********************  CLASS  **********************/
class CMRCodeValueForCodeEntry : public CMRCompiler::CodeTemplateValue
{
	public:
		CMRCodeValueForCodeEntry(const CMRProjectCodeEntry * obj, int correctIndent = 0) {this->obj = obj;this->correctIndent = correctIndent;};
		virtual void genCode(std::ostream & out ,int indent) const {obj->genCCode(out,indent+this->correctIndent);};
	private:
		const CMRProjectCodeEntry * obj;
		int correctIndent;
};

/*********************  CLASS  **********************/
class CMRProjectCodeNode : public CMRProjectCodeEntry
{
	public:
		CMRProjectCodeNode(ProjectContext * context = NULL);
		virtual CMRProjectCodeType getType(void ) const;
		virtual void genCCode(std::ostream & out,int padding = 0) const;
};

/*********************  CLASS  **********************/
class CMRProjectCodeRootNode : public CMRProjectCodeNode
{
	public:
		CMRProjectCodeRootNode(ProjectContext * context = NULL);
		virtual void genCCode(std::ostream & out,int padding = 0) const;
};

/*********************  CLASS  **********************/
class CMRProjectCodeEquation : public CMRProjectCodeEntry
{
	public:
		CMRProjectCodeEquation( const std::string& latexName, const std::string& compute ,const std::string & op = "=");
		virtual CMRProjectCodeType getType ( void ) const;
		CMRCompiler::LatexFormulas & getFormulas(void);
		CMRCompiler::LatexEntity & getOutput(void);
		const std::string & getOperator(void) const;
		void setOperator(const std::string & op);
		virtual void genCCode(std::ostream & out,int padding = 0) const;
	private:
		CMRCompiler::LatexEntity output;
		CMRCompiler::LatexFormulas formula;
		std::string op;
};

/*********************  CLASS  **********************/
class CMRProjectCodeIteratorLoop : public CMRProjectCodeNode
{
	public:
		CMRProjectCodeIteratorLoop( const std::string& iterator );
		virtual CMRProjectCodeType getType ( void ) const;
		const ProjectIterator & getIterator(void) const;
		virtual void genCCode ( std::ostream& out,int padding = 0 ) const;
	private:
		std::string iterator;
};

/*********************  CLASS  **********************/
class CMRProjectCodeVarDecl : public CMRProjectCodeEntry
{
	public:
		CMRProjectCodeVarDecl ( CMRProjectLocalVariable * variable );
		virtual CMRProjectCodeType getType(void) const;
		virtual void genCCode ( std::ostream& out ,int padding = 0 ) const;
	private:
		CMRProjectLocalVariable * variable;
};

/*********************  CLASS  **********************/
class CMRProjectCConstruct
{
	public:
		CMRProjectCConstruct(const std::string & code);
		CMRProjectCConstruct & arg(const std::string & value);
		void genCCode( std::ostream& out, const ProjectContext& context, int padding = 0 ) const;
	private:
		void loadCode(const std::string & code);
		void extractReplacementLocus( ExtractionLocusList& locusList ) const;
		const CMRCompiler::LatexFormulas * getLocusValue(const ExtractionLocus & locus) const;
	private:
		CMRCompiler::LatexFormulasVector args;
		CMRCompiler::LatexFormulasVector autoArgs;
		std::string code;
};

/*********************  CLASS  **********************/
class CMRProjectCSimpleConstruct : public CMRProjectCodeNode
{
	public:
		CMRProjectCSimpleConstruct( ProjectContext * parentContext,const std::string & code);
		CMRProjectCSimpleConstruct & arg(const std::string & value);
		virtual CMRProjectCodeType getType(void) const;
		virtual void genCCode ( std::ostream& out ,int padding = 0 ) const;
	private:
		CMRProjectCConstruct construct;
};

}

#endif //CMR_PROJECT_CODE_H
