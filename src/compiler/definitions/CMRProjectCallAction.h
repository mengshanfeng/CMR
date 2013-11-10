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
#include <string>
#include <vector>
#include <ostream>
#include <common/LangDef.h>

/*********************  TYPES  **********************/
typedef std::vector<std::string> CMRCallZoneVector;
typedef std::map<std::string,std::string> CMRCallParameters;

/*********************  CLASS  **********************/
class CMRProjectCallAction
{
	public:
		CMRProjectCallAction(const std::string & actionName);
		CMRProjectCallAction & addZone(const std::string & zone);
		CMRProjectCallAction & addParameter(const std::string & name,const std::string & value);
		void genCode( std::ostream& out, CMRCompiler::LangDef& lang, int id ) const;
	private:
		std::string actionName;
		CMRCallZoneVector zones;
		CMRCallParameters parameters;
};

#endif //CMR_COMPILER_PROJECT_CALL_ACTION_H
