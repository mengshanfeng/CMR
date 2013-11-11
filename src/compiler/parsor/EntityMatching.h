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
#include "LatexEntityOld.h"
#include "EqDepMatrix.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  STRUCT  *********************/
struct EntityCellMatching
{
	EntityCellMatching(bool matching = false);
	void printDebug(void) const;
	bool match;
	bool depInfo;
	int dx;
	int dy;
	bool write;
	bool out;
};

/*******************  FUNCTION  *********************/
bool cmrEntityExactMatch(const LatexEntityOld & e1,const LatexEntityOld & e2,bool subsup = true);
const LatexFormulasOld * cmrEntityExtractSubscriptParam(const LatexEntityOld & entity,int id);
int cmrApplyOp(int orig,char op,int value);
int cmrFormulaExtractDelta(const LatexFormulasOld * formula,const std::string & varname);
EntityCellMatching cmrEntityCellMatch(const LatexEntityOld & entity,const std::string & name);
void cmrExtractDeps(EqDepMatrix & matrix,const LatexEntityOld & f,const std::string & varname);
void cmrExtractDeps(EqDepMatrix & matrix,const LatexFormulasOld & f,const std::string & varname);

}

#endif //CMR_ENTITY_MATCHING_H
