/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_TRANSFORMATION_EXPAND_EXPO_H
#define CMR_TRANSFORMATION_EXPAND_EXPO_H

/********************  HEADERS  *********************/
#include "CMRTransformationBasic.h"

/*********************  CLASS  **********************/
class CMRTransformationExpandExpo : public CMRTransformationBasic
{
	public:
		CMRTransformationExpandExpo(void);
	protected:
		virtual void transform ( CMRProjectCodeEquation& equation, CMRLatexEntity2& entity );
};

#endif //CMR_TRANSFORMATION_EXPAND_EXPO_H
