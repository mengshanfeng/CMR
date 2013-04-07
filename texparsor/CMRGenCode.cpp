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
#include <iostream>
#include <cstdlib>
#include "parsor/CMRParsorBasics.h"
#include "parsor/CMRLatexEntity.h"
#include "CMRGenCode.h"
#include "CMRProjectContext.h"

using namespace std;

/*******************  FUNCTION  *********************/
bool latexEntityIsInteger(CMRLatexEntity & entity)
{
	if (entity.totalValue.empty() == true)
		return false;
	for (int i = 0 ; i < entity.totalValue.size() ; i++)
		if (!cmrIsNum(entity.totalValue[i]))
			return false;
	return true;
}

/*******************  FUNCTION  *********************/
std::ostream&  cmrGenEqCCode ( ostream& out, CMRProjectContext& context, CMRLatexFormulas& formula )
{
	CMRLatexEntityVector & elems = formula.childs;
	for (CMRLatexEntityVector::iterator it = elems.begin() ;  it != elems.end() ; ++it)
		cmrGenEqCCode(out,context,**it);
	return out;
}

/*******************  FUNCTION  *********************/
std::ostream&  cmrGenEqCCode(ostream& out, CMRProjectContext& context, CMRLatexEntity& entity)
{
	if (latexEntityIsInteger(entity))
	{
		out << entity.totalValue << " ";
	} else if (entity.totalValue.size() == 1 && cmrIsOperator(entity.totalValue[0])) {
		out << entity.totalValue << " ";
	} else if (entity.name == "(") {
		out << "( ";
		assert(entity.params[0] != NULL);
		cmrGenEqCCode(out,context,*entity.params[0]);
		out << ") ";
	} else {
		//search matching in context
		CMREntity * def = context.find(entity);
		if (def == NULL)
		{
			cerr << endl << "Unknown member " << entity.totalValue;// << " in equation " << eq->latexName << " = " << eq->compute << endl;
			abort();
		}
		def->genUsageCCode(out,context,entity);
		out << " ";
	}
	return out;
}
