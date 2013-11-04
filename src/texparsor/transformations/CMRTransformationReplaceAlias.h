/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_TRANSFORMATION_REPLACE_ALIAS_H
#define CMR_TRANSFORMATION_REPLACE_ALIAS_H

/********************  HEADERS  *********************/
#include "CMRTransformationBasic.h"

/*********************  CLASS  **********************/
class CMRTransformationReplaceAlias : public CMRTransformationBasic
{
	public:
		CMRTransformationReplaceAlias(void);
	protected:
		virtual void transform(CMRProjectCodeEquation & equation,CMRLatexEntity2 & entity);
};

#endif //CMR_TRANSFORMATION_REPLACE_ALIAS_H
