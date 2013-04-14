/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_LATEX_EXCEPTION_H
#define CMR_LATEX_EXCEPTION_H

/********************  HEADERS  *********************/
#include <exception>
#include <string>

/*********************  CLASS  **********************/
class CMRLatexException : public std::exception
{
	public:
		CMRLatexException(void) throw();
		CMRLatexException(std::string message) throw();
		virtual ~CMRLatexException(void) throw();
		virtual const char* what() const  throw();
		virtual std::string getMessage(void) const;
		void fatal();
	private:
		std::string message;
};

#endif //CMR_LATEX_EXCEPTION_H
