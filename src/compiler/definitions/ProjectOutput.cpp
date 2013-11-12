/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "definitions/GenCode.h"
#include "common/Debug.h"
#include "ProjectOutput.h"

/**********************  USING  *********************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
ProjectOutputEntry::ProjectOutputEntry ( const std::string& name, const std::string& type, const std::string& value )
{
	this->name = name;
	this->type = type;
	this->value = new LatexFormulas(value);
}

/*******************  FUNCTION  *********************/
void ProjectOutputEntry::genCode ( std::ostream& out , const CMRProjectContext & context, const std::string & codeType)
{
	if (codeType == "DeclStruct")
	{
		out << "\t" << type << " " << name << ";" << endl;
	} else if (codeType == "ImplAction") {
		out << "\t\t" << name << " = ";
		cmrGenEqCCode(out,context,*value) << ";" << endl; 
	} else {
		cmrFatal("Invalid code type for project output : %s",codeType.c_str());
	}
}

/*******************  FUNCTION  *********************/
void ProjectOutput::addEntry ( const std::string& name, const std::string& type, const std::string& value )
{
	//TODO check name conflict, maybe use a map instead of vector
	this->entries.push_back(ProjectOutputEntry(name,type,value));
}

/*******************  FUNCTION  *********************/
void ProjectOutput::genCode ( std::ostream& out ,  const CMRProjectContext & context, const std::string & codeType)
{
	if (codeType == "DeclStruct")
	{
		out << "struct LBMFileEntry" << endl;
		out << "{" << endl;
		for (ProjectOutputEntryVector::iterator it = entries.begin() ; it != entries.end() ; ++it)
			it->genCode(out, context,codeType);
		out << "};" << endl << endl;
	} else if (codeType == "ImplAction") {
		out << "/*******************  FUNCTION  *********************/" << endl;
		out << "struct ActionUpdateFileout" << endl;
		out << "{" << endl;
		out << "\tvoid cellAction(VarSystem::CellAccessor & cell,int x,int y) const" << endl;
		out << "\t{" << endl;
		for (ProjectOutputEntryVector::iterator it = entries.begin() ; it != entries.end() ; ++it)
			it->genCode(out, context,codeType);
		out << "\t}" << endl;
		out << "\t//select the type of loop" << endl;
		out << "\ttypedef CMRMeshOperationSimpleLoopInPlace<VarSystem,ActionUpdateFileout> LoopType;" << endl;
		out << "};" << endl << endl;
	} else {
		cmrFatal("Invalid code type for project output : %s",codeType.c_str());
	}
}

}
