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
#include "CMRProjectIterator.h"

using namespace std;

/*******************  FUNCTION  *********************/
CMRProjectIterator::CMRProjectIterator ( const string& latexName, const string& longName, int start, int end ) 
	: CMREntity ( latexName, longName )
{
	this->start = start;
	this->end = end;
}

/*******************  FUNCTION  *********************/
void CMRProjectIterator::printDebug ( void ) const
{
	CMREntity::printDebug();
	printf("    - values    : %d .. %d\n",start,end);
}

/*******************  FUNCTION  *********************/
void CMRProjectIterator::printCPPCode ( void ) const
{
	cout << "int " << longName << " = " << start << " ; " << longName << " <= " << end << " ; " << longName << "++" << endl;
}
