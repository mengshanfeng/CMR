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
class CMRProjectIterator : public CMREntity
{
	public:
		CMRProjectIterator ( const std::string& latexName, const std::string& longName ,int start, int end);
		void printDebug(void) const;
		void printCPPCode(void) const;
// 	private:
		int start;
		int end;
};

#endif //CMR_PROJECT_ITERATOR_H
