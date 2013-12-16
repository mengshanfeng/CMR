/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "ProjectContext.h"
#include "ProjectIterator.h"

/**********************  USING  *********************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
ProjectIterator::ProjectIterator ( const string& latexName, const string& longName, int start, int end ) 
	: ProjectEntityOld ( latexName, longName )
{
	this->start = start;
	this->end = end;
}

/*******************  FUNCTION  *********************/
void ProjectIterator::printDebug ( std::ostream & out ) const
{
	ProjectEntityOld::printDebug(out);
	out << "    - values     : "<< start << " .. " << end << endl;
}

/*******************  FUNCTION  *********************/
void ProjectIterator::genDefinitionCCode ( ostream& out, const ProjectContext& context ,int indent ) const
{
	//TODO indent
	out << "int " << getLongName() << " = " << start << " ; " << getLongName() << " <= " << end << " ; " << getLongName() << "++";
}

/*******************  FUNCTION  *********************/
void ProjectIterator::genUsageCCode ( ostream& out, const ProjectContext& context, const LatexEntity& entity, bool write ) const
{
// 	stringstream err;
// 	err << "Error, iterator may not be used out of loop definition : " << getLatexName() << " : " << start << " -> " << end << endl;
// 	context.printDebug(err);
// 	throw CMRLatexException(err.str());
	out << getLongName();
}

}
