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
		virtual std::ostream& genDefinitionCCode ( std::ostream& out, const CMRProjectContext& context ) {return out;};
		virtual std::ostream& genUsageCCode ( std::ostream& out, const CMRProjectContext& context, CMRLatexEntity2& entity ) {return out;};
};

#endif //MOCK_PROJECT_ENTITY_H
