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

/*********************  TYPES  **********************/
class CMRProjectTransformation;

/*********************  CLASS  **********************/
class CMRProjectDefinition : public CMRProjectEntity
{
	public:
		CMRProjectDefinition ( const std::string& latexName, const std::string& longName,CMRProjectContext * parentContext = NULL);
		CMRProjectAction & addSubBlock(std::string loopDescr,std::string parameter,CMRProjectCodeTreeInsert location = CMR_INSERT_LAST_CHILD);
		CMRProjectEquation& addEquation( const std::string& latexName, const std::string& longName, const std::string& compute,CMRProjectCodeTreeInsert location = CMR_INSERT_LAST_CHILD);
		virtual void addIndice ( const std::string& name, CMRCaptureType capture = CMR_CAPTURE_NONE );
		virtual void madeCaptureIndice ( const std::string name, CMRCaptureType capture );
		void printDebug(void) const;
		void printCPPCode(CMRProjectContext & context) const;
		void runTransformation(CMRProjectTransformation & transf);
		virtual std::ostream& genUsageCCode ( std::ostream& out, const CMRProjectContext& context, CMRCompiler::LatexEntityOld& entity ) const;
	private:
		CMRProjectAction ops;
};

#endif //CMR_PROJECT_DEFINITION_H
