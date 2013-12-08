/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMPILER_FORMULA_MATCHER_H
#define CMR_COMPILER_FORMULA_MATCHER_H

/********************  HEADERS  *********************/
#include <string>
#include "common/Common.h"
#include "LatexFormula.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  TYPES  **********************/
typedef std::map<std::string,LatexFormulas*> FormulaMatcherCaptureMap;

/********************  ENUM  ************************/
enum FormulaMatcherMode
{
	FORMULA_MATCHER_EXACT = 0x1 <<0,
	FORMULA_MATCHER_CAPTURE = 0x1<<1,
	FORMULA_MATCHER_DO_CAPTURE = 0x1<<2,
	FORMULA_MATCHER_ROOT_PARTIAL = 0x1<<4,
	FORMULA_MATCHER_DEFAULT = FORMULA_MATCHER_EXACT|FORMULA_MATCHER_CAPTURE,
};

/*********************  STRUCT  *********************/
struct FormulaMatcherResult
{
	FormulaMatcherResult(void);
	~FormulaMatcherResult(void);
	int cntExactMatch;
	int cntCaptureMatch;
	int cntCompared;
	FormulaMatcherCaptureMap captures;
};

/********************  ENUM  ************************/
enum EntityCategory
{
	ENTITY_CAT_MEMBER = 0x1<<0,
	ENTITY_CAT_OPERATOR = 0x1 <<1,
	ENTITY_CAT_GROUP = 0x1 <<2,
	ENTITY_CAT_STD = ENTITY_CAT_MEMBER | ENTITY_CAT_GROUP,
	ENTITY_CAT_ALL = ENTITY_CAT_MEMBER | ENTITY_CAT_OPERATOR | ENTITY_CAT_GROUP
};

/*********************  CLASS  **********************/
class FormulaMatcherFilter
{
	public:
		virtual ~FormulaMatcherFilter(void) {};
		virtual bool accept(const LatexEntity & model, const LatexEntity & entity) const = 0;
};

/*********************  CLASS  **********************/
class FormulaMatcherFilterDefault : public FormulaMatcherFilter
{
	public:
		FormulaMatcherFilterDefault(unsigned int accepted = ENTITY_CAT_STD);
		virtual bool accept ( const LatexEntity& model, const LatexEntity& entity ) const;
	protected:
		EntityCategory getCategory(const LatexEntity& entity) const;
	protected:
		unsigned int accepted;
};

/*********************  CLASS  **********************/
class FormulaMatcher
{
	ASSIST_UNIT_TEST( ForTestFormulaMatcher );
	public:
		FormulaMatcher(const std::string & model);
		void markForCapture( const std::string& value, const FormulaMatcherFilter* filter = new FormulaMatcherFilterDefault(ENTITY_CAT_STD) );
		void dumpAsTree ( std::ostream& buffer, int indent = 0 ) const;
		void dumpAsXml(std::ostream & out, int depth = 0) const;
		bool match(const LatexFormulas & f,unsigned int mode = FORMULA_MATCHER_DEFAULT) const;
		bool match(const LatexFormulas & f,FormulaMatcherResult & result,unsigned int mode = FORMULA_MATCHER_DEFAULT) const;
	protected:
		void setupCaptureFlag( LatexEntity& entity, const LatexEntity& what, const FormulaMatcherFilter* filter );
		void setupCaptureFlag( LatexFormulas& formula, const LatexEntity& what, const FormulaMatcherFilter* filter );
		void setupCaptureFlag( LatexFormulasVector& formula, const LatexEntity& what, const FormulaMatcherFilter* filter );
		bool internalMatch( const LatexFormulas& ref, const LatexFormulas& f, FormulaMatcherResult& result, unsigned int mode ) const;
		bool internalMatch( const LatexEntity& ref, const LatexEntity& f, FormulaMatcherResult& result, unsigned int mode ) const;
		bool internalMatch(const LatexFormulasVector & ref,const LatexFormulasVector & f,FormulaMatcherResult & result,unsigned int mode) const;
	protected:
		LatexFormulas formula;
};

};

#endif //CMR_COMPILER_FORMULA_MATCHER_H
