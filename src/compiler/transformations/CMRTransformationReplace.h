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
#include "CMRTransformationBasic.h"
#include "../definitions/CMRProjectEntity.h"

/*********************  CLASS  **********************/
class CMRTransformationReplace : public CMRTransformationBasic
{
	public:
		CMRTransformationReplace(const CMRProjectCaptureMap * replaceMap);
		virtual void transform(CMRProjectCodeEquation & equation,CMRCompiler::LatexEntity & entity);
		static void replaceAll(const CMRProjectCaptureMap& replaceMap, CMRCompiler::LatexEntity& entity);
		static void replaceAll(const CMRProjectCaptureMap& replaceMap, CMRCompiler::LatexFormulas& formula);
		static void replaceAll( const CMRProjectCaptureMap& replaceMap, CMRCompiler::LatexFormulasVector& formulas );
		static const CMRCompiler::LatexFormulas* searchReplaceValue(const CMRProjectCaptureMap& replaceMap, const CMRCompiler::LatexEntity& entity);
	private:
		const CMRProjectCaptureMap * replaceMap;
};

#endif //CMR_TRANSFORMATION_REPLACE_H
