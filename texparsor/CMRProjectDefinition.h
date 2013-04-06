/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_PROJECT_DEFINITION_H
#define CMR_PROJECT_DEFINITION_H

/********************  HEADERS  *********************/
#include "CMRProjectEntity.h"
#include "CMRProjectAction.h"

/*********************  CLASS  **********************/
class CMRProjectDefinition : public CMREntity
{
	public:
		CMRProjectDefinition ( const std::string& latexName, const std::string& longName);
		CMRProjectAction & addSubBlock(std::string loopDescr,std::string parameter,CMRProjectCodeTreeInsert location = CMR_INSERT_LAST_CHILD);
		CMRProjectEquation& addEquation( const std::string& latexName, const std::string& longName, const std::string& compute,CMRProjectCodeTreeInsert location = CMR_INSERT_LAST_CHILD);
		void printDebug(void) const;
		void printCPPCode(void) const;
	private:
		CMRProjectAction ops;
};

#endif //CMR_PROJECT_DEFINITION_H
