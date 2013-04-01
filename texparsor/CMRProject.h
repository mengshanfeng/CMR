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

/*********************  TYPES  **********************/
class CMREntityConstant;
class CMRProjectVariable;
class CMRProjectIterator;
class CMRProjectAction;
class CMRProjectEquation;

/*********************  TYPES  **********************/
typedef std::vector <CMREntityConstant*> CMRProjectConstantVector;
typedef std::vector <CMRProjectVariable*> CMRProjectVariableVector;
typedef std::vector <CMRProjectIterator*> CMRProjectIteratorVector;
typedef std::vector <CMRProjectAction *> CMRProjectActionVector;
typedef std::vector <CMRProjectEquation*> CMRProjectEquationVector;

/*********************  CLASS  **********************/
class CMRProject
{
	public:
		CMRProject() :rootContext(NULL) {};
		CMREntityConstant & addConstant(const std::string& latexName, const std::string& longName);
		CMRProjectVariable & addvariable(const std::string& latexName, const std::string& longName, const std::string& type);
		CMRProjectAction & addAction(std::string name,std::string descr = "");
	private:
		CMRProjectConstantVector constants;
		CMRProjectVariableVector variables;
		CMRProjectActionVector actions;
		CMRProjectContext rootContext;
};

#endif //CMR_PROJECT_H
