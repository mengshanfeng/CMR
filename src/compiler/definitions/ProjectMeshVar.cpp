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
#include <sstream>
#include "GenCode.h"
#include "ProjectMeshVar.h"

/**********************  USING  *********************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
ProjectMeshVar::ProjectMeshVar ( const string& latexName, const string& longName , const std::string & type) 
	: CMRProjectEntity ( latexName, longName )
{
	//setup capture
	changeCaptureType("i",CMR_CAPTURE_REQUIRED);
	changeCaptureType("j",CMR_CAPTURE_REQUIRED);
	
	//check that we have i/j for capture
	assert(haveCapture("i"));
	assert(haveCapture("j"));
	assert(type.empty() == false);
	ghostDepths = 1;
	memoryModel = "CMRMemoryModelColMajor";
	this->type = type;
}

/*******************  FUNCTION  *********************/
void ProjectMeshVar::addDim ( const string& name,int size,int start )
{
	assert(size > 0);
	assert(name.empty() == false);
	defs.push_back(ProjectMeshVarDef(name,size,start));
	changeCaptureType(name,CMR_CAPTURE_REQUIRED);
}

/*******************  FUNCTION  *********************/
ostream& ProjectMeshVar::genCPPAccessorDefinition(ostream& out)
{
	out << "\t\t\tCMRCellAccessor<" << type;
	for (int i = 0 ; i < defs.size() ; i++)
		out << "[" << defs[i].dims << "]";
	out << ",CMRMemoryModelRowMajor> " << getLongName() << ";" << endl;
	return out;
}

/*******************  FUNCTION  *********************/
ostream& ProjectMeshVar::genCPPAccessorAddVar(ostream& out)
{
	//definition
	out << "\t//define variable " << getLatexName() << endl;
	out << "\tthis->addVariable(\"" << getLongName() << "\",sizeof(" << getTypeWithDims() << ")," << ghostDepths << ");" << endl;
	return out;
}

/*******************  FUNCTION  *********************/
ostream& ProjectMeshVar::genCPPAccessorConstrSys(ostream& out,int id)
{
	out << getLongName() << "*(sys.getDomain(" << id << ",tstep)),x,y,absolute)" << endl;
	return out;
}

/*******************  FUNCTION  *********************/
ostream& ProjectMeshVar::genCPPAccessorConstrAcc(ostream& out)
{
	out << getLongName() << "(acc." << getLongName() << ",x,y,absolute)" << endl;
	return out;
}

/*******************  FUNCTION  *********************/
void ProjectMeshVar::genDefinitionCCode ( ostream& out, const CMRProjectContext& context, int indent ) const
{
	throw LatexException("Variable must be defined by calling special functions from project structure, not directly by genDefinitionCCode function.");
}

/*******************  FUNCTION  *********************/
string ProjectMeshVar::getTypeWithDims ( void ) const
{
	stringstream res;
	res << type;
	for (int i = 0 ; i < defs.size() ; i++)
		res << "[" << defs[i].dims << "]";
	return res.str();
}

/*******************  FUNCTION  *********************/
void ProjectMeshVar::genUsageCCode( ostream& out, const CMRProjectContext& context, const LatexEntity& entity, bool write ) const
{
	CMRProjectCaptureMap capture;

	//select out mode
	if (write)
		out << "out.";
	else
		out << "in.";

	//tmp.name = shortName;
	this->capture(entity,capture);
	
	out << getLongName() << "( ";
	cmrGenEqCCode(out,context,*capture["i"]) << ", ";
	cmrGenEqCCode(out,context,*capture["j"]) << ")";
	
	CMRStringVector indices = getCapturedIndices();
	for (CMRStringVector::iterator it = indices.begin() ; it != indices.end() ; ++it)
	{
		if ( *it != "i" && *it != "j")
		{
			out << "[ ";
			cmrGenEqCCode(out,context,*capture[*it]) << "]";
		}
	}
}

/*******************  FUNCTION  *********************/
ProjectMeshVarDef::ProjectMeshVarDef ( const string& name, int dims, int start )
{
	this->name = name;
	this->dims = dims;
	this->start = start;
}

}
