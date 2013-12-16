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
#include <map>
#include <string>
#include <vector>
#include "parsor/LatexEntity.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/********************  ENUM  ************************/
enum CaptureType
{
	CAPTURE_NONE,
	CAPTURE_REQUIRED,
	CAPTURE_OPTIONS
};

/*********************  STRUCT  *********************/
struct CaptureDef
{
	CaptureDef(const std::string & name,CaptureType captureType);
	std::string name;
	CaptureType captureType;
};

/*********************  TYPES  **********************/
class ProjectContext;
typedef std::vector<int> ConstantDimensionsVector;
typedef std::vector<std::string> ConstantValueVector;
typedef std::vector<std::string> StringVector;
typedef std::map<std::string,const LatexFormulas *> ProjectCaptureMap;
typedef std::vector<CaptureDef> ProjectCaptureDefMap;

/*********************  CLASS  **********************/
class IProjectEntity
{
	public:
		IProjectEntity(void) {};
		virtual ~IProjectEntity(void){};
		
		//to provide in first level impl
		virtual std::string getLatexName(void) const = 0;
		virtual const std::string & getLongName(void) const = 0;
		virtual bool match(const LatexEntity & entity) const = 0;
		
		//to overload
		virtual void genDefinitionCCode(std::ostream& out, const ProjectContext& context, int padding = 0) const = 0;
		virtual void genUsageCCode(std::ostream& out, const ProjectContext& context, const LatexEntity& entity, bool write = false) const = 0;
		virtual void printDebug(std::ostream & out) const =  0;
};

/*********************  CLASS  **********************/
class ProjectEntityOld : public IProjectEntity
{
	public:
		ProjectEntityOld(const std::string & latexName,const std::string & longName);
		virtual ~ProjectEntityOld(void);
		void addIndice(const std::string & name,CaptureType captureType = CAPTURE_NONE);
		void addExponent(const std::string & name,CaptureType captureType = CAPTURE_NONE);
		void addParameter(const std::string & name,CaptureType captureType = CAPTURE_NONE);
		void changeCaptureType(const std::string & name,CaptureType captureType);
		bool match(const LatexEntity & entity) const;
		bool match(const std::string & value) const;
		void capture( const LatexEntity& entity, ProjectCaptureMap& capture ) const;
		std::string getLatexName(void) const;
		const std::string & getShortName(void) const;
		const std::string & getLongName(void) const;
		bool haveCapture( const std::string& name );
		StringVector getCapturedIndices(void) const;
		const ProjectCaptureDefMap & getIndices(void) const;
		const ProjectCaptureDefMap & getExponents(void) const;
		const ProjectCaptureDefMap & getParameters(void) const;
		void markAllCaptureAs(CaptureType capture);
		void captureName( void );
		bool isWildcardName(void);
		
		//to overload
		virtual void genDefinitionCCode(std::ostream& out, const ProjectContext& context, int padding = 0) const = 0;
		virtual void genUsageCCode(std::ostream& out, const ProjectContext& context, const LatexEntity& entity, bool write = false) const = 0;
		virtual void printDebug(std::ostream & out) const;
	public:
		friend std::ostream & operator << (std::ostream & out,const ProjectEntityOld & value);
	protected:
		virtual void onUpdateCaptureType(const std::string & name,CaptureType captureType);
		bool internalMatch(const LatexEntity & entity,ProjectCaptureMap * capture) const;
		bool internalMatch( const LatexFormulasVector& formulaList, const ProjectCaptureDefMap& captureDef, ProjectCaptureMap* captureOut ) const;
		void applyLatexName(const std::string & latexName);
		void fillCapture(ProjectCaptureDefMap & capture,LatexFormulasVector & formulaList);
		void addCapture(ProjectCaptureDefMap & capture,const std::string & value,CaptureType captureType);
		void addCapture( ProjectCaptureDefMap& capture, const LatexFormulas& formula, CaptureType captureType );
		bool changeCaptureType(ProjectCaptureDefMap & capture, const std::string & name, CaptureType captureType);
		static std::string formatCaptureList ( const ProjectCaptureDefMap& value, const std::string& sep, const std::string& open, const std::string& close, bool forceOpenClose);
		CaptureDef * findCaptureDef( ProjectCaptureDefMap& value, const std::string& name, bool beCaptured = false );
		CaptureDef * findCaptureDef( const std::string& name, bool beCaptured = false );
		void ensureUniqCapture( const LatexFormulas& f );
		static void printDebug( std::ostream& out, const std::string& name, const ProjectCaptureDefMap& map );
		static void markAllCaptureAs(ProjectCaptureDefMap & map,CaptureType capture);
	private:
		std::string shortName;
		std::string longName;
		ProjectCaptureDefMap indices;
		ProjectCaptureDefMap exponents;
		ProjectCaptureDefMap parameters;
		bool capName;
};

}

#endif //CMR_PROJECT_ENTIT_H
