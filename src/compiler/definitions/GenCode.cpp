/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
//system headers
#include <cassert>
#include <sstream>
//internal headers
#include "ProjectContext.h"
#include "parsor/ParsorBasics.h"
#include "parsor/LatexFormula.h"
#include "GenCode.h"

/**********************  USING  *********************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
bool latexEntityIsInteger(const LatexEntity & entity)
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
bool latexEntityIsNum(const LatexEntity & entity)
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
std::ostream&  cmrGenEqCCode ( ostream& out, const ProjectContext& context, const LatexFormulas& formula )
{
	const LatexEntityVector & elems = formula;
	for (LatexEntityVector::const_iterator it = elems.begin() ;  it != elems.end() ; ++it)
		cmrGenEqCCode(out,context,**it);
	return out;
}

/*******************  FUNCTION  *********************/
std::ostream&  cmrGenEqCCode(ostream& out, const ProjectContext& context, const LatexEntity& entity)
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
	} else if (entity.name == "\\cmrsubaction") {
		#warning "Remove this from here"
		out << "((Action" << *entity.getParameter(0) << "*)NULL)->cellAction(in,out,pos,x,y)";
	} else if (entity.name == "\\mathrm") {
		out << entity.getParameter(0)->front()->getName();
	} else {
		//search matching in context
		const ProjectEntity * def = context.find(entity);
		if (def == NULL)
		{
			stringstream out;
			out << endl << "Unknown member for latex entity conversion to C " << entity.getString();// << " in equation " << eq->latexName << " = " << eq->compute << endl;
			out << endl;
			out << "Context is : " << endl;
			context.printDebug(out);
			throw LatexException(out.str());
		}
		def->genUsageCCode(out,context,entity);
		out << " ";
	}
	return out;
}

}
