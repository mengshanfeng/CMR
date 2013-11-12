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

namespace CMRCompilerOld
{

/*********************  CLASS  *********************/
class ProjectActionOld : public CMRCodeTree
{
	public:
		class Iterator : public CMRCodeTree::Iterator
		{
			public:
				Iterator(ProjectActionOld * current) :CMRCodeTree::Iterator(current){};
				Iterator(CMRCodeTree::Iterator it) :CMRCodeTree::Iterator(it) {};
				ProjectActionOld & operator*(void) {return *((ProjectActionOld*)current);};
				ProjectActionOld * operator->(void) {return ((ProjectActionOld*)current);};
				Iterator & operator = (CMRCodeTree::Iterator it) {*(CMRCodeTree::Iterator*)this = it; return *this;};
		};
		
		class ConstIterator : public CMRCodeTree::ConstIterator
		{
			public:
				ConstIterator(const ProjectActionOld * current) :CMRCodeTree::ConstIterator(current){};
				ConstIterator(CMRCodeTree::ConstIterator it) :CMRCodeTree::ConstIterator(it) {};
				const ProjectActionOld & operator*(void) {return *((const ProjectActionOld*)current);};
				const ProjectActionOld * operator->(void) {return ((const ProjectActionOld*)current);};
				ConstIterator & operator = (CMRCodeTree::ConstIterator it) {*(CMRCodeTree::ConstIterator*)this = it; return *this;};
		};
	public:
		ProjectActionOld( std::string name, std::string descr, CMRProjectContext* parentContext = NULL );
		ProjectActionOld & addSubBlock(std::string loopDescr,std::string parameter,CMRProjectCodeTreeInsert location = CMR_INSERT_LAST_CHILD);
		CMRProjectEquation& addEquation( const std::string& latexName, const std::string& longName, const std::string& compute,CMRProjectCodeTreeInsert location = CMR_INSERT_LAST_CHILD);
		ProjectActionOld & addIteratorLoop( const std::string& iterator, CMRProjectCodeTreeInsert location = CMR_INSERT_LAST_CHILD);
		void genCCode(std::ostream& out, const CMRProjectContext & context ,int depth) const;
		void printDebug(int depth) const;
		void insertAction(ProjectActionOld * action,CMRProjectCodeTreeInsert location);
		const std::string & getName(void) const {return name;};
		const std::string & getDescription(void) const {return description;};
		CMRProjectEquation & getEquation(void) {return *eq;};
		bool hasEquation(void) const {return eq != NULL;};
		void addContextEntry(CMRProjectEntity * entity);
		void addContextEntry(CMRProjectEntity * entity,CMRProjectCodeTreeInsert location);
		const CMRProjectContext & getContext(void) const;
	protected:
		void genEqCCode(std::ostream& out, const CMRProjectContext & context ,int depth) const;
		void genItLoopCCode(std::ostream& out, const CMRProjectContext & context ,int depth) const;
		void genRootElemCCode(std::ostream& out, const CMRProjectContext & context ,int depth) const;
		void checkContext(CMRProjectContext & context) const;
	private:
		CMRProjectEquation * eq;
		std::string name;
		std::string description;
		CMRProjectContext context;
};

}

#endif //CMR_PROJECT_ACTION_H
