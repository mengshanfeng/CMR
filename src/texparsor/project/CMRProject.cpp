/********************  HEADERS  *********************/
#include "CMRProject.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
CMRProject2::CMRProject2(void)
{
	rootContext.addEntry(new CMRProjectLocalVariable("i","x","int"));
	rootContext.addEntry(new CMRProjectLocalVariable("j","y","int"));
}

/*******************  FUNCTION  *********************/
CMRProjectConstant& CMRProject2::addConstant ( const std::string& latexName, const std::string& longName )
{
	CMRProjectConstant * tmp = new CMRProjectConstant(latexName,longName);
	constants.push_back(tmp);
	rootContext.addEntry(tmp);
	return *tmp;
}

/*******************  FUNCTION  *********************/
CMRProjectAction& CMRProject2::addAction ( std::string name, std::string descr )
{
	CMRProjectAction * action = new CMRProjectAction(name,descr,&rootContext);
	actions.push_back(action);
	return *action;
}

/*******************  FUNCTION  *********************/
CMRProjectDefinition& CMRProject2::addDefinition ( const std::string& latexName, const std::string& longName )
{
	CMRProjectDefinition * def = new CMRProjectDefinition(latexName,longName,&rootContext);
	definitions.push_back(def);
	rootContext.addEntry(def);
	return *def;
}

/*******************  FUNCTION  *********************/
CMRProjectDefinition& CMRProject2::addDefinition ( const std::string& latexName, const std::string& longName, const std::string& compute )
{
	CMRProjectDefinition & def = addDefinition(latexName,longName);
	def.addEquation(latexName,compute);
	return def;
}

/*******************  FUNCTION  *********************/
CMRProjectIterator& CMRProject2::addIterator ( const std::string& latexName, const std::string& longName, int start, int end )
{
	CMRProjectIterator * it = new CMRProjectIterator(latexName,longName,start,end);
	iterators.push_back(it);
	rootContext.addEntry(it);
	return *it;
}

/*******************  FUNCTION  *********************/
CMRProjectMeshVar& CMRProject2::addvariable ( const std::string& latexName, const std::string& longName, const std::string& type )
{
	CMRProjectMeshVar * tmp = new CMRProjectMeshVar(latexName,longName,type);
	variables.push_back(tmp);
	rootContext.addEntry(tmp);
	//TODO create alias
	//rootContext.addEntry(new CMRProjectAlias(tmp,tmp->shortName + "'",tmp->longName + "Prim"));
	return *tmp;
}

/*******************  FUNCTION  *********************/
void CMRProject2::printDebug ( void )
{
	//TODO
}

/*******************  FUNCTION  *********************/
void CMRProject2::runTransformation ( CMRTransformation& transf )
{
	for (CMRProjectActionVector::iterator it = actions.begin() ; it != actions.end() ; ++it)
		(*it)->runTransformation(transf);
	for (CMRProjectDefinitionVector::iterator it = definitions.begin() ; it != definitions.end() ; ++it)
		(*it)->runTransformation(transf);
}

/*******************  FUNCTION  *********************/
void CMRProject2::genCCode ( std::ostream& out )
{
	genCCodeOfConsts(out);
	genCCodeOfVariables(out);
	genCCodeOfDefinitions(out);
	genCCodeOfActions(out);
}

/*******************  FUNCTION  *********************/
void CMRProject2::genCCodeOfConsts ( std::ostream& out )
{
	//nothing to do
	if (constants.empty())
			return;

	//gen consts
	out << "/************************ CONSTS **********************/" << endl;
	for (CMRProjectConstantVector::iterator it = constants.begin() ; it != constants.end() ; ++it)
		(*it)->genDefinitionCCode(out,rootContext);
	out << endl;
}

/*******************  FUNCTION  *********************/
void CMRProject2::genCCodeOfVariables ( ostream& out )
{
	//nothing to do
	if (variables.empty())
		return;

	//build the variable system
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
void CMRProject2::genCCodeOfDefinitions ( ostream& out )
{
	if (definitions.empty())
		return;
	out << "/********************* DEFINITIONS ********************/" << endl;
	for (CMRProjectDefinitionVector::iterator it = definitions.begin() ; it != definitions.end() ; ++it)
		(*it)->genDefinitionCCode(out,rootContext);
	out << endl;
}

/*******************  FUNCTION  *********************/
void CMRProject2::genCCodeOfActions ( ostream& out )
{
	if (actions.empty())
		return;

	out << "/*********************** ACTIONS *********************/" << endl;
	for (CMRProjectActionVector::iterator it = actions.begin() ; it != actions.end() ; ++it)
		(*it)->genDefinitionCCode(out,rootContext);
	out << endl;
}
