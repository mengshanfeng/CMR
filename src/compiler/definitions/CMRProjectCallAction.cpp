/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "CMRProjectCallAction.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
CMRProjectCallAction::CMRProjectCallAction ( const std::string& actionName )
{
	this->actionName = actionName;
}

/*******************  FUNCTION  *********************/
CMRProjectCallAction& CMRProjectCallAction::addParameter ( const std::string& name, const std::string& value )
{
	this->parameters[name] = value;
	return *this;
}

/*******************  FUNCTION  *********************/
CMRProjectCallAction& CMRProjectCallAction::addZone ( const std::string& zone )
{
	this->zones.push_back(zone);
	return *this;
}

/*******************  FUNCTION  *********************/
void CMRProjectCallAction::genCode ( std::ostream& out, CMRCompiler::LangDef& lang, int id ) const
{
	out << "Action" << actionName << " * action" << id << " = new Action" << actionName << ";" << endl;
	
	for (CMRCallParameters::const_iterator it = parameters.begin() ; it != parameters.end() ; ++it)
		out << "action" << id << "->" << it->first << " = " << it->second << endl;
	
	out << "CMRMeshOperationSimpleLoopInPlace<VarSystem,Action" << actionName << "> step" << id << "(&sys,action" << id << ");" << endl;
	
	for (CMRCallZoneVector::const_iterator it = zones.begin() ; it != zones.end() ; ++it)
		out << "step" << id << ".run(" << *it << ");" << endl;
}
