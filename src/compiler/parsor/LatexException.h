/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMPILER_LATEX_EXCEPTION_H
#define CMR_COMPILER_LATEX_EXCEPTION_H

/********************  HEADERS  *********************/
#include <exception>
#include <string>

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  CLASS  **********************/
class LatexException : public std::exception
{
	public:
		LatexException(void) throw();
		LatexException(std::string message) throw();
		virtual ~LatexException(void) throw();
		virtual const char* what() const  throw();
		virtual std::string getMessage(void) const;
		void fatal(void);
	private:
		std::string message;
};

}

#endif //CMR_COMPILER_LATEX_EXCEPTION_H
