/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_PROJECT_ITERATOR_H
#define CMR_PROJECT_ITERATOR_H

/********************  HEADERS  *********************/
#include "CMRProjectEntity.h"

/*********************  CLASS  **********************/
class CMRProjectIterator : public CMRProjectEntity
{
	public:
		CMRProjectIterator ( const std::string& latexName, const std::string& longName ,int start, int end);
		void printDebug(std::ostream & out) const;
		virtual void genDefinitionCCode ( std::ostream& out, const CMRProjectContext& context ,int indent = 0) const;
		virtual void genUsageCCode ( std::ostream& out, const CMRProjectContext& context, const CMRLatexEntity2& entity, bool write = false ) const;
	private:
		int start;
		int end;
};

#endif //CMR_PROJECT_ITERATOR_H
