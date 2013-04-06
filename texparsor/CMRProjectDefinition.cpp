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
void CMRProjectDefinition::printCPPCode(void ) const
{
	cout << "Definition : " << this->latexName << " : " << longName << endl;
	ops.genCCode(cout,0);
}

/*******************  FUNCTION  *********************/
CMRProjectEquation& CMRProjectDefinition::addEquation(const string& latexName, const string& longName, const string& compute, CMRProjectCodeTreeInsert location)
{
	return ops.addEquation(latexName,longName,compute,location);
}

/*******************  FUNCTION  *********************/
CMRProjectAction& CMRProjectDefinition::addSubBlock(string loopDescr, string parameter, CMRProjectCodeTreeInsert location)
{
	return ops.addSubBlock(loopDescr,parameter,location);
}
