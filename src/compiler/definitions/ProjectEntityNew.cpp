/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "ProjectEntityNew.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
ProjectEntityNew::ProjectEntityNew ( const std::string& latexName, const std::string& longName )
	:FormulaMatcher(latexName,false),longName(longName)
{
	this->setOptionalExponent(false);
}

/*******************  FUNCTION  *********************/
ProjectEntityNew::~ProjectEntityNew ( void )
{
}

/*******************  FUNCTION  *********************/
std::string ProjectEntityNew::getLatexName ( void ) const
{
	return this->toString();
}

/*******************  FUNCTION  *********************/
const std::string& ProjectEntityNew::getLongName ( void ) const
{
	return this->longName;
}

/*******************  FUNCTION  *********************/
void ProjectEntityNew::printDebug ( std::ostream& out ) const
{
	FormulaMatcher::printDebug(out);
}

/*******************  FUNCTION  *********************/
bool ProjectEntityNew::match ( const LatexEntity& entity ) const
{
	#warning Maybe remove
	return FormulaMatcher::match(entity);
}


}