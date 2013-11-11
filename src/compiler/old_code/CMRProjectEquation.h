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
class CMRProjectEquation : public CMRProjectEntity
{
	public:
		CMRProjectEquation ( const std::string& latexName, const std::string& longName , const std::string & compute);
		void printDebug(void) const;
		CMRCompiler::LatexEntityOld * extractNextInnerLoop(void);
		std::string compute;
		CMRCompiler::LatexFormulasOld formula;
};

/*******************  FUNCTION  *********************/
CMRCompiler::LatexEntityOld* extractFirstInnerLoop(CMRCompiler::LatexFormulasOld & formula);

#endif
