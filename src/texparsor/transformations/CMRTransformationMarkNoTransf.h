/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_TRANSFORMATION_MARK_NO_TRANSF_H
#define CMR_TRANSFORMATION_MARK_NO_TRANSF_H

/********************  HEADERS  *********************/
#include <string>
#include "CMRTransformationBasic.h"
#include "../definitions/CMRProjectEntity.h"

/*********************  CLASS  **********************/
class CMRTransformationMarkNoTransf : public CMRTransformationBasic
{
	public:
		CMRTransformationMarkNoTransf(void);
	protected:
		virtual void transform(CMRProjectCodeEquation & equation,CMRLatexEntity2 & entity);
		bool markChildElements(CMRLatexFormulasVector2 & childs,const CMRProjectCaptureDefMap & defMap);
};

#endif //CMR_TRANSFORMATION_MARK_NO_TRANSF_H
