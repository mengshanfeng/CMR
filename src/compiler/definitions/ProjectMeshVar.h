/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMPILER_PROJECT_VARIABLE_H
#define CMR_COMPILER_PROJECT_VARIABLE_H

/********************  HEADERS  *********************/
#include "ProjectEntityNew.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  STRUCT  *********************/
struct ProjectMeshVarDef
{
	ProjectMeshVarDef(const std::string & name,int dims,int start = 0);
	std::string name;
	int dims;
	int start;
};

/*********************  TYPES  **********************/
typedef std::vector<ProjectMeshVarDef> ProjectMeshVarDefVector;

/*********************  CLASS  **********************/
class ProjectMeshVar : public ProjectEntityNew
{
	public:
		ProjectMeshVar ( const std::string& latexName, const std::string& longName, const std::string& type, int ghost );
		void addDim( const std::string& name,int size, int start = 0);
		void setMemoryModel(const std::string & value);
		virtual void genUsageCCode ( std::ostream& out, const ProjectContext& context, const LatexEntity& entity, bool write = false ) const;
		virtual void genDefinitionCCode ( std::ostream& out, const ProjectContext& context, int indent = 0 ) const;
		std::string getTypeWithDims(void) const;
		std::ostream & genCPPAccessorDefinition(std::ostream & out);
		std::ostream & genCPPAccessorAddVar(std::ostream & out);
		std::ostream & genCPPAccessorConstrSys(std::ostream& out, int id);
		std::ostream & genCPPAccessorConstrAcc(std::ostream & out);
	private:
		int ghostDepths;
		std::string type;
		std::string memoryModel;
		ProjectMeshVarDefVector defs;
};

}

#endif //CMR_COMPILER_PROJECT_VARIABLE_H
