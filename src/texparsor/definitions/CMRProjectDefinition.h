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
#include "CMRProjectCode.h"

/*********************  TYPES  **********************/
class CMRTransformation;

/*********************  CLASS  **********************/
class CMRProjectDefinition : public CMRProjectEntity
{
	public:
		CMRProjectDefinition ( const std::string& latexName, const std::string& longName,CMRProjectContext * parentContext = NULL);
		void printDebug(std::ostream & out) const;
		virtual void genDefinitionCCode ( std::ostream& out, const CMRProjectContext& context ) const;
		virtual void genUsageCCode ( std::ostream& out, const CMRProjectContext& context, const CMRLatexEntity2& entity, bool write = false ) const;
		void runTransformation(CMRTransformation & transf);
		CMRProjectCodeEquation & addEquation(const std::string& latexName, const std::string& compute,const std::string & op = "=");
		CMRProjectCodeIteratorLoop& addIteratorLoop( const std::string& iterator);
		CMRProjectLocalVariable & addLocalVariable(const std::string & latexName, const std::string & longName,const std::string &type, const std::string & defaultValue);
	protected:
		static void genParameterListForDef(std::ostream& out,const CMRProjectCaptureDefMap & map);
		static void genParameterListForUsage( std::ostream& out, const CMRProjectCaptureDefMap& map, CMRProjectCaptureMap& capture );
		/*virtual void addIndice ( const std::string& name, CMRCaptureType capture = CMR_CAPTURE_NONE );
		virtual void madeCaptureIndice ( const std::string name, CMRCaptureType capture );
		void printDebug(void) const;
		void printCPPCode(CMRProjectContext & context) const;
		void runTransformation(CMRProjectTransformation & transf);
		virtual std::ostream& genUsageCCode ( std::ostream& out, const CMRProjectContext& context, CMRLatexEntity& entity ) const;*/
	private:
		CMRProjectCodeNode ops;
};

#endif //CMR_PROJECT_DEFINITION_H
