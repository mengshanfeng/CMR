/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cstdio>
#include "CMRProjectEquation.h"
#include "parsor/TexParsor.h"

using namespace std;
using namespace CMRCompiler;

/********************  NAMESPACE  *******************/
namespace CMRCompilerOld
{

/*******************  FUNCTION  *********************/
CMRProjectEquation::CMRProjectEquation ( const string& latexName, const string& longName, const string& compute ) 
	: CMRProjectEntity ( latexName, longName )
{
	this->compute = compute;
	cmrParseLatexFormula(formula,compute);
}

/*******************  FUNCTION  *********************/
void CMRProjectEquation::printDebug ( void ) const
{
	CMRProjectEntity::printDebug();
	printf("    - value     : %s\n",compute.c_str());
}

/*******************  FUNCTION  *********************/
LatexEntityOld* CMRProjectEquation::extractNextInnerLoop ( void )
{
	return extractFirstInnerLoop(formula);
}

/*******************  FUNCTION  *********************/
LatexEntityOld* extractFirstInnerLoop(LatexFormulasOld & formula)
{
	for (LatexEntityVectorOld::iterator it = formula.childs.begin() ; it != formula.childs.end() ; ++it)
	{
		if ((*it)->name == "\\sum")
			return *it;
	}
	return NULL;
}

}
