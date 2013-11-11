/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_TRANSFORMATION_EXTRACT_LOOPS_H
#define CMR_TRANSFORMATION_EXTRACT_LOOPS_H

/********************  HEADERS  *********************/
#include "CMRTransformationBasic.h"

/*********************  CLASS  **********************/
class CMRTransformationExtractLoops : public CMRTransformationBasic
{
	public:
		CMRTransformationExtractLoops(void);
	protected:
		virtual void transform ( CMRProjectCodeEquation& equation, CMRCompiler::LatexEntity& entity );
};

#endif //CMR_TRANSFORMATION_EXTRACT_LOOPS_H
