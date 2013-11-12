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
#include "../definitions/ProjectEntity.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  CLASS  **********************/
class CMRTransformationReplace : public CMRTransformationBasic
{
	public:
		CMRTransformationReplace(const ProjectCaptureMap * replaceMap);
		virtual void transform(CMRProjectCodeEquation & equation,CMRCompiler::LatexEntity & entity);
		static void replaceAll(const ProjectCaptureMap& replaceMap, CMRCompiler::LatexEntity& entity);
		static void replaceAll(const ProjectCaptureMap& replaceMap, CMRCompiler::LatexFormulas& formula);
		static void replaceAll( const ProjectCaptureMap& replaceMap, CMRCompiler::LatexFormulasVector& formulas );
		static const CMRCompiler::LatexFormulas* searchReplaceValue(const ProjectCaptureMap& replaceMap, const CMRCompiler::LatexEntity& entity);
	private:
		const ProjectCaptureMap * replaceMap;
};

}

#endif //CMR_TRANSFORMATION_REPLACE_H
