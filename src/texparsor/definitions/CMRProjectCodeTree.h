/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_PROJECT_CODE_TREE_H
#define CMR_PROJECT_CODE_TREE_H

/********************  HEADERS  *********************/
#include <cstdlib>
#include <cassert>
#include "CMRProjectContext.h"

/*********************  TYPES  **********************/
enum CMRProjectCodeTreeInsert
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
class CMRProjectCodeTree
{
	public:
		CMRProjectCodeTree(void);
		virtual ~CMRProjectCodeTree(void);
		T * insert(T * entry,CMRProjectCodeTreeInsert location = CMR_INSERT_LAST_CHILD);
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
		CMRProjectCodeTree(const CMRProjectCodeTree<T> & orig);
		CMRProjectCodeTree & operator=(const CMRProjectCodeTree<T> & orig);
	private:
		T * next;
		T * prev;
		T * parent;
		T * firstChild;
		T * lastChild;
};

/*******************  FUNCTION  *********************/
#include "CMRProjectCodeTree_impl.h"

#endif //CMR_PROJECT_CODE_TREE_H
