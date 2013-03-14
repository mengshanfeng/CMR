/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_PROJECT_ENTITY_H
#define CMR_PROJECT_ENTITY_H

/********************  HEADERS  *********************/
#include <string>
#include <map>
#include <vector>
#include "CMRLatexEntity.h"

/********************  ENUM  ************************/
enum CMRCaptureType
{
	CMR_CAPTURE_NONE,
	CMR_CAPTURE_REQUIRED,
	CMR_CAPTURE_OPTIONS
};

/*********************  TYPES  **********************/
typedef std::vector<std::string> CMREntityIndiceVector;
typedef std::vector<CMRCaptureType> CMREntityIndiceCaptureVector;
typedef std::vector<int> CMRConstantDimensionsVector;
typedef std::vector<double> CMRConstantValueVector;
typedef std::map<std::string,std::string> CMRIndiceCaptureMap;

/*********************  CLASS  **********************/
class CMREntity
{
	public:
		CMREntity(const std::string & latexName,const std::string & longName);
		virtual ~CMREntity(void);
		void addIndice(const std::string & name,CMRCaptureType capture = CMR_CAPTURE_NONE);
		void printDebug(void) const;
		bool match(CMRLatexEntity & entity,CMRIndiceCaptureMap & capture);
	protected:
		void applyLatexName(const std::string & latexName);
	public:
		std::string latexName;
		std::string shortName;
		std::string longName;
		std::string exponent;
		CMREntityIndiceVector indices;
		CMREntityIndiceCaptureVector indicesCapture;
		int requiredIndices;
};

#endif //CMR_PROJECT_ENTITY_H
