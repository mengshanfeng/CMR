/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMPILER_LATEX_FORMULA_H
#define CMR_COMPILER_LATEX_FORMULA_H

/********************  HEADERS  *********************/
#include <ostream>
#include <vector>
#include <string>
// #include "CMRLatexException.h"
#include "LatexEntity.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  TYPES  **********************/
class LatexParsorContext;
class LatexEntity;
class LatexFormulas;
typedef std::vector<LatexEntity*> LatexEntityVector;
typedef std::vector<LatexFormulas*> LatexFormulasVector;

/*********************  CLASS  **********************/
class LatexFormulas : public LatexEntityVector
{
	public:
		LatexFormulas(void);
		LatexFormulas(const std::string & value);
		LatexFormulas(const LatexFormulas & orig);
		~LatexFormulas(void);
		void parse(const std::string & value);
		void parse(LatexParsorContext & context);
		void split(LatexFormulasVector & formulas,const std::string & sep = ",");
		std::string getString(void) const;
		friend std::ostream & operator << (std::ostream & out,const LatexFormulas & value);
		void dumpAsXml(std::ostream & out, int depth = 0) const;
		void dumpAsTree(std::ostream & out, int depth = 0) const;
		void clear(void);
		bool isOnlyOneName(void) const;
		bool isSimpleEntity(void) const;
		void setExtraInfo(const std::string & key,void * value,bool allowOverride = false);
		void deleteInfo(const std::string & key, bool throwOnError = true);
		void * getExtraInfo(const std::string & key,bool throwOnError = true);
		bool hasInfo(const std::string & key) const;
		LatexFormulas & operator=(const LatexFormulas & orig);
	private:
		void copy(const LatexFormulas & orig);
		CMRLatexExtraInfo extraInfos;
};

}

#endif //CMR_COMPILER_LATEX_FORMULA_H
