/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_PROJECT_ENTIT_NEW_H
#define CMR_PROJECT_ENTIT_NEW_H

/********************  HEADERS  *********************/
#include <string>
#include <parsor/FormulaMatcher.h>
#include "ProjectEntityOld.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  TYPES  **********************/
class ProjectContext;

/*********************  CLASS  **********************/
class ProjectEntityNew : public IProjectEntity, public FormulaMatcher
{
	public:
		ProjectEntityNew(const std::string & latexName,const std::string & longName);
		virtual ~ProjectEntityNew(void);
		std::string getLatexName(void) const;
		const std::string & getLongName(void) const;
		
		//maybe temporaty
		virtual bool match ( const LatexEntity& entity ) const;
		
		//to overload
		virtual void genDefinitionCCode(std::ostream& out, const ProjectContext& context, int padding = 0) const = 0;
		virtual void genUsageCCode(std::ostream& out, const ProjectContext& context, const LatexEntity& entity, bool write = false) const = 0;
		virtual void printDebug(std::ostream & out) const;
	private:
		std::string longName;
};

}

#endif //CMR_PROJECT_ENTIT_NEW_H
