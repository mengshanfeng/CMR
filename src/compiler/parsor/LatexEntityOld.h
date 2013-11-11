/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMPILER_LATEX_ENTITY_OLD_H
#define CMR_COMPILER_LATEX_ENTITY_OLD_H

/********************  HEADERS  *********************/
#include <vector>
#include <string>

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  TYPES  **********************/
class LatexParsorContext;
struct LatexEntityOld;
struct LatexFormulasOld;
typedef std::vector<LatexFormulasOld*> LatexFormulasListOld;
typedef std::vector<LatexEntityOld*> LatexEntityVectorOld;

/*********************  STRUCT  *********************/
struct LatexFormulasOld
{
	LatexEntityVectorOld childs;
	std::string string;
};

/*********************  STRUCT  *********************/
struct LatexEntityOld
{
	LatexEntityOld(void);
	~LatexEntityOld(void);
	void print(int depth, int pos);
	int countIndices(void) const;
	LatexFormulasListOld getIndices( void );
	std::string getString(void) const;
	std::string name;
	std::string parent;
	LatexFormulasListOld subscript;
	LatexFormulasListOld superscript;
	LatexFormulasListOld params;
	int from;
	int to;
	std::string totalValue;
	std::string subscriptTotalValue;
	std::string superscriptTotalValue;
	std::string argsTotalValue;
};

/*******************  FUNCTION  *********************/
void cmrPrintFormula(const LatexFormulasListOld & formulas,int depth);
void cmrPrintFormula(const LatexFormulasOld & formula,int depth);

}

#endif //CMR_LATEX_ENTITY_OLD_H
