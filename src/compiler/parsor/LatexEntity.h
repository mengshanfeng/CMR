/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMPILER_LATEX_ENTITY_H
#define CMR_COMPILER_LATEX_ENTITY_H

/********************  HEADERS  *********************/
#include <string>
#include <vector>
#include <map>
#include "LatexException.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  TYPES  **********************/
class LatexParsorContext;
class LatexFormulas;
typedef std::vector<LatexFormulas*> LatexFormulasVector;
typedef std::map<std::string,void *> CMRLatexExtraInfo;

/*********************  CLASS  **********************/
class LatexEntity
{
	public:
		LatexEntity(void);
		LatexEntity(const std::string & value);
		LatexEntity(const LatexEntity & orig);
		~LatexEntity(void);
		std::string getString(void) const;
		const std::string & getName(void) const;
		int countIndices(void) const;
		int countExponents(void) const;
		int countParameters(void) const;
		LatexFormulas * getIndice(int id);
		LatexFormulas * getExponent(int id);
		LatexFormulas * getParameter(int id);
		const LatexFormulas * getIndice(int id) const;
		const LatexFormulas * getExponent(int id) const;
		const LatexFormulas * getParameter(int id) const;
		LatexFormulasVector & getIndices(void);
		LatexFormulasVector & getExponents(void);
		LatexFormulasVector & getParameters(void);
		void parse( const std::string& value ) throw(LatexException);
		void parse( LatexParsorContext & context);
		void clear(void);
		void dumpAsXml(std::ostream & out, int depth = 0) const;
		void dumpAsTree(std::ostream & out, int depth = 0) const;
		bool isOnlyOneName(void) const;
		void setExtraInfo(const std::string & key,void * value,bool allowOverride = false);
		void deleteInfo(const std::string & key, bool throwOnError = true);
		void * getExtraInfo(const std::string & key,bool throwOnError = true);
		bool hasInfo(const std::string & key) const;
		LatexEntity & operator=(const LatexEntity & value);
	protected:
		void copy(const LatexEntity & value);
		static void copy(LatexFormulasVector & vectorOut,const LatexFormulasVector & vectorOrig);
		static void writeFormulaList(std::ostream & out,const LatexFormulasVector & value,const std::string & sep=",",const std::string & open = "{",const std::string & close =  "}", bool forceOpenClose = false);
		void parseSubGroup(LatexParsorContext & context);
		void parseStandard(LatexParsorContext & context);
		static std::string extractName( LatexParsorContext& context );
		LatexFormulas* extractSubFormula( LatexParsorContext& context );
		void extractSubAndSuperScript(LatexParsorContext & context);
		void extractParameters(LatexParsorContext & context);
		static void dumpAsXml(std::ostream & out, int depth,const std::string & name,const LatexFormulasVector & list);
		static void dumpAsTree(std::ostream & out, int depth,const std::string & name,const LatexFormulasVector & list);
		static bool needProtection( const LatexFormulasVector& value );
	public:
		LatexEntity & operator=(const std::string & value);
		friend std::ostream & operator << (std::ostream & out,const LatexEntity & value);
	public:
		std::string name;
		LatexFormulasVector indices;
		LatexFormulasVector exponents;
		LatexFormulasVector parameters;
		CMRLatexExtraInfo extraInfos;
};

}

#endif //CMR_COMPILER_LATEX_ENTITY_H
