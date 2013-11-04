/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_MSG_FORMAT_H
#define CMR_MSG_FORMAT_H

/********************  HEADERS  *********************/
#include <string>
#include <ostream>
#include <exception>
#include <sstream>

/*********************  CLASS  **********************/
class CMRMsgException : public std::exception
{
	public:
		CMRMsgException(void) throw();
		CMRMsgException(std::string message) throw();
		virtual ~CMRMsgException(void) throw();
		virtual const char* what() const  throw();
		virtual std::string getMessage(void) const;
		void fatal();
	private:
		std::string message;
};

/*********************  CLASS  **********************/
class CMRMsgFormat
{
	public:
		CMRMsgFormat(const std::string & format);
		CMRMsgFormat & arg(const std::string & arg);
		template<class T> CMRMsgFormat & arg(const T & arg);
		friend std::ostream& operator<< (std::ostream &out, const CMRMsgFormat & msg);
		void fatal(void) const;
		void warning(void) const;
		void debug(void) const;
		void exception(void) const;
		const std::string & str(void) const;
	protected:
		std::string value;
		int cntArg;
};

/*******************  FUNCTION  *********************/
template <class T>
CMRMsgFormat& CMRMsgFormat::arg ( const T& arg)
{
	std::stringstream tmp;
	tmp << arg;
	return this->arg(tmp.str());
}


#endif //CMR_MSG_FORMAT_H
