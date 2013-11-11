
#ifndef CMR_PROJECT_CONSTANT_H
#define CMR_PROJECT_CONSTANT_H

/********************  HEADERS  *********************/
#include "CMRProjectEntity.h"

/*********************  TYPES  **********************/
class CMRTransformation;
class CMRModelBasedReplacement;

/*********************  TYPES  **********************/
typedef std::vector<CMRCompiler::LatexFormulas> CMRConstantFormulaVector;

/*********************  CLASS  **********************/
class CMRProjectConstant : public CMRProjectEntity
{
	public:
		CMRProjectConstant( const std::string& latexName, const std::string& longName );
		void loadValues(const std::string & data,int dimensions);
		virtual void printDebug(std::ostream & out) const;
		virtual void genDefinitionCCode ( std::ostream& out, const CMRProjectContext& context ,int indent = 0) const;
		virtual void genUsageCCode ( std::ostream& out, const CMRProjectContext& context, const CMRCompiler::LatexEntity& entity, bool write = false ) const;
		void printValues(std::ostream & out) const;
	protected:
		void addDimension(int size);
		void loadValuesScalar(const std::string & data);
		void loadValuesVector(const std::string & data);
		void loadValuesMatrix(const std::string & data);
		void transform(void);
		void transform(CMRCompiler::LatexFormulas& formula, CMRModelBasedReplacement & action);
		void transform(CMRCompiler::LatexEntity& entity, CMRModelBasedReplacement & action);
	protected:
		CMRConstantDimensionsVector dims;
		CMRConstantFormulaVector formulas;
};

#endif //CMR_PROJECT_CONSTANT_H
