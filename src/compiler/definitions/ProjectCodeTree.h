/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMPILER_PROJECT_CODE_TREE_H
#define CMR_COMPILER_PROJECT_CODE_TREE_H

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  TYPES  **********************/
enum ProjectCodeTreeInsert
{
	CMR_INSERT_FIRST_CHILD,
	CMR_INSERT_LAST_CHILD,
	CMR_INSERT_FIRST,
	CMR_INSERT_LAST,
	CMR_INSERT_AFTER,
	CMR_INSERT_BEFORE,
	CMR_INSERT_INPLACE
};

/*********************  CLASS  *********************/
template <class T>
class ProjectCodeTree
{
	public:
		ProjectCodeTree(void);
		virtual ~ProjectCodeTree(void);
		T * insert(T * entry,ProjectCodeTreeInsert location = CMR_INSERT_LAST_CHILD);
		void detach(void);
		void clearChilds(void);
		bool hasChild(void) const;
		int getDepth(void) const;
		T * getFirstChild(void) {return firstChild;};
		T * getLastChild(void) {return lastChild;};
		T * getPrev(void) {return prev;};
		T * getNext(void) {return next;};
		T * getParent(void) {return parent;};
		const T * getFirstChild(void) const {return firstChild;};
		const T * getLastChild(void) const {return lastChild;};
		const T * getPrev(void) const {return prev;};
		const T * getNext(void) const {return next;};
		const T * getParent(void) const {return parent;};
	protected:
		virtual void onParentChange(T * newParent);
	private:
		ProjectCodeTree(const ProjectCodeTree<T> & orig);
		ProjectCodeTree & operator=(const ProjectCodeTree<T> & orig);
	private:
		T * next;
		T * prev;
		T * parent;
		T * firstChild;
		T * lastChild;
};

}

/*******************  FUNCTION  *********************/
#include "ProjectCodeTree_impl.h"

#endif //CMR_COMPILER_PROJECT_CODE_TREE_H
