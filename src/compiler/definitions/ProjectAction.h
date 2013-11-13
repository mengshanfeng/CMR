/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMPILER_PROJECT_ACTION_H
#define CMR_COMPILER_PROJECT_ACTION_H

/********************  HEADERS  *********************/
#include "CMRProjectCode.h"

/*********************  TYPES  **********************/
class CMRTransformation;
class CMRProjectIterator;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  TYPES  **********************/
class LangDef;

/*********************  CLASS  **********************/
class ProjectActionParameter : public ProjectEntity
{
	public:
		ProjectActionParameter ( const std::string& latexName, const std::string& longName, const std::string & type );
		virtual void genDefinitionCCode ( std::ostream& out, const ProjectContext& context, int padding = 0 ) const;
		virtual void genUsageCCode ( std::ostream& out, const ProjectContext& context, const LatexEntity& entity, bool write = false ) const;
		const std::string & getType(void) const;
	private:
		std::string type;
};

/*********************  TYPES  **********************/
typedef std::vector<ProjectActionParameter*> ProjectActionParameterVector;

/*********************  CLASS  **********************/
class CodeTemplateValueActionParameters : public CodeTemplateValue
{
	public:
		CodeTemplateValueActionParameters(const ProjectActionParameterVector * list, const std::string & separator = ",",bool firstSeparator = false,bool lastSeparator = false, bool indentEach = false);
		virtual void genCode ( std::ostream& out, int indent ) const;
	private:
		bool firstSeparator;
		bool lastSeparator;
		std::string separator;
		bool indentEach;
		const ProjectActionParameterVector * list;
};

/*********************  CLASS  **********************/
class ProjectAction
{
	public:
		ProjectAction ( const std::string & name, const std::string & descr, const std::string & loopType,ProjectContext * parentContext = NULL);
		virtual void printDebug(std::ostream & out) const;
		virtual void genDefinitionCCode ( std::ostream& out, const LangDef & lang, const ProjectContext& context, int padding = 0 ) const;
		virtual void genUsageCCode ( std::ostream& out, const ProjectContext& context, const LatexEntity& entity, bool write = false ) const;
		void runTransformation(CMRTransformation & transf);
		CMRProjectCodeEquation & addEquation(const std::string& eq);
		CMRProjectCodeEquation & addEquation(const std::string& latexName, const std::string& compute,const std::string & op = "=");
		CMRProjectCodeIteratorLoop& addIteratorLoop( const std::string& iterator);
		CMRProjectLocalVariable & addLocalVariable(const std::string & latexName, const std::string & longName,const std::string &type, const std::string & defaultValue);
		ProjectIterator & addIterator(const std::string & latexName, const std::string & longName, int start, int end);
		void changeCaptureType(const std::string & name, CaptureType captureType) { assert(false);};
		ProjectContext & getContext(void);
		CMRProjectCodeEntry * insert(CMRProjectCodeEntry * entry,ProjectCodeTreeInsert location = CMR_INSERT_LAST_CHILD);
		ProjectActionParameter & addParameter(const std::string & latexName, const std::string & longName,const std::string &type);
	private:
		CMRProjectCodeRootNode ops;
		std::string name;
		std::string descr;
		std::string loopType;
		ProjectActionParameterVector parameters;
};

}

#endif //CMR_COMPILER_PROJECT_ACTION_H
