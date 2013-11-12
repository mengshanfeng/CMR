/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "common/Common.h"
#include "CMRProjectCallAction.h"

/**********************  USING  *********************/
using namespace std;


/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

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
void CMRProjectCallAction::genCode ( ostream& out, CMRCompiler::LangDef& lang, int id, int indent ) const
{
	//ideally we may generate :
	//app.addInitAction(new ActionInitCellType::LoopType(new ActionInitCellType(CELL_LEFT_IN)),
	//				  app.getLocalRect().expended(1).getBorder(CMR_LEFT));
	
	if (parameters.empty() == false)
	{
		doIndent(out,indent) << "Action" << actionName << " * action" << id << " = new Action" << actionName << "();" << endl;
		
		for (CMRCallParameters::const_iterator it = parameters.begin() ; it != parameters.end() ; ++it)
			doIndent(out,indent) << "action" << id << "->" << it->first << " = " << it->second << endl;
	}
	
	for (CMRCallZoneVector::const_iterator it = zones.begin() ; it != zones.end() ; ++it)
	{
		if (parameters.empty())
			doIndent(out,indent) << "app.addInitAction(new Action" << actionName << "::LoopType(new Action" << actionName << "())),";
		else
			doIndent(out,indent) << "app.addInitAction(new Action" << actionName << "::LoopType(action" << id << ")),";
		doIndent(out,indent) << *it << ");" << endl;
	}
}

}
