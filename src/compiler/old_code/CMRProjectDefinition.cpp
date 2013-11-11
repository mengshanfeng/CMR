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
#include "CMRProjectDefinition.h"
#include "CMRProjectTransformation.h"
#include "CMRProject.h"
#include "CMRGenCode.h"

using namespace std;
using namespace CMRCompiler;

/*******************  FUNCTION  *********************/
CMRProjectDefinition::CMRProjectDefinition(const std::string& latexName, const std::string& longName,CMRProjectContext * parentContext)
	:CMRProjectEntity(latexName,longName), ops("body","body",parentContext)
{
	this->ops.addContextEntry(new CMRProjectLocalVariable(latexName,"tmp"));
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
	cout << "double compute_" << this->longName << "(const VarSystem::CellAccessor & in,VarSystem::CellAccessor & out,int x,int y";
	
	for (int i = 0 ; i < indicesCapture.size() ; i++)
	{
		assert(indicesCapture[i] != CMR_CAPTURE_OPTIONS);
		if (indicesCapture[i] == CMR_CAPTURE_REQUIRED && indices[i] != "i" && indices[i] != "j")
			cout << ", int " << indices[i];
	}
	
	cout << ")" << endl;
	cout << "{" << endl;
	ops.genCCode(cout,context,1);
	cout << "\t\treturn " << this->longName << endl;
	cout << "}" << endl;
	cout << endl;
}

/*******************  FUNCTION  *********************/
ostream& CMRProjectDefinition::genUsageCCode ( ostream& out, const CMRProjectContext& context, LatexEntityOld& entity ) const
{
	//extract matching
	CMRProjectCaptureMap capture;
	
	//extract matching
	bool res = this->match(entity,capture);
	assert(res == true);
	
	out << "compute_" << this->longName << "(in,out,x,y";
	for (int i = 0 ; i < indicesCapture.size() ; i++)
	{
		assert(indicesCapture[i] != CMR_CAPTURE_OPTIONS);
		
		if (indicesCapture[i] == CMR_CAPTURE_REQUIRED && indices[i] != "i" && indices[i] != "j")
		{
			out << ",";
			assert(capture[indices[i]] != NULL);
			cmrGenEqCCode(out,context, *capture[indices[i]]);
		}
	}
	out << ");";
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

/*******************  FUNCTION  *********************/
void CMRProjectDefinition::addIndice ( const string& name, CMRCaptureType capture )
{
	CMRProjectEntity::addIndice ( name, capture );
	this->ops.addContextEntry(new CMRProjectLocalVariable(name,"tmpvariable"));
}

/*******************  FUNCTION  *********************/
void CMRProjectDefinition::madeCaptureIndice ( const string name, CMRCaptureType capture )
{
	CMRProjectEntity::madeCaptureIndice ( name, capture );
	this->ops.addContextEntry(new CMRProjectLocalVariable(name,"tmpvariable"));
}
