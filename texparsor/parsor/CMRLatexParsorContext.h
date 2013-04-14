/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_LATEX_PARSOR_CONTEXT_H
#define CMR_LATEX_PARSOR_CONTEXT_H

/********************  HEADERS  *********************/
#include <string>
#include "CMRLatexException.h"

/*********************  CLASS  **********************/
class CMRLatexParsorContext
{
	public:
		CMRLatexParsorContext(const std::string & value);
		CMRLatexParsorContext(const CMRLatexParsorContext * parent,int start,int end);
		int getPosition(void) const;
		int move( int delta );
		void skipWhiteSpace(void);
		bool isEnd(void) const;
		CMRLatexParsorContext & operator++(void);
		char getCurrent(void) const;
		char getCurAndMove( int delta = 1 );
		bool startBy(const std::string & value) const;
		bool startBy(char value) const;
		CMRLatexParsorContext extractSubZone( char delimOpen, char delimClose );
		void fatal(const std::string & message) const throw(CMRLatexException);
		const std::string & getValue(void) const;
	private:
		int position;
		std::string value;
		const CMRLatexParsorContext * parent;
		int relToParent;
};

#endif //CMR_LATEX_PARSOR_CONTEXT_H
