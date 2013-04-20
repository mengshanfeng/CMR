/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_PROJECT_VARIABLE_H
#define CMR_PROJECT_VARIABLE_H


#include "CMRProjectEntity.h"
#include "../parsor/CMRParsorBasics.h"

/*********************  STRUCT  *********************/
struct CMRProjectVariableDef
{
	CMRProjectVariableDef(const std::string & name,int dims,int start = 0);
	std::string name;
	int dims;
	int start;
};

/*********************  TYPES  **********************/
typedef std::vector<CMRProjectVariableDef> CMRProjectVariableDefVector;

/*********************  CLASS  **********************/
class CMRProjectVariable : public CMRProjectEntity
{
	public:
		CMRProjectVariable ( const std::string& latexName, const std::string& longName, const std::string& type );
		void addDim( const std::string& name,int size, int start = 0);
		virtual void genUsageCCode ( std::ostream& out, const CMRProjectContext& context, CMRLatexEntity2& entity, bool write = false ) const;
		virtual void genDefinitionCCode ( std::ostream& out, const CMRProjectContext& context ) const;
		std::string getTypeWithDims(void) const;
		std::ostream & genCPPAccessorDefinition(std::ostream & out);
		std::ostream & genCPPAccessorAddVar(std::ostream & out);
		std::ostream & genCPPAccessorConstrSys(std::ostream& out, int id);
		std::ostream & genCPPAccessorConstrAcc(std::ostream & out);
	private:
		int ghostDepths;
		std::string type;
		std::string memoryModel;
		CMRProjectVariableDefVector defs;
};

#endif //CMR_PROJECT_VARIABLE_H
