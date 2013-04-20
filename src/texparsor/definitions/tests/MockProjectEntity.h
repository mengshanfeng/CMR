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
#include <CMRProjectEntity.h>

/*********************  CLASS  **********************/
class MockProjectEntity : public CMRProjectEntity
{
	public:
		MockProjectEntity ( const std::string& latexName, const std::string& longName ) : CMRProjectEntity(latexName,longName) {};
		virtual void genDefinitionCCode ( std::ostream& out, const CMRProjectContext& context ) const {out << "double " << getLongName()<< ";" << std::endl;};
		virtual void genUsageCCode ( std::ostream& out, const CMRProjectContext& context, CMRLatexEntity2& entity, bool write = false ) const {out << getLongName() ;};
};

#endif //MOCK_PROJECT_ENTITY_H
