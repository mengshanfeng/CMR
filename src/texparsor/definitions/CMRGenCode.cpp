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
#include <sstream>
#include "../parsor/CMRParsorBasics.h"
#include "../parsor/CMRLatexEntity2.h"
#include "../parsor/CMRLatexFormula.h"
#include "CMRGenCode.h"
#include "CMRProjectContext.h"

using namespace std;

/*******************  FUNCTION  *********************/
bool latexEntityIsInteger(const CMRLatexEntity2 & entity)
{
	//check if name is empty
	if (entity.getName().empty() == true)
		return false;

	//check if have expo, indices or params
	if (entity.isOnlyOneName() == false)
		return false;
	
	//loop on chars of name
	const string tmp = entity.getName();
	for (int i = 0 ; i < tmp.size() ; i++)
		if (!cmrIsNum(tmp[i]))
			return false;

	//ok this is good
	return true;
}

/*******************  FUNCTION  *********************/
bool latexEntityIsNum(const CMRLatexEntity2 & entity)
{
	//check if name is empty
	if (entity.getName().empty() == true)
		return false;

	//check if have expo, indices or params
	if (entity.isOnlyOneName() == false)
		return false;
	
	//loop on chars of name
	const string tmp = entity.getName();
	for (int i = 0 ; i < tmp.size() ; i++)
		if (!cmrIsNumFloat(tmp[i]))
			return false;

	//ok this is good
	return true;
}

/*******************  FUNCTION  *********************/
std::ostream&  cmrGenEqCCode ( ostream& out, const CMRProjectContext& context, const CMRLatexFormulas2& formula )
{
	const CMRLatexEntityVector2 & elems = formula;
	for (CMRLatexEntityVector2::const_iterator it = elems.begin() ;  it != elems.end() ; ++it)
		cmrGenEqCCode(out,context,**it);
	return out;
}

/*******************  FUNCTION  *********************/
std::ostream&  cmrGenEqCCode(ostream& out, const CMRProjectContext& context, const CMRLatexEntity2& entity)
{
	if (latexEntityIsNum(entity))
	{
		out << entity.getName() << " ";
	} else if (entity.getName().size() == 1 && entity.isOnlyOneName() && cmrIsOperator(entity.getName()[0])) {
		out << entity.getName() << " ";
	} else if (entity.name == "()") {
		out << "( ";
		assert(entity.getParameter(0) != NULL);
		cmrGenEqCCode(out,context,*entity.getParameter(0));
		out << ") ";
	} else {
		//search matching in context
		const CMRProjectEntity * def = context.find(entity);
		if (def == NULL)
		{
			stringstream out;
			out << endl << "Unknown member " << entity.getString();// << " in equation " << eq->latexName << " = " << eq->compute << endl;
			out << endl;
			out << "Context is : " << endl;
			context.printDebug(out);
			throw CMRLatexException(out.str());
		}
		def->genUsageCCode(out,context,entity);
		out << " ";
	}
	return out;
}
