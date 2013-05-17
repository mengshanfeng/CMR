/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_TRANFORMATION_BASIC_H
#define CMR_TRANFORMATION_BASIC_H

/********************  HEADERS  *********************/
#include <string>
#include "CMRTransformation.h"

/*********************  TYPES  **********************/
class CMRProjectCodeEntry;
class CMRProjectCodeNode;
class CMRProjectCodeIteratorLoop;
class CMRProjectCodeEquation;
class CMRLatexFormulas2;
class CMRLatexEntity2;

/*********************  CLASS  **********************/
class CMRTransformationBasic : public CMRTransformation
{
	public:
		CMRTransformationBasic(const std::string& name);
		virtual void run(CMRProjectCodeEntry& entry);
	protected:
		virtual void transform(CMRProjectCodeNode & node);
		virtual void transform(CMRProjectCodeIteratorLoop & loop);
		virtual void transform(CMRProjectCodeEquation & equation);
		virtual void transform(CMRProjectCodeEquation & equation,CMRLatexFormulas2 & formula);
		virtual void transform(CMRProjectCodeEquation & equation,CMRLatexEntity2 & entity);
};

#endif //CMR_TRANFORMATION_BASIC_H
