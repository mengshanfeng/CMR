/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_PROJECT_ACTION_H
#define CMR_PROJECT_ACTION_H

/********************  HEADERS  *********************/
#include "CMRProjectCode.h"
#include "CMRProjectIterator.h"
#include "common/CodeTemplateDic.h"

/*********************  TYPES  **********************/
class CMRTransformation;

namespace CMRCompiler{
	class LangDef;
};

/*********************  CLASS  **********************/
class CMRProjectActionParameter : public CMRProjectEntity
{
	public:
		CMRProjectActionParameter ( const std::string& latexName, const std::string& longName, const std::string & type );
		virtual void genDefinitionCCode ( std::ostream& out, const CMRProjectContext& context, int padding = 0 ) const;
		virtual void genUsageCCode ( std::ostream& out, const CMRProjectContext& context, const CMRCompiler::LatexEntity& entity, bool write = false ) const;
		const std::string & getType(void) const;
	private:
		std::string type;
};

/*********************  TYPES  **********************/
typedef std::vector<CMRProjectActionParameter*> CMRProjectActionParameterVector;

/*********************  CLASS  **********************/
class CodeTemplateValueActionParameters : public CMRCompiler::CodeTemplateValue
{
	public:
		CodeTemplateValueActionParameters(const CMRProjectActionParameterVector * list, const std::string & separator = ",",bool firstSeparator = false,bool lastSeparator = false, bool indentEach = false);
		virtual void genCode ( std::ostream& out, int indent ) const;
	private:
		bool firstSeparator;
		bool lastSeparator;
		std::string separator;
		bool indentEach;
		const CMRProjectActionParameterVector * list;
};

/*********************  CLASS  **********************/
class CMRProjectAction
{
	public:
		CMRProjectAction ( const std::string & name, const std::string & descr,CMRProjectContext * parentContext = NULL);
		virtual void printDebug(std::ostream & out) const;
		virtual void genDefinitionCCode ( std::ostream& out, const CMRCompiler::LangDef & lang, const CMRProjectContext& context, int padding = 0 ) const;
		virtual void genUsageCCode ( std::ostream& out, const CMRProjectContext& context, const CMRCompiler::LatexEntity& entity, bool write = false ) const;
		void runTransformation(CMRTransformation & transf);
		CMRProjectCodeEquation & addEquation(const std::string& eq);
		CMRProjectCodeEquation & addEquation(const std::string& latexName, const std::string& compute,const std::string & op = "=");
		CMRProjectCodeIteratorLoop& addIteratorLoop( const std::string& iterator);
		CMRProjectLocalVariable & addLocalVariable(const std::string & latexName, const std::string & longName,const std::string &type, const std::string & defaultValue);
		CMRProjectIterator & addIterator(const std::string & latexName, const std::string & longName, int start, int end);
		void changeCaptureType(const std::string & name, enum CMRCaptureType captureType) { assert(false);};
		CMRProjectContext & getContext(void);
		CMRProjectCodeEntry * insert(CMRProjectCodeEntry * entry,CMRProjectCodeTreeInsert location = CMR_INSERT_LAST_CHILD);
		CMRProjectActionParameter & addParameter(const std::string & latexName, const std::string & longName,const std::string &type);
	private:
		CMRProjectCodeRootNode ops;
		std::string name;
		std::string descr;
		CMRProjectActionParameterVector parameters;
};

#endif //CMR_PROJECT_ACTION_H
