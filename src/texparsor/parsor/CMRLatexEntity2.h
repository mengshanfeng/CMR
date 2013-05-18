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
#include <map>
#include "CMRLatexException.h"

/*********************  TYPES  **********************/
class CMRLatexParsorContext;
class CMRLatexFormulas2;
typedef std::vector<CMRLatexFormulas2*> CMRLatexFormulasVector2;
typedef std::map<std::string,void *> CMRLatexExtraInfo;

/*********************  CLASS  **********************/
class CMRLatexEntity2
{
	public:
		CMRLatexEntity2(void);
		CMRLatexEntity2(const std::string & value);
		CMRLatexEntity2(const CMRLatexEntity2 & orig);
		~CMRLatexEntity2(void);
		std::string getString(void) const;
		const std::string & getName(void) const;
		int countIndices(void) const;
		int countExponents(void) const;
		int countParameters(void) const;
		CMRLatexFormulas2 * getIndice(int id);
		CMRLatexFormulas2 * getExponent(int id);
		CMRLatexFormulas2 * getParameter(int id);
		const CMRLatexFormulas2 * getIndice(int id) const;
		const CMRLatexFormulas2 * getExponent(int id) const;
		const CMRLatexFormulas2 * getParameter(int id) const;
		CMRLatexFormulasVector2 & getIndices(void);
		CMRLatexFormulasVector2 & getExponents(void);
		CMRLatexFormulasVector2 & getParameters(void);
		void parse( const std::string& value ) throw(CMRLatexException);
		void parse( CMRLatexParsorContext & context);
		void clear(void);
		void dumpAsXml(std::ostream & out, int depth = 0) const;
		void dumpAsTree(std::ostream & out, int depth = 0) const;
		bool isOnlyOneName(void) const;
		void setExtraInfo(const std::string & key,void * value,bool allowOverride = false);
		void deleteInfo(const std::string & key, bool throwOnError = true);
		void * getExtraInfo(const std::string & key,bool throwOnError = true);
		bool hasInfo(const std::string & key) const;
		CMRLatexEntity2 & operator=(const CMRLatexEntity2 & value);
	protected:
		void copy(const CMRLatexEntity2 & value);
		static void copy(CMRLatexFormulasVector2 & vectorOut,const CMRLatexFormulasVector2 & vectorOrig);
		static void writeFormulaList(std::ostream & out,const CMRLatexFormulasVector2 & value,const std::string & sep=",",const std::string & open = "{",const std::string & close =  "}", bool forceOpenClose = false);
		void parseSubGroup(CMRLatexParsorContext & context);
		void parseStandard(CMRLatexParsorContext & context);
		static std::string extractName( CMRLatexParsorContext& context );
		CMRLatexFormulas2* extractSubFormula( CMRLatexParsorContext& context );
		void extractSubAndSuperScript(CMRLatexParsorContext & context);
		void extractParameters(CMRLatexParsorContext & context);
		static void dumpAsXml(std::ostream & out, int depth,const std::string & name,const CMRLatexFormulasVector2 & list);
		static void dumpAsTree(std::ostream & out, int depth,const std::string & name,const CMRLatexFormulasVector2 & list);
	public:
		CMRLatexEntity2 & operator=(const std::string & value);
		friend std::ostream & operator << (std::ostream & out,const CMRLatexEntity2 & value);
	public:
		std::string name;
		CMRLatexFormulasVector2 indices;
		CMRLatexFormulasVector2 exponents;
		CMRLatexFormulasVector2 parameters;
		CMRLatexExtraInfo extraInfos;
};

#endif //CMR_LATEX_ENTITY2_H
