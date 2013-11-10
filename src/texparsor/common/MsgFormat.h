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
#include "Common.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  CLASS  **********************/
class MsgFormat
{
	public:
		MsgFormat(const std::string & format);
		MsgFormat & arg(const std::string & arg);
		template<class T> MsgFormat & arg(const T & arg);
		friend std::ostream& operator<< (std::ostream &out, const MsgFormat & msg);
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
MsgFormat& MsgFormat::arg ( const T& arg)
{
	std::stringstream tmp;
	tmp << arg;
	return this->arg(tmp.str());
}

};

#endif //CMR_MSG_FORMAT_H
