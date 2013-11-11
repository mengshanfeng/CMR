/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMPILER_EXCEPTION_H
#define CMR_COMPILER_EXCEPTION_H

/********************  HEADERS  *********************/
#include <string>
#include <exception>

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  CLASS  **********************/
class Exception : public std::exception
{
	public:
		Exception(void) throw();
		Exception(std::string message) throw();
		virtual ~Exception(void) throw();
		virtual const char* what() const  throw();
		virtual std::string getMessage(void) const;
		void fatal();
	private:
		std::string message;
};

}

#endif //CMR_COMPILER_EXCEPTION_H
