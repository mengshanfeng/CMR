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
#include <vector>
#include <string>

/*********************  TYPES  **********************/
struct CMRLatexEntity;
struct CMRLatexFormulas;
typedef std::vector<CMRLatexFormulas*> CMRLatexFormulasList;
typedef std::vector<CMRLatexEntity*> CMRLatexEntityVector;

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

/*******************  FUNCTION  *********************/
void cmrPrintFormula(const CMRLatexFormulasList & formulas,int depth);
void cmrPrintFormula(const CMRLatexFormulas & formula,int depth);

#endif //CMR_LATEX_ENTITY_H
