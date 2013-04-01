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
#include "parsor/CMRTexParsor.h"

using namespace std;

/*******************  FUNCTION  *********************/
CMRProjectEquation::CMRProjectEquation ( const string& latexName, const string& longName, const string& compute ) 
	: CMREntity ( latexName, longName )
{
	this->compute = compute;
	cmrParseLatexFormula(formula,compute);
}

/*******************  FUNCTION  *********************/
void CMRProjectEquation::printDebug ( void ) const
{
	CMREntity::printDebug();
	printf("    - value     : %s\n",compute.c_str());
}

/*******************  FUNCTION  *********************/
CMRLatexEntity* CMRProjectEquation::extractNextInnerLoop ( void )
{
	return extractFirstInnerLoop(formula);
}

/*******************  FUNCTION  *********************/
CMRLatexEntity* extractFirstInnerLoop(CMRLatexFormulas & formula)
{
	for (CMRLatexEntityVector::iterator it = formula.childs.begin() ; it != formula.childs.end() ; ++it)
	{
		if ((*it)->name == "\\sum")
			return *it;
	}
	return NULL;
}
