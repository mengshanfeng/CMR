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

/********************  MACRO  ***********************/
#define CMR_FORMULA_MATCHER_FILTER_DEFAULT (new FormulaMatcherFilterDefault(ENTITY_CAT_STD))
#define CMR_EXPONENT_NAME "\\cmropexp"

/*********************  TYPES  **********************/
typedef std::map<std::string,LatexFormulas*> FormulaMatcherCaptureMap;

/********************  ENUM  ************************/
enum FormulaMatcherMode
{
	FORMULA_MATCHER_EXACT = 0x1 <<0,
	FORMULA_MATCHER_CAPTURE = 0x1<<1,
	FORMULA_MATCHER_DO_CAPTURE = 0x1<<2,
	FORMULA_MATCHER_ROOT_PARTIAL = 0x1<<3,
	FORMULA_MATCHER_ROOT_OP_EXPO = 0x1<<4,
	FORMULA_MATCHER_DEFAULT = FORMULA_MATCHER_EXACT|FORMULA_MATCHER_CAPTURE|FORMULA_MATCHER_ROOT_OP_EXPO,
};

/*********************  STRUCT  *********************/
struct FormulaMatcherResult
{
	FormulaMatcherResult(void);
	~FormulaMatcherResult(void);
	bool hasExtract(const std::string & key) const;
	bool hasExponent(void) const;
	void printDebug( std::ostream& out ) const;
	std::string toString(void) const;
	int cntExactMatch;
	int cntCaptureMatch;
	int cntCompared;
	FormulaMatcherCaptureMap captures;
	LatexFormulas * exponent;
};

/********************  ENUM  ************************/
enum EntityCategory
{
	ENTITY_CAT_MEMBER = 0x1<<0,
	ENTITY_CAT_OPERATOR = 0x1 <<1,
	ENTITY_CAT_GROUP = 0x1 <<2,
	ENTITY_CAT_WILDCARD = 0x1 << 3,
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

/*********************  STRUCT  *********************/
struct FormulaMatcherContext
{
	FormulaMatcherContext(unsigned int mode,const LatexFormulas * ref,const LatexFormulas * formula);
	FormulaMatcherContext(const FormulaMatcherContext * parent,const LatexFormulas * ref,const LatexFormulas * formula);
	bool hasRootPartial(void) const;
	bool hasRootOptionalExpo(void) const;
	const FormulaMatcherContext * parent;
	const LatexFormulas * ref;
	const LatexFormulas * cur;
	unsigned int mode;
};

/*********************  CLASS  **********************/
class FormulaMatcher
{
	ASSIST_UNIT_TEST( ForTestFormulaMatcher );
	public:
		FormulaMatcher(const std::string & model, bool allowMultipleRootElmts = true);
		virtual ~FormulaMatcher(void);
		void markForWildcadCapture(const std::string & value);
		void markForCapture( const std::string& value, unsigned int filter = ENTITY_CAT_STD, bool optional = false, bool wildcard = false);
		void markForCapture( const std::string& value, const FormulaMatcherFilter* filter, bool optional = false, bool wildcard = false );
		void dumpAsTree ( std::ostream& buffer, int indent = 0 ) const;
		void dumpAsXml(std::ostream & out, int depth = 0) const;
		bool match(const LatexFormulas & f,unsigned int mode = FORMULA_MATCHER_DEFAULT) const;
		bool match(const LatexFormulas & f,LatexFormulas::const_iterator & startIt,unsigned int mode = FORMULA_MATCHER_DEFAULT) const;
		bool match(const LatexFormulas & f,FormulaMatcherResult & result,unsigned int mode = FORMULA_MATCHER_DEFAULT) const;
		bool match(const LatexFormulas & f,LatexFormulas::const_iterator & startIt,FormulaMatcherResult & result,unsigned int mode = FORMULA_MATCHER_DEFAULT) const;
		void capture(const LatexFormulas & f,FormulaMatcherResult & result,unsigned int mode = FORMULA_MATCHER_DEFAULT) const;
		void capture(const LatexFormulas & f,LatexFormulas::const_iterator & startIt,FormulaMatcherResult & result,unsigned int mode = FORMULA_MATCHER_DEFAULT) const;
		void printDebug(std::ostream & out) const;
		std::string toString(void) const;
		std::string toDebugString(void) const;
		void setOptionalExponent(bool force = true);
		
		//some internal ops
		operator std::string() const;
		
		//friend ops
		friend std::ostream & operator << (std::ostream & out,const FormulaMatcher & value);
	protected:
		bool setupCaptureFlag( LatexEntity& entity, const LatexEntity& what, const FormulaMatcherFilter* filter, bool optional, bool wildcard );
		bool setupCaptureFlag( LatexFormulas& formula, const LatexEntity& what, const FormulaMatcherFilter* filter, bool optional, bool wildcard );
		bool setupCaptureFlag( LatexFormulasVector& formula, const LatexEntity& what, const FormulaMatcherFilter* filter, bool optional, bool wildcard );
		bool internalMatchSubFormula( FormulaMatcherResult& result, const FormulaMatcherContext& context, LatexFormulas::const_iterator& startIt ) const;
		bool internalMatchSubFormula( FormulaMatcherResult& result, const FormulaMatcherContext& context, LatexFormulas::const_iterator& startIt,  const LatexFormulas::const_iterator& refStart ) const;
		bool internalMatchNextRefEntityWildCard( FormulaMatcherResult& result, const FormulaMatcherContext& context, LatexFormulas::const_iterator& itRef, LatexFormulas::const_iterator& itF ) const;
		bool internalMatchNextRefEntity( FormulaMatcherResult& result, const FormulaMatcherContext& context, LatexFormulas::const_iterator& itRef, LatexFormulas::const_iterator& itF ) const;
		bool internalMatchChilds( FormulaMatcherResult& result, const FormulaMatcherContext& context, const LatexEntity& ref, const LatexEntity& cur, LatexEntityChilds childMode ) const;
		bool isOptionalFormula(const LatexFormulas& f) const;
		void printDebug(std::ostream & out,const LatexFormulas & f) const;
		void printDebug(std::ostream & out,const LatexEntity & entity) const;
		void printDebug(std::ostream & out,const LatexFormulasVector & childs,const std::string & sep) const;
	protected:
		LatexFormulas formula;
		bool allowExponent;
};

};

#endif //CMR_COMPILER_FORMULA_MATCHER_H
