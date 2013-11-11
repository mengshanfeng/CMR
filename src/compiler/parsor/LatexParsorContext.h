/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMPILER_LATEX_PARSOR_CONTEXT_H
#define CMR_COMPILER_LATEX_PARSOR_CONTEXT_H

/********************  HEADERS  *********************/
#include <string>
#include "LatexException.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  CLASS  **********************/
class LatexParsorContext
{
	public:
		LatexParsorContext(const std::string & value);
		LatexParsorContext(const LatexParsorContext * parent,int start,int end);
		int getPosition(void) const;
		int move( int delta );
		void skipWhiteSpace(void);
		bool isEnd(void) const;
		LatexParsorContext & operator++(void);
		char getCurrent(void) const;
		char getCurAndMove( int delta = 1 );
		bool startBy(const std::string & value) const;
		bool startBy(char value) const;
		LatexParsorContext extractSubZone( char delimOpen, char delimClose );
		void fatal(const std::string & message) const throw(LatexException);
		const std::string & getValue(void) const;
	private:
		int position;
		std::string value;
		const LatexParsorContext * parent;
		int relToParent;
};

};

#endif //CMR_COMPILER_LATEX_PARSOR_CONTEXT_H
