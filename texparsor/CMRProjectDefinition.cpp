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
#include "CMRProjectDefinition.h"
#include "CMRProjectTransformation.h"

using namespace std;

/*******************  FUNCTION  *********************/
CMRProjectDefinition::CMRProjectDefinition(const std::string& latexName, const std::string& longName)
	:CMREntity(latexName,longName), ops("body","body")
{

}

/*******************  FUNCTION  *********************/
void CMRProjectDefinition::printDebug(void ) const
{
	cout << "Definition : " << this->latexName << " : " << longName << endl;
	ops.printDebug(0);
}

/*******************  FUNCTION  *********************/
void CMRProjectDefinition::printCPPCode(CMRProjectContext & context) const
{
	cout << "//Definition : " << this->latexName << " : " << longName << endl;
	cout << "double compute_" << this->longName << "(const VarSystem::CellAccessor & in,VarSystem::CellAccessor & out,int x,int y)" << endl;
	cout << "{" << endl;
	ops.genCCode(cout,context,1);
	cout << "\t\treturn " << this->longName << endl;
	cout << "}" << endl;
	cout << endl;
}

/*******************  FUNCTION  *********************/
ostream& CMRProjectDefinition::genUsageCCode ( ostream& out, CMRProjectContext& context, CMRLatexEntity& entity ) const
{
	out << "compute_" << this->longName << "(in,out,x,y)";
	return out;
}

/*******************  FUNCTION  *********************/
CMRProjectEquation& CMRProjectDefinition::addEquation(const string& latexName, const string& longName, const string& compute, CMRProjectCodeTreeInsert location)
{
	string tmp ;
	if (latexName == this->latexName)
		tmp = "\\CMRTMP^1000000000{definition}";
	else
		tmp = latexName;
	return ops.addEquation(tmp,longName,compute,location);
}

/*******************  FUNCTION  *********************/
CMRProjectAction& CMRProjectDefinition::addSubBlock(string loopDescr, string parameter, CMRProjectCodeTreeInsert location)
{
	return ops.addSubBlock(loopDescr,parameter,location);
}

/*******************  FUNCTION  *********************/
void CMRProjectDefinition::runTransformation ( CMRProjectTransformation& transf )
{
	transf.run(&ops);
}
