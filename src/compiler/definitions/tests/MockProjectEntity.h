/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef MOCK_PROJECT_ENTITY_H
#define MOCK_PROJECT_ENTITY_H

/********************  HEADERS  *********************/
#include <ostream>
#include <string>
#include <ProjectEntity.h>

using namespace CMRCompiler;

/*********************  CLASS  **********************/
class MockProjectEntity : public ProjectEntity
{
	public:
		MockProjectEntity ( const std::string& latexName, const std::string& longName ) : ProjectEntity(latexName,longName) {};
		virtual void genDefinitionCCode ( std::ostream& out, const ProjectContext& context, int indent ) const {out << "double " << getLongName()<< ";" << std::endl;};
		virtual void genUsageCCode ( std::ostream& out, const ProjectContext& context, const LatexEntity& entity, bool write = false ) const {out << getLongName() ;};
};

#endif //MOCK_PROJECT_ENTITY_H
