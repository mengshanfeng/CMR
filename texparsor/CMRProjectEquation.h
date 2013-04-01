/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_PROJECT_EQUATION_H
#define CMR_PROJECT_EQUATION_H

/********************  HEADERS  *********************/
#include "CMRProjectEntity.h"
#include <string>

/*********************  CLASS  **********************/
class CMRProjectEquation : public CMREntity
{
	public:
		CMRProjectEquation ( const std::string& latexName, const std::string& longName , const std::string & compute);
		void printDebug(void) const;
		CMRLatexEntity * extractNextInnerLoop(void);
	private:
		std::string compute;
		CMRLatexFormulas formula;
};

/*******************  FUNCTION  *********************/
CMRLatexEntity* extractFirstInnerLoop(CMRLatexFormulas & formula);

#endif 