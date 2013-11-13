
#ifndef CMR_COMPILER_PROJECT_CONSTANT_H
#define CMR_COMPILER_PROJECT_CONSTANT_H

/********************  HEADERS  *********************/
#include "ProjectEntity.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  TYPES  **********************/
class CMRTransformation;
class CMRModelBasedReplacement;

/*********************  TYPES  **********************/
typedef std::vector<LatexFormulas> ConstantFormulaVector;

/*********************  CLASS  **********************/
class ProjectConstant : public ProjectEntity
{
	public:
		ProjectConstant( const std::string& latexName, const std::string& longName, const std::string & type );
		void loadValues(const std::string & data,int dimensions);
		virtual void printDebug(std::ostream & out) const;
		virtual void genDefinitionCCode ( std::ostream& out, const ProjectContext& context ,int indent = 0) const;
		virtual void genUsageCCode ( std::ostream& out, const ProjectContext& context, const LatexEntity& entity, bool write = false ) const;
		void printValues(std::ostream & out) const;
	protected:
		void addDimension(int size);
		void loadValuesScalar(const std::string & data);
		void loadValuesVector(const std::string & data);
		void loadValuesMatrix(const std::string & data);
		void transform(void);
		void transform(LatexFormulas& formula, CMRModelBasedReplacement & action);
		void transform(LatexEntity& entity, CMRModelBasedReplacement & action);
	protected:
		ConstantDimensionsVector dims;
		ConstantFormulaVector formulas;
		std::string type;
};

}

#endif //CMR_COMPILER_PROJECT_CONSTANT_H
