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
typedef std::vector<CMRLatexEntity*> CMRLatexFormulas;
typedef std::vector<CMRLatexFormulas*> CMRLatexFormulasList;

/*********************  STRUCT  *********************/
struct CMRLatexEntity
{
	CMRLatexEntity(void);
	~CMRLatexEntity(void);
	void print(int depth, int pos);
	std::string name;
	std::string parent;
	CMRLatexFormulas subscript;
	CMRLatexFormulas superscript;
	CMRLatexFormulasList params;
	int from;
	int to;
	std::string totalValue;
	std::string subscriptTotalValue;
	std::string superscriptTotalValue;
	std::string argsTotalValue;
};

/*******************  FUNCTION  *********************/
void cmrPrintFormula(const CMRLatexFormulas & formula,int depth);

#endif //CMR_LATEX_ENTITY_H
