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
#include <vector>
#include <list>
#include "CMRProjectContext.h"
#include "CMRProjectEquation.h"
#include "CMRCodeTree.h"

/*********************  CLASS  *********************/
class CMRProjectAction : public CMRCodeTree
{
	public:
		class Iterator : public CMRCodeTree::Iterator
		{
			public:
				Iterator(CMRProjectAction * current) :CMRCodeTree::Iterator(current){};
				Iterator(CMRCodeTree::Iterator it) :CMRCodeTree::Iterator(it) {};
				CMRProjectAction & operator*(void) {return *((CMRProjectAction*)current);};
				CMRProjectAction * operator->(void) {return ((CMRProjectAction*)current);};
				Iterator & operator = (CMRCodeTree::Iterator it) {*(CMRCodeTree::Iterator*)this = it; return *this;};
		};
		
		class ConstIterator : public CMRCodeTree::ConstIterator
		{
			public:
				ConstIterator(const CMRProjectAction * current) :CMRCodeTree::ConstIterator(current){};
				ConstIterator(CMRCodeTree::ConstIterator it) :CMRCodeTree::ConstIterator(it) {};
				const CMRProjectAction & operator*(void) {return *((const CMRProjectAction*)current);};
				const CMRProjectAction * operator->(void) {return ((const CMRProjectAction*)current);};
				ConstIterator & operator = (CMRCodeTree::ConstIterator it) {*(CMRCodeTree::ConstIterator*)this = it; return *this;};
		};
	public:
		CMRProjectAction(std::string name,std::string descr = "");
		CMRProjectAction & addSubBlock(std::string loopDescr,std::string parameter,CMRProjectCodeTreeInsert location = CMR_INSERT_LAST_CHILD);
		CMRProjectEquation& addEquation( const std::string& latexName, const std::string& longName, const std::string& compute,CMRProjectCodeTreeInsert location = CMR_INSERT_LAST_CHILD);
		void replaceLoops(int * tmpId);
		void genCCode(std::ostream& out, CMRProjectContext & context ,int depth) const;
		void printDebug(int depth) const;
		void insertAction(CMRProjectAction * action,CMRProjectCodeTreeInsert location);
		const std::string & getName(void) const {return name;};
		const std::string & getDescription(void) const {return description;};
	protected:
		void genEqCCode(std::ostream& out, CMRProjectContext & context ,int depth) const;
		void genEqCCode(std::ostream& out, CMRProjectContext & context, CMRLatexEntity & entity) const;
	private:
		CMRProjectEquation * eq;
		std::string name;
		std::string description;
};

#endif //CMR_PROJECT_ACTION_H
