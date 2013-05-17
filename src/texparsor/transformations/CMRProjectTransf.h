/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_PROJECT_TRANSFORMATION_H
#define CMR_PROJECT_TRANSFORMATION_H

/********************  HEADERS  *********************/
#include <string>

/*********************  TYPES  **********************/
class CMRProjectCodeEntry;
class CMRProjectCodeNode;
class CMRProjectCodeIteratorLoop;
class CMRProjectCodeEquation;
class CMRLatexFormulas2;
class CMRLatexEntity2;

/*********************  CLASS  **********************/
class CMRProjectTransformation
{
	public:
		CMRProjectTransformation(const std::string & name);
		virtual ~CMRProjectTransformation(void);
		virtual void run(CMRProjectCodeEntry & entry) = 0;
		const std::string & getName(void) const;
	private:
		std::string name;
};

/*********************  CLASS  **********************/
class CMRProjectTransformationBasic : public CMRProjectTransformation
{
	public:
		CMRProjectTransformationBasic(const std::string& name);
		virtual void run(CMRProjectCodeEntry& entry);
	protected:
		virtual void transform(CMRProjectCodeNode & node);
		virtual void transform(CMRProjectCodeIteratorLoop & loop);
		virtual void transform(CMRProjectCodeEquation & equation);
		virtual void transform(CMRProjectCodeEquation & equation,CMRLatexFormulas2 & formula);
		virtual void transform(CMRProjectCodeEquation & equation,CMRLatexEntity2 & entity);
};

#endif //CMR_PROJECT_TRANSFORMATION_H
