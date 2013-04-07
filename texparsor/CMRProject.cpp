/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <iostream>
#include "CMRProject.h"
#include "CMRProjectConstant.h"
#include "CMRProjectVariable.h"
#include "CMRProjectAction.h"
#include "CMRProjectDefinition.h"
#include "CMRProjectIterator.h"
#include "CMRProjectTransformation.h"

using namespace std;

/*******************  FUNCTION  *********************/
CMREntityConstant& CMRProject::addConstant ( const string& latexName, const string& longName )
{
	CMREntityConstant * tmp = new CMREntityConstant(latexName,longName);
	constants.push_back(tmp);
	return *tmp;
}

/*******************  FUNCTION  *********************/
CMRProjectVariable& CMRProject::addvariable ( const string& latexName, const string& longName, const string& type )
{
	CMRProjectVariable * tmp = new CMRProjectVariable(latexName,longName,type);
	variables.push_back(tmp);
	return *tmp;
}

/*******************  FUNCTION  *********************/
CMRProjectAction& CMRProject::addAction ( string name, string descr )
{
	CMRProjectAction * action = new CMRProjectAction(name,descr);
	actions.push_back(action);
	return *action;
}

/*******************  FUNCTION  *********************/
void CMRProject::replaceLoops(void )
{
	CMRProjectTransfExtractLoops t;
	runTransformation(t);
}

/*******************  FUNCTION  *********************/
void CMRProject::insertImplicitMul(void )
{
	CMRProjectTransfImplicitMul t;
	runTransformation(t);
}

/*******************  FUNCTION  *********************/
void CMRProject::runTransformation ( CMRProjectTransformation& transf )
{
	for (CMRProjectActionVector::iterator it = actions.begin() ; it != actions.end() ; ++it)
		transf.run((*it));
	for (CMRProjectDefinitionVector::iterator it = definitions.begin() ; it != definitions.end() ; ++it)
		(*it)->runTransformation(transf);
}

/*******************  FUNCTION  *********************/
void CMRProject::printDebug(void )
{
	cout << "=============================================" << endl;
	for (CMRProjectActionVector::iterator it = actions.begin() ; it != actions.end() ; ++it)
	{
		cout << "Actions [" << (*it)->getName() << " , " << (*it)->getDescription() << "] : " << endl;
		(*it)->printDebug(1);
	}
	cout << "=============================================" << endl;
}

/*******************  FUNCTION  *********************/
void CMRProject::genCCodeOfConsts(ostream& out)
{
	if (constants.empty())
		return;

	out << "/************************ CONSTS **********************/" << endl;
	for (CMRProjectConstantVector::iterator it = constants.begin() ; it != constants.end() ; ++it)
		(*it)->printCPPCode();
}

/*******************  FUNCTION  *********************/
void CMRProject::genCCodeOfVariables(ostream& out)
{
	if (variables.empty())
		return;

	out << "/********************** VARIABLES *********************/" << endl;
	out << "class VarSystem : public CMRVarSystem" << endl;
	out << "{" << endl;
	out << "\tpublic:" << endl;
	out << "\t\tstruct CellAccessor" << endl;
	out << "\t\t{" << endl;
	out << "\t\t\tCellAccessor(CMRVarSystem & sys,int tstep,int x,int y,bool absolute = true);" << endl;
	out << "\t\t\tCellAccessor(CellAccessor & acc,int x,int y,bool absolute = false);" << endl;
	
	for (CMRProjectVariableVector::iterator it = variables.begin() ; it != variables.end() ; ++it)
		(*it)->genCPPAccessorDefinition(out);

	out << "\t\t};" << endl;
	out << "\tpublic:" << endl;
	out << "\t\tVarSystem(CMRDomainBuilder * builder);" << endl;
	out << "};" << endl;
	out << endl;

	out << "VarSystem::VarSystem ( CMRDomainBuilder * builder)" << endl;
	out << "\t:CMRVarSystem(builder)" << endl;
	out << "{" << endl;
	
	for (CMRProjectVariableVector::iterator it = variables.begin() ; it != variables.end() ; ++it)
		(*it)->genCPPAccessorAddVar(out);

	out << "}" << endl;
	out << endl;
	out << "VarSystem::CellAccessor::CellAccessor (CMRVarSystem & sys,int tstep,int x,int y,bool absolute)" << endl;
	
	int cnt = 0;
	for (CMRProjectVariableVector::iterator it = variables.begin() ; it != variables.end() ; ++it)
	{
		if (cnt == 0)
			out << "\t:";
		else
			out << "\t,";
		(*it)->genCPPAccessorConstrSys(out,cnt++);
		out << endl;
	}

	out << "{}" << endl;
	out << endl;
	out << "VarSystem::CellAccessor::CellAccessor ( CellAccessor& acc, int x, int y,bool absolute)" << endl;
	
	cnt = 0;
	for (CMRProjectVariableVector::iterator it = variables.begin() ; it != variables.end() ; ++it)
	{
		if (cnt == 0)
			out << "\t:";
		else
			out << "\t,";
		(*it)->genCPPAccessorConstrAcc(out);
		out << endl;
	}
	
	out << "{}" << endl;
	out << endl;	
}

