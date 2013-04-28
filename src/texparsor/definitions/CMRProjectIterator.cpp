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
#include <sstream>
#include "CMRProjectIterator.h"
#include "CMRProjectContext.h"

using namespace std;

/*******************  FUNCTION  *********************/
CMRProjectIterator::CMRProjectIterator ( const string& latexName, const string& longName, int start, int end ) 
	: CMRProjectEntity ( latexName, longName )
{
	this->start = start;
	this->end = end;
}

/*******************  FUNCTION  *********************/
void CMRProjectIterator::printDebug ( std::ostream & out ) const
{
	CMRProjectEntity::printDebug(out);
	out << "    - values     : "<< start << " .. " << end << endl;
}

/*******************  FUNCTION  *********************/
void CMRProjectIterator::genDefinitionCCode ( ostream& out, const CMRProjectContext& context ) const
{
	out << "int " << getLongName() << " = " << start << " ; " << getLongName() << " <= " << end << " ; " << getLongName() << "++";
}

/*******************  FUNCTION  *********************/
void CMRProjectIterator::genUsageCCode ( ostream& out, const CMRProjectContext& context, const CMRLatexEntity2& entity, bool write ) const
{
	stringstream err;
	err << "Error, iterator may not be used out of loop definition : " << getLatexName() << " : " << start << " -> " << end << endl;
	context.printDebug(err);
	throw CMRLatexException(err.str());
}
