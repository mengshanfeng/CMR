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
#include <iostream>
#include <cstdlib>
#include "CMRProjectIterator.h"
#include "CMRProjectContext.h"

using namespace std;
using namespace CMRCompiler;

/********************  NAMESPACE  *******************/
namespace CMRCompilerOld
{

/*******************  FUNCTION  *********************/
CMRProjectIterator::CMRProjectIterator ( const string& latexName, const string& longName, int start, int end ) 
	: CMRProjectEntity ( latexName, longName )
{
	this->start = start;
	this->end = end;
}

/*******************  FUNCTION  *********************/
void CMRProjectIterator::printDebug ( void ) const
{
	CMRProjectEntity::printDebug();
	printf("    - values    : %d .. %d\n",start,end);
}

/*******************  FUNCTION  *********************/
void CMRProjectIterator::printCPPCode ( void ) const
{
	cout << "int " << longName << " = " << start << " ; " << longName << " <= " << end << " ; " << longName << "++" << endl;
}

/*******************  FUNCTION  *********************/
ostream& CMRProjectIterator::genUsageCCode ( ostream& out, const CMRProjectContext& context, LatexEntityOld& entity ) const
{
	cerr << "Error, iterator may not be used out of loop definition : " << latexName << " : " << start << " -> " << end << endl;
	context.printDebug();
	abort();
}

}