/*******************  FUNCTION  *********************/
void CMRProject::genCCodeOfDefinitions(ostream& out,CMRProjectContext & rootContext)
{
	if (definitions.empty())
		return;
	out << "/********************* DEFINITIONS ********************/" << endl;
	for (CMRProjectDefinitionVector::iterator it = definitions.begin() ; it != definitions.end() ; ++it)
		(*it)->printCPPCode(rootContext);
}

/*******************  FUNCTION  *********************/
void CMRProject::genCCodeOfActions(ostream& out,CMRProjectContext & rootContext)
{
	out << "/*********************** ACTIONS *********************/" << endl;
	for (CMRProjectActionVector::iterator it = actions.begin() ; it != actions.end() ; ++it)
	{
		out << "struct Action" << (*it)->getName() << endl;
		out << "{" << endl;
		out << "\tstatic void cellAction(const VarSystem::CellAccessor & in,VarSystem::CellAccessor& out,const CMRCellPosition & pos,int x,int y)" << endl;
		out << "\t{" << endl;
		(*it)->genCCode(out,rootContext,1);
		out << "\t}" << endl;
		out << "};" << endl;
		out << endl;
	}
}

/*******************  FUNCTION  *********************/
void CMRProject::buildRootContext(CMRProjectContext& context)
{
	for (CMRProjectIteratorVector::iterator it = iterators.begin() ; it != iterators.end() ; ++it)
		context.addEntry(*it);
	for (CMRProjectConstantVector::iterator it = constants.begin() ; it != constants.end() ; ++it)
		context.addEntry(*it);
	for (CMRProjectDefinitionVector::iterator it = definitions.begin() ; it != definitions.end() ; ++it)
		context.addEntry(*it);
	for (CMRProjectVariableVector::iterator it = variables.begin() ; it != variables.end() ; ++it)
	{
		context.addEntry(*it);
		context.addEntry(new CMRProjectAlias(*it,(*it)->shortName + "'",(*it)->longName + "Prim"));
	}
	context.addEntry(new CMRProjectLocalVariable("i","x"));
	context.addEntry(new CMRProjectLocalVariable("j","y"));
}

/*******************  FUNCTION  *********************/
void CMRProject::genCCode(ostream& out)
{
	//gen root context
	CMRProjectContext rootContext(NULL);
	buildRootContext(rootContext);
	
	genCCodeOfConsts(out);
	genCCodeOfVariables(out);
	genCCodeOfDefinitions(out,rootContext);
	genCCodeOfActions(out,rootContext);
}

/*******************  FUNCTION  *********************/
CMRProjectDefinition & CMRProject::addDefinition(const string& latexName, const string& longName)
{
	CMRProjectDefinition * def = new CMRProjectDefinition(latexName,longName);
	definitions.push_back(def);
	return *def;
}

/*******************  FUNCTION  *********************/
CMRProjectDefinition& CMRProject::addDefinition(const string& latexName, const string& longName, const string& compute)
{
	CMRProjectDefinition & def = addDefinition(latexName,longName);
	def.addEquation(latexName,longName,compute);
	return def;
}

/*******************  FUNCTION  *********************/
CMRProjectIterator& CMRProject::addIterator(const string& latexName, const string& longName, int start, int end)
{
	CMRProjectIterator * it = new CMRProjectIterator(latexName,longName,start,end);
	iterators.push_back(it);
	return *it;
}
