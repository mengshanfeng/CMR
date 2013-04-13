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
#include "CMRProjectAction.h"

/*********************  TYPES  **********************/
struct CMRLatexEntity;
struct CMRLatexFormulas;

/*********************  CLASS  **********************/
class CMRProjectTransformation
{
	public:
		CMRProjectTransformation(bool loopOnLatexEntities = true);
		virtual ~CMRProjectTransformation(void);
		void run(CMRProjectAction * rootAction);
	protected:
		virtual void init(void);
		virtual void finish(void);
		virtual CMRProjectAction::Iterator transform( CMRProjectAction::Iterator action,int depth);
		virtual void closeNode(CMRProjectAction::Iterator,int depth);
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
		virtual CMRProjectAction::Iterator transform( CMRProjectAction::Iterator action,int depth);
	private:
		int tmpId;
};

/*********************  CLASS  **********************/
class CMRProjectTransfImplicitMul : public CMRProjectTransformation
{
	public:
		CMRProjectTransfImplicitMul (void);
	protected:
		void replaceImplMul(CMRLatexFormulasList & formula);
		void replaceImplMul(CMRLatexFormulas & formula);
		virtual CMRProjectAction::Iterator transform ( CMRProjectAction::Iterator action, int depth );
};

/*********************  CLASS  **********************/
class CMRProjectTransfExpandFrac : public CMRProjectTransformation
{
	protected:
		void expandFrac(CMRLatexFormulasList & forumlas);
		void expandFrac(CMRLatexEntity & entity);
		void expandFrac(CMRLatexFormulas & forumlas);
		virtual CMRProjectAction::Iterator transform ( CMRProjectAction::Iterator action, int depth );
};

/*********************  CLASS  **********************/
class CMRProjectTransfExpendExponent : public CMRProjectTransformation
{
	protected:
		void expandExponent( CMRLatexFormulas& formulas, CMRProjectAction& action );
		void expandExponent( CMRLatexEntity& entity, CMRProjectAction& action );
		virtual CMRProjectAction::Iterator transform ( CMRProjectAction::Iterator action, int depth );
};

#endif //CMR_PROJECT_TRANSFORMATION_H
