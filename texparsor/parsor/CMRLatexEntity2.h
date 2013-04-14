/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_LATEX_ENTITY2_H
#define CMR_LATEX_ENTITY2_H

/********************  HEADERS  *********************/
#include <string>
#include <vector>
#include "CMRLatexException.h"

/*********************  TYPES  **********************/
class CMRLatexParsorContext;
class CMRLatexFormulas2;
typedef std::vector<CMRLatexFormulas2*> CMRLatexFormulasVector2;

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
		bool isOnlyOneName(void) const;
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

#endif //CMR_LATEX_ENTITY2_H
