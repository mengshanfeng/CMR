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
#include "../definitions/ProjectEntity.h"
#include "../definitions/ProjectContext.h"
#include "../definitions/ProjectConstant.h"
#include "../definitions/ProjectMeshVar.h"
#include "../definitions/ProjectAction.h"
#include "../definitions/ProjectIterator.h"
#include "../definitions/ProjectDefinition.h"
#include <definitions/ProjectCallAction.h>
#include <definitions/ProjectOutput.h>
#include "../transformations/CMRTransformation.h"
#include "../common/LangDef.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  TYPES  **********************/
typedef std::vector <ProjectConstant*> CMRProjectConstantVector;
typedef std::vector <CMRCompiler::ProjectMeshVar*> CMRProjectVariableVector;
typedef std::vector <ProjectIterator*> CMRProjectIteratorVector;
typedef std::vector <CMRCompiler::ProjectAction*> CMRProjectActionVector;
typedef std::vector <ProjectDefinition*> CMRProjectDefinitionVector;
typedef std::vector <ProjectCallAction*> CMRProjectCallActionVector;

/*********************  CLASS  **********************/
class CMRProject2
{
	public:
		CMRProject2(void);
		ProjectConstant & addConstant(const std::string& latexName, const std::string& longName);
		CMRCompiler::ProjectMeshVar & addvariable(const std::string& latexName, const std::string& longName, const std::string& type);
		CMRCompiler::ProjectAction & addAction(std::string name,std::string descr = "");
		ProjectDefinition & addDefinition(const std::string& latexName, const std::string& longName);
		ProjectDefinition& addDefinition( const std::string& latexName, const std::string& longName, const std::string& compute );
		ProjectIterator & addIterator(const std::string& latexName, const std::string& longName ,int start, int end);
		ProjectCallAction & addInitCallAction(const std::string & actionName);
		ProjectCallAction & addMainLoopCallAction(const std::string & actionName);
		void addOutputEntry(const std::string &name, const std::string & type, const std::string & value);
		void addUserHeader(const std::string & value);
		void runTransformation(CMRTransformation & transf);
		void genCCode(std::ostream & out);
		void printDebug(void);
	protected:
		void genCCodeOfConsts(std::ostream & out);
		void genCCodeOfVariables(std::ostream & out);
		void genCCodeOfDefinitions(std::ostream& out);
		void genCCodeOfActions( std::ostream& out, CMRCompiler::LangDef& lang );
		void genCCodeOfMain( std::ostream & out, CMRCompiler::LangDef& lang );
	private:
		CMRProjectConstantVector constants;
		CMRProjectVariableVector variables;
		CMRProjectActionVector actions;
		CMRProjectDefinitionVector definitions;
		CMRProjectIteratorVector iterators;
		CMRProjectCallActionVector initActions;
		CMRProjectCallActionVector loopActions;
		ProjectOutput output;
		ProjectContext rootContext;
		StringVector userHeaders;
		CMRCompiler::LangDef lang;
};

}

#endif //CMR_PROJECT_H
