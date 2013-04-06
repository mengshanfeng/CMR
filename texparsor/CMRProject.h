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

/*********************  TYPES  **********************/
class CMREntityConstant;
class CMRProjectVariable;
class CMRProjectIterator;
class CMRProjectEquation;
class CMRProjectAction;
class CMRProjectDefinition;
class CMRProjectContext;

/*********************  TYPES  **********************/
typedef std::vector <CMREntityConstant*> CMRProjectConstantVector;
typedef std::vector <CMRProjectVariable*> CMRProjectVariableVector;
typedef std::vector <CMRProjectIterator*> CMRProjectIteratorVector;
typedef std::vector <CMRProjectAction*> CMRProjectActionVector;
typedef std::vector <CMRProjectDefinition*> CMRProjectDefinitionVector;

/*********************  CLASS  **********************/
class CMRProjectLocalVariable : public CMREntity
{
	public:
		CMRProjectLocalVariable(const std::string& latexName, const std::string& longName) : CMREntity(latexName,longName) {};
};

/*********************  CLASS  **********************/
class CMRProjectAlias : public CMREntity
{
	public:
		CMRProjectAlias(CMREntity * entity, const std::string & alias,const std::string & longAlias) : CMREntity(entity->latexName,entity->longName) { this->real = entity; this->shortName = alias; this->latexEntity.name = alias; this->longName = longAlias;};
		virtual bool match(CMRLatexEntity& entity, CMRIndiceCaptureMap& capture) const
		{
			if (entity.name != shortName)
				return false;
			
			CMRLatexEntity tmp = entity;
			tmp.name = real->shortName;
			return real->match(tmp,capture);
		}
		virtual std::ostream& genUsageCCode(std::ostream& out, CMRProjectContext& context, CMRLatexEntity& entity) const {return real->genUsageCCode(out,context,entity);};
	private:
		CMREntity * real;
};

/*********************  CLASS  **********************/
class CMRProject
{
	public:
		CMREntityConstant & addConstant(const std::string& latexName, const std::string& longName);
		CMRProjectVariable & addvariable(const std::string& latexName, const std::string& longName, const std::string& type);
		CMRProjectAction & addAction(std::string name,std::string descr = "");
		CMRProjectDefinition & addDefinition(const std::string& latexName, const std::string& longName);
		CMRProjectDefinition& addDefinition( const std::string& latexName, const std::string& longName, const std::string& compute );
		CMRProjectIterator & addIterator(const std::string& latexName, const std::string& longName ,int start, int end);
		void replaceLoops(void);
		void genCCode(std::ostream & out);
		void printDebug(void);
	protected:
		void genCCodeOfConsts(std::ostream & out);
		void genCCodeOfVariables(std::ostream & out);
		void genCCodeOfDefinitions(std::ostream& out, CMRProjectContext& rootContext);
		void genCCodeOfActions(std::ostream& out, CMRProjectContext& rootContext);
		void buildRootContext(CMRProjectContext & context);
	private:
		CMRProjectConstantVector constants;
		CMRProjectVariableVector variables;
		CMRProjectActionVector actions;
		CMRProjectDefinitionVector definitions;
		CMRProjectIteratorVector iterators;
};

#endif //CMR_PROJECT_H
