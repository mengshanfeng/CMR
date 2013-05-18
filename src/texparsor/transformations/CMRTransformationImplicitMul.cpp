/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "CMRTransformationImplicitMul.h"
#include "../parsor/CMRLatexFormula.h"
#include "../parsor/CMRParsorBasics.h"

/*******************  FUNCTION  *********************/
CMRTransformationImplicitMul::CMRTransformationImplicitMul ( void ) 
	: CMRTransformationBasic("CMRTransformationImplicitMul")
{

}

/*******************  FUNCTION  *********************/
void CMRTransformationImplicitMul::transform ( CMRProjectCodeEquation& equation, CMRLatexFormulas2& formula )
{
	int cnt = 0;
	CMRLatexFormulas2 f;
	
	//check first element for minus at first pos (-a....)
	if (formula[0]->getString() == "-")
		cnt++;
	
	//loop on all elements
	for (CMRLatexFormulas2::iterator it = formula.begin() ;  it != formula.end() ; ++it)
	{
		std::string tmp = (*it)->getString();
		if (cnt % 2 == 1 && (tmp.size() > 1 || cmrIsOperator(tmp[0]) == false))
		{
			f.push_back(new CMRLatexEntity2("*"));
			cnt++;
		}
		f.push_back(*it);
		*it = NULL;
		cnt++;
	}
	
	formula.clear();
	for (CMRLatexFormulas2::iterator it = f.begin() ;  it != f.end() ; ++it)
	{
		formula.push_back(*it);
		*it = NULL;
	}

	CMRTransformationBasic::transform ( equation, formula );
}
