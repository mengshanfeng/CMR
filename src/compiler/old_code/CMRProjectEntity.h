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
#include "parsor/LatexEntityOld.h"

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
typedef std::vector<std::string> CMRConstantValueVector;
typedef std::map<std::string,CMRCompiler::LatexFormulasOld *> CMRProjectCaptureMap;

/*********************  CLASS  **********************/
class CMRProjectEntity
{
	public:
		CMRProjectEntity(const std::string & latexName,const std::string & longName);
		virtual ~CMRProjectEntity(void);
		virtual void addIndice(const std::string & name,CMRCaptureType capture = CMR_CAPTURE_NONE);
		virtual void setCaptureExponent(bool status = true);
		virtual void printDebug(void) const;
		virtual bool match(CMRCompiler::LatexEntityOld & entity,CMRProjectCaptureMap & capture) const;
		virtual std::ostream & genUsageCCode(std::ostream& out, const CMRProjectContext& context, CMRCompiler::LatexEntityOld& entity) const { out << longName; return out;};
		virtual void madeCaptureIndice(const std::string name,enum CMRCaptureType capture);
	protected:
		void applyLatexName(const std::string & latexName);
		bool haveCaptureFor(const std::string name) const;
	public:
		std::string latexName;
		std::string shortName;
		std::string longName;
		std::string exponent;
		CMREntityIndiceVector indices;
		CMREntityIndiceCaptureVector indicesCapture;
		int requiredIndices;
		CMRCompiler::LatexEntityOld latexEntity;
		bool captureExponent;
};

#endif //CMR_PROJECT_ENTITY_H
