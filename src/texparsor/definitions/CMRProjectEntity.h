/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_PROJECT_ENTIT_H
#define CMR_PROJECT_ENTIT_H

/********************  HEADERS  *********************/
#include <string>
#include <map>
#include <vector>
#include "../parsor/CMRLatexEntity2.h"

/********************  ENUM  ************************/
enum CMRCaptureType
{
	CMR_CAPTURE_NONE,
	CMR_CAPTURE_REQUIRED,
	CMR_CAPTURE_OPTIONS
};

/*********************  STRUCT  *********************/
struct CMRCaptureDef
{
	CMRCaptureDef(const std::string & name,CMRCaptureType captureType);
	std::string name;
	CMRCaptureType captureType;
};

/*********************  TYPES  **********************/
class CMRProjectContext;
typedef std::vector<int> CMRConstantDimensionsVector;
typedef std::vector<std::string> CMRConstantValueVector;
typedef std::vector<std::string> CMRStringVector;
typedef std::map<std::string,CMRLatexFormulas2 *> CMRProjectCaptureMap;
typedef std::vector<CMRCaptureDef> CMRProjectCaptureDefMap;

/*********************  CLASS  **********************/
class CMRProjectEntity
{
	public:
		CMRProjectEntity(const std::string & latexName,const std::string & longName);
		virtual ~CMRProjectEntity(void);
		void addIndice(const std::string & name,CMRCaptureType captureType = CMR_CAPTURE_NONE);
		void addExponent(const std::string & name,CMRCaptureType captureType = CMR_CAPTURE_NONE);
		void addParameter(const std::string & name,CMRCaptureType captureType = CMR_CAPTURE_NONE);
		void changeCaptureType(const std::string & name, enum CMRCaptureType captureType);
		bool match(CMRLatexEntity2 & entity) const;
		bool match(const std::string & value) const;
		void capture(CMRLatexEntity2 & entity,CMRProjectCaptureMap & capture) const;
		std::string getLatexName(void) const;
		const std::string & getShortName(void) const;
		const std::string & getLongName(void) const;
		bool haveCapture( const std::string& name );
		CMRStringVector getCapturedIndices(void) const;
		
		//to overload
		virtual void genDefinitionCCode(std::ostream& out, const CMRProjectContext& context) const = 0;
		virtual void genUsageCCode(std::ostream& out, const CMRProjectContext& context, CMRLatexEntity2& entity, bool write = false) const = 0;
		virtual void printDebug(std::ostream & out) const;
	public:
		friend std::ostream & operator << (std::ostream & out,const CMRProjectEntity & value);
	protected:
		virtual void onUpdateCaptureType(const std::string & name,enum CMRCaptureType captureType);
		bool internalMatch(CMRLatexEntity2 & entity,CMRProjectCaptureMap * capture) const;
		bool internalMatch( CMRLatexFormulasVector2& formulaList, const CMRProjectCaptureDefMap& captureDef, CMRProjectCaptureMap* captureOut ) const;
		void applyLatexName(const std::string & latexName);
		void fillCapture(CMRProjectCaptureDefMap & capture,CMRLatexFormulasVector2 & formulaList);
		void addCapture(CMRProjectCaptureDefMap & capture,const std::string & value,CMRCaptureType captureType);
		void addCapture( CMRProjectCaptureDefMap& capture, const CMRLatexFormulas2& formula, CMRCaptureType captureType );
		bool changeCaptureType(CMRProjectCaptureDefMap & capture, const std::string & name, enum CMRCaptureType captureType);
		static std::string formatCaptureList ( const CMRProjectCaptureDefMap& value, const std::string& sep, const std::string& open, const std::string& close, bool forceOpenClose);
		CMRCaptureDef * findCaptureDef( CMRProjectCaptureDefMap& value, const std::string& name, bool beCaptured = false );
		CMRCaptureDef * findCaptureDef( const std::string& name, bool beCaptured = false );
		void ensureUniqCapture( const CMRLatexFormulas2& f );
		static void printDebug( std::ostream& out, const std::string& name, const CMRProjectCaptureDefMap& map );
	private:
		std::string shortName;
		std::string longName;
		CMRProjectCaptureDefMap indices;
		CMRProjectCaptureDefMap exponents;
		CMRProjectCaptureDefMap parameters;
};

#endif //CMR_PROJECT_ENTIT_H
