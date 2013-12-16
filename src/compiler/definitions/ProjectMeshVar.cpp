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
#include "common/Debug.h"
#include "ProjectMeshVar.h"
#include "ProjectContext.h"

/**********************  USING  *********************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
ProjectMeshVar::ProjectMeshVar ( const string& latexName, const string& longName, const string& type, int ghost ) 
	: ProjectEntityNew ( latexName, longName )
{
	//setup capture
	markForCapture("i");
	markForCapture("j");
	
	//check that we have i/j for capture
	assert(type.empty() == false);
	assert(ghost >= 0);
	
	ghostDepths = ghost;
	memoryModel = "CMRMemoryModelColMajor";
	this->type = type;
}

/*******************  FUNCTION  *********************/
void ProjectMeshVar::addDim ( const string& name,int size,int start )
{
	assert(size > 0);
	assert(name.empty() == false);
	defs.push_back(ProjectMeshVarDef(name,size,start));
	markForCapture(name);
}

/*******************  FUNCTION  *********************/
ostream& ProjectMeshVar::genCPPAccessorDefinition(ostream& out)
{
	out << "\t\t\tCMRCellAccessor<" << type;
	for (int i = 0 ; i < defs.size() ; i++)
		out << "[" << defs[i].dims << "]";
	out << "," << memoryModel << "> " << getLongName() << ";" << endl;
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
	out << getLongName() << "(*(sys.getDomain(" << id << ",tstep)),x,y,absolute)" << endl;
	return out;
}

/*******************  FUNCTION  *********************/
ostream& ProjectMeshVar::genCPPAccessorConstrAcc(ostream& out)
{
	out << getLongName() << "(acc." << getLongName() << ",x,y,absolute)" << endl;
	return out;
}

/*******************  FUNCTION  *********************/
void ProjectMeshVar::genDefinitionCCode ( ostream& out, const ProjectContext& context, int indent ) const
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
void ProjectMeshVar::genUsageCCode( ostream& out, const ProjectContext& context, const LatexEntity& entity, bool write ) const
{
	FormulaMatcherResult capture;
	const std::string & loopType = context.readKey("CMRActionLoopType");
	
	//select out mode
	if (loopType == "CMRMeshOperationSimpleLoop" || loopType == "CMRMeshOperationSimpleLoopWithPos" || loopType.empty())
	{
		if (write)
			out << "(*out.";
		else
			out << "(*in.";
	} else if (loopType == "CMRMeshOperationSimpleLoopInPlace" || loopType == "CMRMeshOperationSimpleLoopInPlaceWithPos") {
		out << "(*cell.";
	} else {
		cmrFatal("Invalid action loop type : %s",loopType.c_str());
	}

	//tmp.name = shortName;
	this->capture(entity,capture);
	
	out << getLongName() << "( ";
	cmrGenEqCCode(out,context,*capture.captures["i"]) << ", ";
	cmrGenEqCCode(out,context,*capture.captures["j"]) << "))";
	
	for (ProjectMeshVarDefVector::const_iterator it = defs.begin() ; it != defs.end() ; ++it)
	{
		if ( it->name != "i" && it->name != "j")
		{
			out << "[ ";
			cmrGenEqCCode(out,context,*capture.captures[it->name]) << "]";
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

/*******************  FUNCTION  *********************/
void ProjectMeshVar::setMemoryModel ( const string& value )
{
	this->memoryModel = value;
}

}
