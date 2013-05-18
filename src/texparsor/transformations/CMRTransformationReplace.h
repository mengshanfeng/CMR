/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_TRANSFORMATION_REPLACE_H
#define CMR_TRANSFORMATION_REPLACE_H

/********************  HEADERS  *********************/
#include <string>
#include "CMRTransformationBasic.h"
#include "../definitions/CMRProjectEntity.h"

/*********************  CLASS  **********************/
class CMRTransformationReplace : public CMRTransformationBasic
{
	public:
		CMRTransformationReplace(const CMRProjectCaptureMap * replaceMap);
		virtual void transform(CMRProjectCodeEquation & equation,CMRLatexEntity2 & entity);
		static void replaceAll(const CMRProjectCaptureMap& replaceMap, CMRLatexEntity2& entity);
		static void replaceAll(const CMRProjectCaptureMap& replaceMap, CMRLatexFormulas2& formula);
		static const CMRLatexFormulas2* searchReplaceValue(const CMRProjectCaptureMap& replaceMap, const CMRLatexEntity2& entity);
	private:
		const CMRProjectCaptureMap * replaceMap;
};

#endif //CMR_TRANSFORMATION_REPLACE_H
