/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef MOCK_PROJECT_ENTITY_NEW_H
#define MOCK_PROJECT_ENTITY_NEW_H

/********************  HEADERS  *********************/
#include <ostream>
#include <string>
#include <ProjectEntityNew.h>

using namespace CMRCompiler;

/*********************  CLASS  **********************/
class MockProjectEntityNew : public ProjectEntityNew
{
	public:
		MockProjectEntityNew ( const std::string& latexName, const std::string& longName ) : ProjectEntityNew(latexName,longName) {};
		virtual void genDefinitionCCode ( std::ostream& out, const ProjectContext& context, int indent ) const {out << "double " << getLongName()<< ";" << std::endl;};
		virtual void genUsageCCode ( std::ostream& out, const ProjectContext& context, const LatexEntity& entity, bool write = false ) const {out << getLongName() ;};
};

#endif //MOCK_PROJECT_ENTITY_NEW_H
