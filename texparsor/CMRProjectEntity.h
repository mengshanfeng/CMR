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
#include "parsor/CMRLatexEntity.h"

/********************  ENUM  ************************/
enum CMRCaptureType
{
	CMR_CAPTURE_NONE,
	CMR_CAPTURE_REQUIRED,
	CMR_CAPTURE_OPTIONS
};

/*********************  TYPES  **********************/
struct CMRProjectContext;
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
		void setCaptureExponent(bool status = true);
		virtual void printDebug(void) const;
		virtual bool match(CMRLatexEntity & entity,CMRIndiceCaptureMap & capture) const;
		virtual std::ostream & genUsageCCode(std::ostream& out, CMRProjectContext& context, CMRLatexEntity& entity) const { out << longName; return out;};
	protected:
		void applyLatexName(const std::string & latexName);
		bool haveCaptureFor(const std::string name) const;
		void madeCaptureIndice(const std::string name,enum CMRCaptureType capture);
	public:
		std::string latexName;
		std::string shortName;
		std::string longName;
		std::string exponent;
		CMREntityIndiceVector indices;
		CMREntityIndiceCaptureVector indicesCapture;
		int requiredIndices;
		CMRLatexEntity latexEntity;
		bool captureExponent;
};

#endif //CMR_PROJECT_ENTITY_H
