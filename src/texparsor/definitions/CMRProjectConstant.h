
#ifndef CMR_PROJECT_CONSTANT_H
#define CMR_PROJECT_CONSTANT_H

/********************  HEADERS  *********************/
#include "CMRProjectEntity.h"

/*********************  CLASS  **********************/
class CMRProjectConstant : public CMRProjectEntity
{
	public:
		CMRProjectConstant( const std::string& latexName, const std::string& longName );
		void loadValues(const std::string & data,int dimensions);
		virtual void printDebug(std::ostream & out) const;
		virtual void genDefinitionCCode ( std::ostream& out, const CMRProjectContext& context ,int indent = 0) const;
		virtual void genUsageCCode ( std::ostream& out, const CMRProjectContext& context, const CMRLatexEntity2& entity, bool write = false ) const;
		void printValues(std::ostream & out) const;
	protected:
		void addDimension(int size);
		void loadValuesScalar(const std::string & data);
		void loadValuesVector(const std::string & data);
		void loadValuesMatrix(const std::string & data);
	protected:
		CMRConstantDimensionsVector dims;
		CMRConstantValueVector values;
};

#endif //CMR_PROJECT_CONSTANT_H
