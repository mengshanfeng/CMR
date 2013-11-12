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

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  CLASS  **********************/
class CMRTransformationExpandExpo : public CMRTransformationBasic
{
	public:
		CMRTransformationExpandExpo(void);
	protected:
		virtual void transform ( CMRProjectCodeEquation& equation, CMRCompiler::LatexEntity& entity );
};

}

#endif //CMR_TRANSFORMATION_EXPAND_EXPO_H
