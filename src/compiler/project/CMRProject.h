/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_PROJECT_H
#define CMR_PROJECT_H

/********************  HEADERS  *********************/
#include <vector>
#include "../definitions/CMRProjectEntity.h"
#include "../definitions/CMRProjectContext.h"
#include "../definitions/CMRProjectConstant.h"
#include "../definitions/ProjectMeshVar.h"
#include "../definitions/ProjectAction.h"
#include "../definitions/CMRProjectIterator.h"
#include "../definitions/CMRProjectDefinition.h"
#include <definitions/CMRProjectCallAction.h>
#include "../transformations/CMRTransformation.h"
#include "../common/LangDef.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  TYPES  **********************/
typedef std::vector <CMRProjectConstant*> CMRProjectConstantVector;
typedef std::vector <CMRCompiler::ProjectMeshVar*> CMRProjectVariableVector;
typedef std::vector <CMRProjectIterator*> CMRProjectIteratorVector;
typedef std::vector <CMRCompiler::ProjectAction*> CMRProjectActionVector;
typedef std::vector <CMRProjectDefinition*> CMRProjectDefinitionVector;
typedef std::vector <CMRProjectCallAction*> CMRProjectCallActionVector;

/*********************  CLASS  **********************/
class CMRProject2
{
	public:
		CMRProject2(void);
		CMRProjectConstant & addConstant(const std::string& latexName, const std::string& longName);
		CMRCompiler::ProjectMeshVar & addvariable(const std::string& latexName, const std::string& longName, const std::string& type);
		CMRCompiler::ProjectAction & addAction(std::string name,std::string descr = "");
		CMRProjectDefinition & addDefinition(const std::string& latexName, const std::string& longName);
		CMRProjectDefinition& addDefinition( const std::string& latexName, const std::string& longName, const std::string& compute );
		CMRProjectIterator & addIterator(const std::string& latexName, const std::string& longName ,int start, int end);
		CMRProjectCallAction & addInitCallAction(const std::string & actionName);
		CMRProjectCallAction & addMainLoopCallAction(const std::string & actionName);
		void runTransformation(CMRTransformation & transf);
		void genCCode(std::ostream & out);
		void printDebug(void);
	protected:
		void genCCodeOfConsts(std::ostream & out);
		void genCCodeOfVariables(std::ostream & out);
		void genCCodeOfDefinitions(std::ostream& out);
		void genCCodeOfActions( std::ostream& out, CMRCompiler::LangDef& lang );
		void genCCodeOfInit( std::ostream & out, CMRCompiler::LangDef& lang );
		void genCCodeOfMainLoop( std::ostream & out, CMRCompiler::LangDef& lang );
		void genCCodeOfMain( std::ostream & out, CMRCompiler::LangDef& lang );
	private:
		CMRProjectConstantVector constants;
		CMRProjectVariableVector variables;
		CMRProjectActionVector actions;
		CMRProjectDefinitionVector definitions;
		CMRProjectIteratorVector iterators;
		CMRProjectCallActionVector initActions;
		CMRProjectCallActionVector loopActions;
		CMRProjectContext rootContext;
		CMRCompiler::LangDef lang;
};

}

#endif //CMR_PROJECT_H
