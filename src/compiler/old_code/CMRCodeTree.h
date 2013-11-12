/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_CODE_TREE_H
#define CMR_CODE_TREE_H

/********************  HEADERS  *********************/
#include <vector>
#include <list>
#include "CMRProjectContext.h"
#include "CMRProjectEquation.h"

/********************  NAMESPACE  *******************/
namespace CMRCompilerOld
{

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
class CMRCodeTree
{
	public:
		class Iterator
		{
			public:
				Iterator(CMRCodeTree * current);
				Iterator & operator++(void);
				Iterator & operator--(void);
				Iterator & moveNext(void);
				Iterator & movePrev(void);
				Iterator & moveUp(void);
				Iterator & moveDown(void);
				bool hasNext(void) const;
				bool hasPrev(void) const;
				bool hasParent(void) const;
				bool hasChild(void) const;
				bool isEnd(void) const;
				bool operator == (const Iterator & it) const;
				bool operator != (const Iterator & it) const;
			protected:
				CMRCodeTree * current;
		};
		
		class ConstIterator
		{
			public:
				ConstIterator(const CMRCodeTree * current);
				ConstIterator & operator++(void);
				ConstIterator & operator--(void);
				ConstIterator & moveNext(void);
				ConstIterator & movePrev(void);
				ConstIterator & moveUp(void);
				ConstIterator & moveDown(void);
				bool hasNext(void) const;
				bool hasPrev(void) const;
				bool hasParent(void) const;
				bool hasChild(void) const;
				bool isEnd(void) const;
				bool operator == (const ConstIterator & it) const;
				bool operator != (const ConstIterator & it) const;
			protected:
				const CMRCodeTree * current;
		};
	public:
		CMRCodeTree(void);
		virtual ~CMRCodeTree(void);
		void insert(CMRCodeTree * action,CMRProjectCodeTreeInsert location = CMR_INSERT_AFTER);
		void remove(void);
		void clear(void);
		Iterator getFirstChild(void);
		Iterator getLastChild(void);
		Iterator getCurrent(void);
		Iterator getParent(void);
		Iterator getParentFirst(void);
		Iterator getEnd(void);
		ConstIterator getFirstChild(void) const;
		ConstIterator getLastChild(void) const;
		ConstIterator getCurrent(void) const;
		ConstIterator getParent(void) const;
		ConstIterator getParentFirst(void) const;
		ConstIterator getEnd(void) const;
		bool hasChild(void) const;
		int getDepth(void) const;
	private:
		CMRCodeTree(const CMRCodeTree & orig);
		CMRCodeTree & operator=(const CMRCodeTree & orig);
	private:
		CMRCodeTree * next;
		CMRCodeTree * prev;
		CMRCodeTree * parent;
		CMRCodeTree * firstChild;
		CMRCodeTree * lastChild;
};

/*********************  CLASS  *********************/
template <class T>
class CMRCodeTreeAbstract : public CMRCodeTree
{
	public:
		class Iterator : public CMRCodeTree::Iterator
		{
			public:
				Iterator(CMRCodeTreeAbstract<T> * current) :CMRCodeTree(current){};
				CMRCodeTreeAbstract<T> & operator*(void) {return ((CMRCodeTreeAbstract<T>)current)->value;};
				CMRCodeTreeAbstract<T> & operator->(void) {return ((CMRCodeTreeAbstract<T>)current)->value;};
		};
	public:
		CMRCodeTreeAbstract(const T & value) {this->value = value;};
	private:
		T value;
};

}

#endif //CMR_CODE_TREE_H
