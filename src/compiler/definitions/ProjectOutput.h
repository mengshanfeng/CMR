/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_PROJECT_OUTPUT_H
#define CMR_PROJECT_OUTPUT_H

/********************  HEADERS  *********************/
#include <string>
#include <vector>
#include <ostream>
#include "parsor/LatexFormula.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  TYPES  **********************/
class CMRProjectContext;
	
/*********************  CLASS  **********************/
class ProjectOutputEntry
{
	public:
		ProjectOutputEntry(const std::string & name, const std::string & type, const std::string & value);
		void genCode( std::ostream& out, const CMRProjectContext& context, const std::string& codeType );
	private:
		std::string name;
		std::string type;
		LatexFormulas * value;
};

/*********************  TYPES  **********************/
typedef std::vector<ProjectOutputEntry> ProjectOutputEntryVector;

/*********************  CLASS  **********************/
class ProjectOutput
{
	public:
		void addEntry(const std::string & name, const std::string & type, const std::string & value);
		void genCode( std::ostream& out, const CMRProjectContext& context, const std::string& codeType );
	private:
		ProjectOutputEntryVector entries;
};

}

#endif //CMR_PROJECT_OUTPUT_ENTRY_H
