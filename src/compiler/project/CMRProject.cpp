/********************  HEADERS  *********************/
#include "CMRProject.h"

/**********************  USING  *********************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
CMRProject2::CMRProject2(void)
{
	rootContext.addEntry(new CMRProjectLocalVariable("i","x","int"));
	rootContext.addEntry(new CMRProjectLocalVariable("j","y","int"));
	rootContext.addEntry(new CMRProjectLocalVariable("x","pos.getAbsX()","int"));
	rootContext.addEntry(new CMRProjectLocalVariable("y","pos.getAbsY()","int"));
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
ProjectAction& CMRProject2::addAction ( std::string name, std::string descr )
{
	ProjectAction * action = new ProjectAction(name,descr,&rootContext);
	actions.push_back(action);
	return *action;
}

/*******************  FUNCTION  *********************/
ProjectDefinition& CMRProject2::addDefinition ( const std::string& latexName, const std::string& longName )
{
	ProjectDefinition * def = new ProjectDefinition(latexName,longName,&rootContext);
	definitions.push_back(def);
	rootContext.addEntry(def);
	return *def;
}

/*******************  FUNCTION  *********************/
ProjectDefinition& CMRProject2::addDefinition ( const std::string& latexName, const std::string& longName, const std::string& compute )
{
	ProjectDefinition & def = addDefinition(latexName,longName);
	def.addEquation(latexName,compute);
	return def;
}

/*******************  FUNCTION  *********************/
ProjectIterator& CMRProject2::addIterator ( const std::string& latexName, const std::string& longName, int start, int end )
{
	ProjectIterator * it = new ProjectIterator(latexName,longName,start,end);
	iterators.push_back(it);
	rootContext.addEntry(it);
	return *it;
}

/*******************  FUNCTION  *********************/
ProjectMeshVar& CMRProject2::addvariable ( const std::string& latexName, const std::string& longName, const std::string& type )
{
	ProjectMeshVar * tmp = new ProjectMeshVar(latexName,longName,type);
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
	genCCodeOfActions(out,lang);
	genCCodeOfInit(out,lang);
	genCCodeOfMainLoop(out,lang);
	genCCodeOfMain(out,lang);
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
void CMRProject2::genCCodeOfActions ( ostream& out, CMRCompiler::LangDef & lang )
{
	if (actions.empty())
		return;

	out << "/*********************** ACTIONS *********************/" << endl;
	for (CMRProjectActionVector::iterator it = actions.begin() ; it != actions.end() ; ++it)
		(*it)->genDefinitionCCode(out,lang,rootContext);
	out << endl;
}

/*******************  FUNCTION  *********************/
CMRProjectCallAction& CMRProject2::addInitCallAction ( const string& actionName )
{
	CMRProjectCallAction * action = new CMRProjectCallAction(actionName);
	this->initActions.push_back(action);
	return *action;
}

/*******************  FUNCTION  *********************/
CMRProjectCallAction& CMRProject2::addMainLoopCallAction ( const string& actionName )
{
	CMRProjectCallAction * action = new CMRProjectCallAction(actionName);
	this->loopActions.push_back(action);
	return *action;
}

/*******************  FUNCTION  *********************/
void CMRProject2::genCCodeOfInit ( ostream& out, LangDef& lang )
{
	int cnt = 1;

	out << "/*******************  FUNCTION  *********************/" << endl;
	out << "void setup_init_state(VarSystem & sys,const CMRRect & global,const CMRRect & local)" << endl;
	out << "{" << endl;
	for (CMRProjectCallActionVector::const_iterator it = initActions.begin() ; it != initActions.end() ; ++it)
		(*it)->genCode(out,lang,cnt++);
	out << "}" << endl;
	out << endl;
}

/*******************  FUNCTION  *********************/
void CMRProject2::genCCodeOfMainLoop ( ostream& out, LangDef& lang )
{
	int cnt = 1;

	out << "/*******************  FUNCTION  *********************/" << endl;
	out << "void main_loop(VarSystem & sys,const CMRRect & global,const CMRRect & local)" << endl;
	out << "{" << endl;
	for (CMRProjectCallActionVector::const_iterator it = loopActions.begin() ; it != loopActions.end() ; ++it)
		(*it)->genCode(out,lang,cnt++);
	out << "}" << endl;
	out << endl;
}

/*******************  FUNCTION  *********************/
void CMRProject2::genCCodeOfMain ( ostream& out, LangDef& lang )
{
	out << "/*******************  FUNCTION  *********************/" << endl;
	out << "int main(void)" << endl;
	out << "{" << endl;
	out << "\t//init MPI" << endl;
	out << "\tMPI_Init(&argc,&argv);" << endl;
	out << "\tinfo_on_master(\"Start with np = %d\",cmrGetMPISize());" << endl;
	out << "\tMPI_Barrier(MPI_COMM_WORLD);" << endl << endl;
	
	out << "\t//try space splitter" << endl;
	out << "\tCMRRect globalRect(0,0,WIDTH,HEIGHT);" << endl;
	out << "\tCMRBasicSpaceSplitter splitter(globalRect,cmrGetMPISize(),0);" << endl;
	out << "\tsplitter.printDebug(CMR_MPI_MASTER);" << endl << endl;
	
	out << "\t//try system computation" << endl;
	out << "\tCMRMPIDomainBuilder builder(&splitter);" << endl;
	out << "\tVarSystem sys(&builder);" << endl << endl;
	
	out << "\t//get local domaine" << endl;
	out << "\tCMRRect localDomain = splitter.getLocalDomain(cmrGetMPIRank());" << endl << endl;
	
	out << "\t//init CURRENT and PREV" << endl;
	out << "\tsetup_init_state(sys,globalRect,localDomain);" << endl;
	out << "\tsys.permutVar(CMR_ALL);" << endl;
	out << "\tsetup_init_state(sys,globalRect,localDomain);" << endl;
	
	out << "\t//time steps" << endl;
	out << "\tfor ( int i = 1 ; i < ITERATIONS ; i++ )" << endl;
	out << "\t{" << endl;
	out << "\t\t//progression" << endl;
	out << "\t\tif (i % 10 == 0)" << endl;
	out << "\t\t\tinfo_on_master(\"Progress [%5d / %5d]\",i,ITERATIONS);" << endl;
	out << "\t\tmain_loop(sys,globalRect,localDomain);" << endl;
	out << "\t}" << endl;
	out << "}" << endl;
}

}
