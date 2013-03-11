/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_ENTITY_MATCHING_H
#define CMR_ENTITY_MATCHING_H

/********************  HEADERS  *********************/
#include <string>
#include "CMRLatexEntity.h"
#include "CMREqDepMatrix.h"

/*********************  STRUCT  *********************/
struct CMREntityCellMatching
{
	CMREntityCellMatching(bool matching = false);
	void printDebug(void) const;
	bool match;
	bool depInfo;
	int dx;
	int dy;
	bool write;
	bool out;
};

/*******************  FUNCTION  *********************/
bool cmrEntityExactMatch(const CMRLatexEntity & e1,const CMRLatexEntity & e2,bool subsup = true);
const CMRLatexFormulas * cmrEntityExtractSubscriptParam(const CMRLatexEntity & entity,int id);
int cmrApplyOp(int orig,char op,int value);
int cmrFormulaExtractDelta(const CMRLatexFormulas * formula,const std::string & varname);
CMREntityCellMatching cmrEntityCellMatch(const CMRLatexEntity & entity,const std::string & name);
void cmrExtractDeps(CMREqDepMatrix & matrix,const CMRLatexEntity & f,const std::string & varname);
void cmrExtractDeps(CMREqDepMatrix & matrix,const CMRLatexFormulas & f,const std::string & varname);

#endif //CMR_ENTITY_MATCHING_H
