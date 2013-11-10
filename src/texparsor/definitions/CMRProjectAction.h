/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_PROJECT_ACTION_H
#define CMR_PROJECT_ACTION_H

/********************  HEADERS  *********************/
#include "CMRProjectCode.h"
#include "CMRProjectIterator.h"

/*********************  TYPES  **********************/
class CMRTransformation;
class CMRLangDef;

/*********************  CLASS  **********************/
class CMRProjectAction
{
	public:
		CMRProjectAction ( const std::string & name, const std::string & descr,CMRProjectContext * parentContext = NULL);
		virtual void printDebug(std::ostream & out) const;
		virtual void genDefinitionCCode ( std::ostream& out, const CMRLangDef & lang, const CMRProjectContext& context, int padding = 0 ) const;
		virtual void genUsageCCode ( std::ostream& out, const CMRProjectContext& context, const CMRLatexEntity2& entity, bool write = false ) const;
		void runTransformation(CMRTransformation & transf);
		CMRProjectCodeEquation & addEquation(const std::string& eq);
		CMRProjectCodeEquation & addEquation(const std::string& latexName, const std::string& compute,const std::string & op = "=");
		CMRProjectCodeIteratorLoop& addIteratorLoop( const std::string& iterator);
		CMRProjectLocalVariable & addLocalVariable(const std::string & latexName, const std::string & longName,const std::string &type, const std::string & defaultValue);
		CMRProjectIterator & addIterator(const std::string & latexName, const std::string & longName, int start, int end);
		void changeCaptureType(const std::string & name, enum CMRCaptureType captureType) { assert(false);};
		CMRProjectContext & getContext(void);
		CMRProjectCodeEntry * insert(CMRProjectCodeEntry * entry,CMRProjectCodeTreeInsert location = CMR_INSERT_LAST_CHILD);
	private:
		CMRProjectCodeRootNode ops;
		std::string name;
		std::string descr;
};

#endif //CMR_PROJECT_ACTION_H
