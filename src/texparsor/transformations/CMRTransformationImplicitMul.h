/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_TRANSFORMATION_IMPLICIT_MUL_H
#define CMR_TRANSFORMATION_IMPLICIT_MUL_H

/********************  HEADERS  *********************/
#include <string>
#include "CMRTransformationBasic.h"

/*********************  CLASS  **********************/
class CMRTransformationImplicitMul : public CMRTransformationBasic
{
	public:
		CMRTransformationImplicitMul(void);
	protected:
		virtual void transform ( CMRProjectCodeEquation& equation, CMRLatexFormulas2& formula );
};

#endif //CMR_TRANSFORMATION_IMPLICIT_MUL_H
