/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMPILER_PROJECT_CALL_ACTION_H
#define CMR_COMPILER_PROJECT_CALL_ACTION_H

/********************  HEADERS  *********************/
#include <vector>
#include "common/LangDef.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  TYPES  **********************/
typedef std::vector<std::string> CallActionZoneVector;
typedef std::map<std::string,std::string> CallActionParameters;

/*********************  CLASS  **********************/
class ProjectCallAction
{
	public:
		ProjectCallAction(const std::string & actionName);
		ProjectCallAction & addZone(const std::string & zone);
		ProjectCallAction & addParameter(const std::string & name,const std::string & value);
		void genCode( std::ostream& out, CMRCompiler::LangDef& lang, const std::string& addFName, int id, int indent ) const;
	private:
		std::string actionName;
		CallActionZoneVector zones;
		CallActionParameters parameters;
};

}

#endif //CMR_COMPILER_PROJECT_CALL_ACTION_H
