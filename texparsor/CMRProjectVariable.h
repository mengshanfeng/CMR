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
#include "parsor/CMRParsorBasics.h"

/*********************  CLASS  **********************/
class CMRProjectVariable : public CMREntity
{
	public:
		CMRProjectVariable ( const std::string& latexName, const std::string& longName, const std::string& type );
		void addDim( int size, const std::string& name, int start = 0);
		void printCPPCode(void) const;
		std::string getTypeWithDims(void) const;
	private:
		int ghostDepths;
		std::string type;
		std::string memoryModel;
		CMRConstantDimensionsVector dims;
		CMRConstantDimensionsVector dimStart;
		CMRStringVector dimNames;
};

#endif //CMR_PROJECT_VARIABLE_H
