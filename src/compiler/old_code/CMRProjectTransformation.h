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
#include "ProjectActionOld.h"

/*********************  TYPES  **********************/
namespace CMRCompiler
{
	struct LatexEntityOld;
	struct LatexFormulasOld;
}

/********************  NAMESPACE  *******************/
namespace CMRCompilerOld
{

/*********************  CLASS  **********************/
class CMRProjectTransformation
{
	public:
		CMRProjectTransformation(bool loopOnLatexEntities = true);
		virtual ~CMRProjectTransformation(void);
		void run(ProjectActionOld * rootAction);
	protected:
		virtual void init(void);
		virtual void finish(void);
		virtual ProjectActionOld::Iterator transform( ProjectActionOld::Iterator action,int depth);
		virtual void closeNode(ProjectActionOld::Iterator,int depth);
	private:
		bool loopOnLatexEntities;
};

/*********************  CLASS  **********************/
class CMRProjectTransfExtractLoops : public CMRProjectTransformation
{
	public:
		CMRProjectTransfExtractLoops(void);
		virtual void init ( void );
	protected:
		virtual ProjectActionOld::Iterator transform( ProjectActionOld::Iterator action,int depth);
	private:
		int tmpId;
};

/*********************  CLASS  **********************/
class CMRProjectTransfImplicitMul : public CMRProjectTransformation
{
	public:
		CMRProjectTransfImplicitMul (void);
	protected:
		void replaceImplMul(CMRCompiler::LatexFormulasListOld & formula);
		void replaceImplMul(CMRCompiler::LatexFormulasOld & formula);
		virtual ProjectActionOld::Iterator transform ( ProjectActionOld::Iterator action, int depth );
};

/*********************  CLASS  **********************/
class CMRProjectTransfExpandFrac : public CMRProjectTransformation
{
	protected:
		void expandFrac(CMRCompiler::LatexFormulasListOld & forumlas);
		void expandFrac(CMRCompiler::LatexEntityOld & entity);
		void expandFrac(CMRCompiler::LatexFormulasOld & forumlas);
		virtual ProjectActionOld::Iterator transform ( ProjectActionOld::Iterator action, int depth );
};

/*********************  CLASS  **********************/
class CMRProjectTransfExpendExponent : public CMRProjectTransformation
{
	protected:
		void expandExponent( CMRCompiler::LatexFormulasOld& formulas, ProjectActionOld& action );
		void expandExponent( CMRCompiler::LatexEntityOld& entity, ProjectActionOld& action );
		virtual ProjectActionOld::Iterator transform ( ProjectActionOld::Iterator action, int depth );
};

}

#endif //CMR_PROJECT_TRANSFORMATION_H
