
#ifndef CMR_PROJECT_CONSTANT_H
#define CMR_PROJECT_CONSTANT_H

/********************  HEADERS  *********************/
#include "CMRProjectEntity.h"

/*********************  CLASS  **********************/
//<param short_name="R" long_name="relax_param" type="double" unit="??" estimated_min="0" estimated_max="1.3" value="1" comment=""/>
class CMREntityConstant : public CMREntity
{
	public:
		CMREntityConstant( const std::string& latexName, const std::string& longName );
		void loadValues(const std::string & data,int dimensions);
		void printDebug(void) const;
		void printCPPCode(void) const;
	protected:
		void addDimension(int size);
		void loadValuesScalar(const std::string & data);
		void loadValuesVector(const std::string & data);
		void loadValuesMatrix(const std::string & data);
	public:
		std::string unit;
		std::string estimatedMin;
		std::string estimatedMax;
		std::string comment;
		CMRConstantDimensionsVector dims;
		CMRConstantValueVector values;
};

#endif //CMR_PROJECT_CONSTANT_H
