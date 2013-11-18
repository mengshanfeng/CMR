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
#include "ProjectEntity.h"
#include "CMRProjectCode.h"
// #include "ProjectIterator.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  TYPES  **********************/
class CMRTransformation;
class ProjectIterator;

/*********************  CLASS  **********************/
class ProjectDefinition : public ProjectEntity
{
	public:
		ProjectDefinition ( const std::string& latexName, const std::string& longName,ProjectContext * parentContext = NULL);
		virtual void printDebug(std::ostream & out) const;
		virtual void genDefinitionCCode ( std::ostream& out, const ProjectContext& context, int padding = 0 ) const;
		virtual void genUsageCCode ( std::ostream& out, const ProjectContext& context, const CMRCompiler::LatexEntity& entity, bool write = false ) const;
		void runTransformation(CMRTransformation & transf);
		CMRProjectCodeEquation & addEquation(const std::string& latexName, const std::string& compute,const std::string & op = "=");
		CMRProjectCodeEquation & addEquation(const std::string& eq);
		CMRProjectCodeIteratorLoop& addIteratorLoop( const std::string& iterator);
		CMRProjectLocalVariable & addLocalVariable(const std::string & latexName, const std::string & longName,const std::string &type, const std::string & defaultValue);
		ProjectIterator & addIterator(const std::string & latexName, const std::string & longName, int start, int end);
		ProjectContext & getContext(void);
		CMRProjectCodeEntry * insert(CMRProjectCodeEntry * entry,ProjectCodeTreeInsert location = CMR_INSERT_LAST_CHILD);
		void setCaptureSize(const std::string & name,const std::string & type,int size = 0);
	protected:
		void genParameterListForDef(std::ostream& out,const ProjectCaptureDefMap & map) const;
		void genParameterListForUsage( std::ostream& out, const ProjectCaptureDefMap& map, ProjectCaptureMap& capture,const ProjectContext & context ) const;
		virtual void onUpdateCaptureType ( const std::string& name, CaptureType captureType );
		/*virtual void addIndice ( const std::string& name, CMRCaptureType capture = CMR_CAPTURE_NONE );
		virtual void madeCaptureIndice ( const std::string name, CMRCaptureType capture );
		void printDebug(void) const;
		void printCPPCode(CMRProjectContext & context) const;
		void runTransformation(CMRProjectTransformation & transf);
		virtual std::ostream& genUsageCCode ( std::ostream& out, const CMRProjectContext& context, CMRLatexEntity& entity ) const;*/
	private:
		ProjectContext parametersContext;
		CMRProjectCodeRootNode ops;
		std::map<std::string,int> sizes;
		std::map<std::string,std::string> types;
};

}

#endif //CMR_PROJECT_DEFINITION_H
