/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cassert>
#include <iostream>
#include <sstream>
#include "CMRProjectVariable.h"

using namespace std;

/*******************  FUNCTION  *********************/
CMRProjectVariable::CMRProjectVariable ( const string& latexName, const string& longName , const std::string & type) 
	: CMREntity ( latexName, longName )
{
	//check that we have i/j for capture
	assert(haveCaptureFor("i"));
	assert(haveCaptureFor("j"));
	assert(type.empty() == false);
	ghostDepths = 1;
	memoryModel = "CMRMemoryModelColMajor";
	this->type = type;
}

/*******************  FUNCTION  *********************/
void CMRProjectVariable::addDim ( int size, const string& name,int start )
{
	assert(size > 0);
	assert(name.empty() == false);
	dims.push_back(size);
	dimNames.push_back(name);
	dimStart.push_back(start);
	madeCaptureIndice(name,CMR_CAPTURE_OPTIONS);
}

/*******************  FUNCTION  *********************/
ostream& CMRProjectVariable::genCPPAccessorDefinition(ostream& out)
{
	out << "\t\t\tCMRCellAccessor<" << type;
	for (int i = 0 ; i < dims.size() ; i++)
		out << "[" << dims[i] << "]";
	out << ",CMRMemoryModelRowMajor> " << longName << ";" << endl;
	return out;
}

/*******************  FUNCTION  *********************/
ostream& CMRProjectVariable::genCPPAccessorAddVar(ostream& out)
{
	//definition
	out << "\t//define variable " << latexName << endl;
	out << "\tthis->addVariable(\"" << longName << "\",sizeof(" << getTypeWithDims() << ")," << ghostDepths << ");" << endl;
	return out;
}

/*******************  FUNCTION  *********************/
ostream& CMRProjectVariable::genCPPAccessorConstrSys(ostream& out,int id)
{
	out << longName << "*(sys.getDomain(" << id << ",tstep)),x,y,absolute)" << endl;
	return out;
}

/*******************  FUNCTION  *********************/
ostream& CMRProjectVariable::genCPPAccessorConstrAcc(ostream& out)
{
	out << longName << "(acc." << longName << ",x,y,absolute)" << endl;
	return out;
}

/*******************  FUNCTION  *********************/
void CMRProjectVariable::printCPPCode ( void ) const
{
	//definition
	cout << "//define variable " << latexName << endl;
	cout << "this->addVariable(\"" << longName << "\",sizeof(" << getTypeWithDims() << ")," << ghostDepths << ");" << endl;
	
	//accessor member definition
	cout << "//define variable " << latexName << endl;
	cout << "CMRCellAccessor<" << getTypeWithDims() << "," << memoryModel << "> " << longName << endl;
	
	//build local accessor from domain
	cout << longName << "*(sys.getDomain(0,tstep)),x,y,absolute)" << endl;
	
	//build local accessor from parent accessor
	cout << longName << "(acc.directions,x,y,absolute)" << endl;
	
	cout << endl;
}

/*******************  FUNCTION  *********************/
string CMRProjectVariable::getTypeWithDims ( void ) const
{
	stringstream res;
	res << type;
	for (int i = 0 ; i < dims.size() ; i++)
		res << "[" << dims[i] << "]";
	return res.str();
}