/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_PROJECT_CODE_TREE_IMPL_H
#define CMR_PROJECT_CODE_TREE_IMPL_H

/********************  HEADERS  *********************/
#include "CMRProjectCodeTree.h"

/*******************  FUNCTION  *********************/
template <class T>
CMRProjectCodeTree<T>::CMRProjectCodeTree ( void )
{
	//init vars
	this->next = NULL;
	this->prev = NULL;
	this->parent = NULL;
	this->firstChild = NULL;
	this->lastChild = NULL;
}

/*******************  FUNCTION  *********************/
template <class T>
CMRProjectCodeTree<T>::~CMRProjectCodeTree ( void )
{
	clearChilds();
}

/*******************  FUNCTION  *********************/
template <class T>
void CMRProjectCodeTree<T>::clearChilds ( void )
{
	//vars
	T * cur = firstChild;
	T * next;
	
	//loop en delete all chidls
	while (cur != NULL)
	{
		next = cur->next;
		delete cur;
		cur = next;
	}
	
	//reset vars
	this->firstChild = NULL;
	this->lastChild = NULL;
}

/*******************  FUNCTION  *********************/
template <class T>
void CMRProjectCodeTree<T>::detach ( void )
{
	//update next
	if (next != NULL)
		next->prev = prev;
	else if (parent != NULL)
		parent->lastChild = prev;
	
	//update prev
	if (prev != NULL)
		prev->next = next;
	else
		parent->firstChild = next;
	
	//update parent
	this->parent = NULL;
	this->next = NULL;
	this->prev = NULL;
	
	//update
	onParentChange(parent);
}

/*******************  FUNCTION  *********************/
template <class T>
int CMRProjectCodeTree<T>::getDepth ( void ) const
{
	//vars
	int tmp = 0;
	const T * current = (T*)this;
	
	//loop in tree
	while (current->parent != NULL)
	{
		tmp++;
		current = current->parent;
	}
	
	//finish
	return tmp;
}

/*******************  FUNCTION  *********************/
template <class T>
bool CMRProjectCodeTree<T>::hasChild ( void ) const
{
	assert(firstChild == NULL || lastChild != NULL);
	return (firstChild != NULL);
}

/*******************  FUNCTION  *********************/
template <class T>
void CMRProjectCodeTree<T>::onParentChange ( T* newParent )
{

}

/*******************  FUNCTION  *********************/
template <class T>
T * CMRProjectCodeTree<T>::insert ( T* entry, CMRProjectCodeTreeInsert location )
{
	//vars
	T * localParent;
	T * entryParent;
	
	//trivial
	if (entry == NULL)
		return NULL;
	
	//errors
	assert(dynamic_cast<T*>(this) != NULL);
	
	//errors if already has parent
	if (entry->parent != NULL)
		throw CMRLatexException("Invalid insertion of entry which already has parent !");
	
	//setup old parents
	localParent = this->parent;
	entryParent = entry->parent;

	//do it
	switch(location)
	{
		case CMR_INSERT_AFTER:
			entry->prev = (T*)this;
			entry->next = next;
			if (next != NULL)
				next->prev = entry;
			else if (parent != NULL)
				parent->lastChild = entry;
			next = entry;
			entry->parent = parent;
			break;
		case CMR_INSERT_BEFORE:
			entry->next = (T*)this;
			entry->prev = prev;
			if (prev != NULL)
				prev->next = entry;
			else if (parent != NULL)
				parent->firstChild = entry;
			prev = entry;
			entry->parent = parent;
			break;
		case CMR_INSERT_FIRST_CHILD:
			if (firstChild == NULL)
				lastChild = firstChild = entry;
			else
				firstChild->insert(entry,CMR_INSERT_BEFORE);
			entry->parent = (T*)this;
			break;
		case CMR_INSERT_LAST_CHILD:
			if (lastChild == NULL)
				lastChild = firstChild = entry;
			else
				lastChild->insert(entry,CMR_INSERT_AFTER);
			entry->parent = (T*)this;
			break;
		case  CMR_INSERT_FIRST:
			assert(parent != NULL);
			parent->insert(entry,CMR_INSERT_FIRST_CHILD);
			break;
		case  CMR_INSERT_LAST:
			assert(parent != NULL);
			parent->insert(entry,CMR_INSERT_LAST_CHILD);
			break;
		case CMR_INSERT_INPLACE:
			insert(entry, CMR_INSERT_AFTER);
			detach();
			break;
		default:
			assert(false);
			abort();
	}
	
	//callback
	if (localParent != parent)
		onParentChange(parent);
	if (entryParent != entry->parent)
		((CMRProjectCodeTree<T>*)entry)->onParentChange(entry->parent);
	
	//finish
	return entry;
}

#endif //CMR_PROJECT_CODE_TREE_IMPL_H
