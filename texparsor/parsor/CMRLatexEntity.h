/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_LATEX_ENTITY_H
#define CMR_LATEX_ENTITY_H

/********************  HEADERS  *********************/
#include <ostream>
#include <vector>
#include <string>
#include "CMRLatexException.h"

class CMRLatexParsorContext;
/*********************  TYPES  **********************/
struct CMRLatexEntity;
struct CMRLatexFormulas;
typedef std::vector<CMRLatexFormulas*> CMRLatexFormulasList;
typedef std::vector<CMRLatexEntity*> CMRLatexEntityVector;

class CMRLatexEntity2;
class CMRLatexFormulas2;
typedef std::vector<CMRLatexFormulas2*> CMRLatexFormulasVector2;
typedef std::vector<CMRLatexEntity2*> CMRLatexEntityVector2;

/*********************  STRUCT  *********************/
struct CMRLatexFormulas
{
	CMRLatexEntityVector childs;
	std::string string;
};

/*********************  STRUCT  *********************/
struct CMRLatexEntity
{
	CMRLatexEntity(void);
	~CMRLatexEntity(void);
	void print(int depth, int pos);
	int countIndices(void) const;
	CMRLatexFormulasList getIndices( void );
	std::string getString(void) const;
	std::string name;
	std::string parent;
	CMRLatexFormulasList subscript;
	CMRLatexFormulasList superscript;
	CMRLatexFormulasList params;
	int from;
	int to;
	std::string totalValue;
	std::string subscriptTotalValue;
	std::string superscriptTotalValue;
	std::string argsTotalValue;
};

/*********************  CLASS  **********************/
class CMRLatexFormulas2 : public CMRLatexEntityVector2
{
	public:
		CMRLatexFormulas2(void);
		CMRLatexFormulas2(const std::string & value);
		~CMRLatexFormulas2(void);
		void parse(const std::string & value);
		void parse(CMRLatexParsorContext & context);
		void split(CMRLatexFormulasVector2 & formulas,const std::string & sep = ",");
		std::string getString(void) const;
		friend std::ostream & operator << (std::ostream & out,const CMRLatexFormulas2 & value);
		void dumpAsXml(std::ostream & out, int depth = 0) const;
		void dumpAsTree(std::ostream & out, int depth = 0) const;
		void clear(void);
	private:
		CMRLatexFormulas2(const CMRLatexFormulas2 & orig);
		CMRLatexFormulas2 & operator=(const CMRLatexFormulas2 & orig);
};

/*********************  CLASS  **********************/
class CMRLatexEntity2
{
	public:
		CMRLatexEntity2(void);
		CMRLatexEntity2(const std::string & value);
		~CMRLatexEntity2(void);
		std::string getString(void) const;
		const std::string & getName(void) const;
		int countIndices(void) const;
		int countExponents(void) const;
		int countParameters(void) const;
		const CMRLatexFormulas2 * getIndice(int id) const;
		const CMRLatexFormulas2 * getExponent(int id) const;
		const CMRLatexFormulas2 * getParameter(int id) const;
		void parse( const std::string& value ) throw(CMRLatexException);
		void parse( CMRLatexParsorContext & context);
		void clear(void);
		void dumpAsXml(std::ostream & out, int depth = 0) const;
		void dumpAsTree(std::ostream & out, int depth = 0) const;
	protected:
		static void writeFormulaList(std::ostream & out,const CMRLatexFormulasVector2 & value,const std::string & sep=",",const std::string & open = "{",const std::string & close =  "}", bool forceOpenClose = false);
		void parseSubGroup(CMRLatexParsorContext & context);
		void parseStandard(CMRLatexParsorContext & context);
		static std::string extractName( CMRLatexParsorContext& context );
		CMRLatexFormulas2* extractSubFormula( CMRLatexParsorContext& context );
		void extractSubAndSuperScript(CMRLatexParsorContext & context);
		void extractParameters(CMRLatexParsorContext & context);
		static void dumpAsXml(std::ostream & out, int depth,const std::string & name,const CMRLatexFormulasVector2 & list);
		static void dumpAsTree(std::ostream & out, int depth,const std::string & name,const CMRLatexFormulasVector2 & list);
	private:
		CMRLatexEntity2(const CMRLatexEntity2 & orig);
		CMRLatexEntity2 & operator=(const CMRLatexEntity2 & value);
	public:
		CMRLatexEntity2 & operator=(const std::string & value);
		friend std::ostream & operator << (std::ostream & out,const CMRLatexEntity2 & value);
	public:
		std::string name;
		CMRLatexFormulasVector2 indices;
		CMRLatexFormulasVector2 exponents;
		CMRLatexFormulasVector2 parameters;
};

/*******************  FUNCTION  *********************/
void cmrPrintFormula(const CMRLatexFormulasList & formulas,int depth);
void cmrPrintFormula(const CMRLatexFormulas & formula,int depth);

#endif //CMR_LATEX_ENTITY_H
