/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_LATEX_FORMULA_H
#define CMR_LATEX_FORMULA_H

/********************  HEADERS  *********************/
#include <ostream>
#include <vector>
#include <string>
// #include "CMRLatexException.h"
#include "CMRLatexEntity2.h"

/*********************  TYPES  **********************/
class CMRLatexParsorContext;
class CMRLatexEntity2;
class CMRLatexFormulas2;
typedef std::vector<CMRLatexEntity2*> CMRLatexEntityVector2;
typedef std::vector<CMRLatexFormulas2*> CMRLatexFormulasVector2;

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
		bool isOnlyOneName(void) const;
		bool isSimpleEntity(void) const;
		void setExtraInfo(const std::string & key,void * value,bool allowOverride = false);
		void deleteInfo(const std::string & key, bool throwOnError = true);
		void * getExtraInfo(const std::string & key,bool throwOnError = true);
		bool hasInfo(const std::string & key) const;
	private:
		CMRLatexFormulas2(const CMRLatexFormulas2 & orig);
		CMRLatexFormulas2 & operator=(const CMRLatexFormulas2 & orig);
		CMRLatexExtraInfo extraInfos;
};

#endif //CMR_LATEX_FORMULA_H
