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
#include "CMRProjectEntity.h"
#include "CMRProjectContext.h"
#include "CMRProjectConstant.h"
#include "CMRProjectMeshVar.h"
#include "CMRProjectAction.h"
#include "CMRProjectIterator.h"
#include "CMRProjectDefinition.h"
#include "../transformations/CMRTransformation.h"

/*********************  TYPES  **********************/
typedef std::vector <CMRProjectConstant*> CMRProjectConstantVector;
typedef std::vector <CMRProjectMeshVar*> CMRProjectVariableVector;
typedef std::vector <CMRProjectIterator*> CMRProjectIteratorVector;
typedef std::vector <CMRProjectAction*> CMRProjectActionVector;
typedef std::vector <CMRProjectDefinition*> CMRProjectDefinitionVector;

/*********************  CLASS  **********************/
class CMRProject2
{
	public:
		CMRProject2(void);
		CMRProjectConstant & addConstant(const std::string& latexName, const std::string& longName);
		CMRProjectMeshVar & addvariable(const std::string& latexName, const std::string& longName, const std::string& type);
		CMRProjectAction & addAction(std::string name,std::string descr = "");
		CMRProjectDefinition & addDefinition(const std::string& latexName, const std::string& longName);
		CMRProjectDefinition& addDefinition( const std::string& latexName, const std::string& longName, const std::string& compute );
		CMRProjectIterator & addIterator(const std::string& latexName, const std::string& longName ,int start, int end);
		void runTransformation(CMRTransformation & transf);
		void genCCode(std::ostream & out);
		void printDebug(void);
	protected:
		void genCCodeOfConsts(std::ostream & out);
		void genCCodeOfVariables(std::ostream & out);
		void genCCodeOfDefinitions(std::ostream& out);
		void genCCodeOfActions(std::ostream& out);
	private:
		CMRProjectConstantVector constants;
		CMRProjectVariableVector variables;
		CMRProjectActionVector actions;
		CMRProjectDefinitionVector definitions;
		CMRProjectIteratorVector iterators;
		CMRProjectContext rootContext;
};

#endif //CMR_PROJECT_H
