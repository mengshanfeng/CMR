/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cassert>
#include <cstdlib>
#include "CMRCodeTree.h"

/*******************  FUNCTION  *********************/
CMRCodeTree::CMRCodeTree(void )
{
	prev = NULL;
	next = NULL;
	firstChild = NULL;
	lastChild = NULL;
	parent = NULL;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::~CMRCodeTree(void )
{
	clear();
}

/*******************  FUNCTION  *********************/
CMRCodeTree::Iterator CMRCodeTree::getCurrent(void )
{
	Iterator it(this);
	return it;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::Iterator CMRCodeTree::getFirstChild(void )
{
	Iterator it(firstChild);
	return it;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::Iterator CMRCodeTree::getLastChild(void )
{
	Iterator it(lastChild);
	return it;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::Iterator CMRCodeTree::getEnd(void )
{
	Iterator it(NULL);
	return it;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::Iterator CMRCodeTree::getParent(void )
{
	Iterator it(parent);
	return it;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::Iterator CMRCodeTree::getParentFirst(void )
{
	assert(parent != NULL);
	Iterator it(parent->firstChild);
	return it;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::ConstIterator CMRCodeTree::getCurrent(void ) const
{
	ConstIterator it(this);
	return it;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::ConstIterator CMRCodeTree::getFirstChild(void ) const
{
	ConstIterator it(firstChild);
	return it;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::ConstIterator CMRCodeTree::getLastChild(void ) const
{
	ConstIterator it(lastChild);
	return it;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::ConstIterator CMRCodeTree::getEnd(void ) const
{
	ConstIterator it(NULL);
	return it;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::ConstIterator CMRCodeTree::getParent(void ) const
{
	ConstIterator it(parent);
	return it;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::ConstIterator CMRCodeTree::getParentFirst(void ) const
{
	assert(parent != NULL);
	ConstIterator it(parent->firstChild);
	return it;
}

/*******************  FUNCTION  *********************/
void CMRCodeTree::clear(void )
{
	CMRCodeTree * cur = firstChild;
	CMRCodeTree * next;
	
	while (cur != NULL)
	{
		next = cur->next;
		delete cur;
		cur = next;
	}
}

/*******************  FUNCTION  *********************/
void CMRCodeTree::remove(void )
{
	if (next != NULL)
		next->prev = prev;
	else if (parent != NULL)
		parent->lastChild = prev;
	
	if (prev != NULL)
		prev->next = next;
	else
		parent->firstChild = next;
}

/*******************  FUNCTION  *********************/
void CMRCodeTree::insert(CMRCodeTree* action, CMRProjectCodeTreeInsert location)
{
	//trivial
	if (action == NULL)
		return;

	//do it
	switch(location)
	{
		case CMR_INSERT_AFTER:
			action->prev = this;
			action->next = next;
			if (next != NULL)
				next->prev = action;
			else
				parent->lastChild = action;
			next = action;
			action->parent = parent;
			break;
		case CMR_INSERT_BEFORE:
			action->next = this;
			action->prev = prev;
			if (prev != NULL)
				prev->next = action;
			else
				parent->firstChild = action;
			prev = action;
			action->parent = parent;
			break;
		case CMR_INSERT_FIRST_CHILD:
			if (firstChild == NULL)
				lastChild = firstChild = action;
			else
				firstChild->insert(action,CMR_INSERT_BEFORE);
			action->parent = this;
			break;
		case CMR_INSERT_LAST_CHILD:
			if (lastChild == NULL)
				lastChild = firstChild = action;
			else
				lastChild->insert(action,CMR_INSERT_AFTER);
			action->parent = this;
			break;
		case  CMR_INSERT_FIRST:
			assert(parent != NULL);
			parent->insert(action,CMR_INSERT_FIRST_CHILD);
			break;
		case  CMR_INSERT_LAST:
			assert(parent != NULL);
			parent->insert(action,CMR_INSERT_LAST_CHILD);
			break;
		default:
			assert(false);
			abort();
	}
}

/*******************  FUNCTION  *********************/
bool CMRCodeTree::Iterator::hasChild(void ) const
{
	if (current != NULL)
		return current->firstChild != NULL;
	else
		return false;
}

/*******************  FUNCTION  *********************/
bool CMRCodeTree::Iterator::hasNext(void ) const
{
	if (current != NULL)
		return current->next != NULL;
	else
		return false;
}

/*******************  FUNCTION  *********************/
bool CMRCodeTree::Iterator::hasParent(void ) const
{
	if (current != NULL)
		return current->parent != NULL;
	else
		return false;
}

/*******************  FUNCTION  *********************/
bool CMRCodeTree::Iterator::hasPrev(void ) const
{
	if (current != NULL)
		return current->prev != NULL;
	else
		return false;
}

/*******************  FUNCTION  *********************/
bool CMRCodeTree::Iterator::isEnd(void ) const
{
	return current == NULL;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::Iterator::Iterator(CMRCodeTree* current)
{
	this->current = current;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::Iterator& CMRCodeTree::Iterator::moveDown(void )
{
	if (current != NULL)
		current = current->firstChild;
	return *this;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::Iterator& CMRCodeTree::Iterator::moveNext(void )
{
	if (current != NULL)
		current = current->next;
	return *this;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::Iterator& CMRCodeTree::Iterator::movePrev(void )
{
	if (current != NULL)
		current = current->prev;
	return *this;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::Iterator& CMRCodeTree::Iterator::moveUp(void )
{
	if (current != NULL)
		current = current->parent;
	return *this;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::Iterator& CMRCodeTree::Iterator::operator++(void )
{
	return moveNext();
}

/*******************  FUNCTION  *********************/
CMRCodeTree::Iterator& CMRCodeTree::Iterator::operator--(void )
{
	return movePrev();
}

/*******************  FUNCTION  *********************/
bool CMRCodeTree::Iterator::operator==(const CMRCodeTree::Iterator& it) const
{
	return current == it.current;
}

/*******************  FUNCTION  *********************/
bool CMRCodeTree::Iterator::operator!=(const CMRCodeTree::Iterator& it) const
{
	return current != it.current;
}

/*******************  FUNCTION  *********************/
bool CMRCodeTree::ConstIterator::hasChild(void ) const
{
	if (current != NULL)
		return current->firstChild != NULL;
	else
		return false;
}

/*******************  FUNCTION  *********************/
bool CMRCodeTree::ConstIterator::hasNext(void ) const
{
	if (current != NULL)
		return current->next != NULL;
	else
		return false;
}

/*******************  FUNCTION  *********************/
bool CMRCodeTree::ConstIterator::hasParent(void ) const
{
	if (current != NULL)
		return current->parent != NULL;
	else
		return false;
}

/*******************  FUNCTION  *********************/
bool CMRCodeTree::ConstIterator::hasPrev(void ) const
{
	if (current != NULL)
		return current->prev != NULL;
	else
		return false;
}

/*******************  FUNCTION  *********************/
bool CMRCodeTree::ConstIterator::isEnd(void ) const
{
	return current == NULL;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::ConstIterator::ConstIterator(const CMRCodeTree* current)
{
	this->current = current;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::ConstIterator& CMRCodeTree::ConstIterator::moveDown(void )
{
	if (current != NULL)
		current = current->firstChild;
	return *this;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::ConstIterator& CMRCodeTree::ConstIterator::moveNext(void )
{
	if (current != NULL)
		current = current->next;
	return *this;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::ConstIterator& CMRCodeTree::ConstIterator::movePrev(void )
{
	if (current != NULL)
		current = current->prev;
	return *this;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::ConstIterator& CMRCodeTree::ConstIterator::moveUp(void )
{
	if (current != NULL)
		current = current->parent;
	return *this;
}

/*******************  FUNCTION  *********************/
CMRCodeTree::ConstIterator& CMRCodeTree::ConstIterator::operator++(void )
{
	return moveNext();
}

/*******************  FUNCTION  *********************/
CMRCodeTree::ConstIterator& CMRCodeTree::ConstIterator::operator--(void )
{
	return movePrev();
}

/*******************  FUNCTION  *********************/
bool CMRCodeTree::ConstIterator::operator==(const CMRCodeTree::ConstIterator& it) const
{
	return current == it.current;
}

/*******************  FUNCTION  *********************/
bool CMRCodeTree::ConstIterator::operator!=(const CMRCodeTree::ConstIterator& it) const
{
	return current != it.current;
}

/*******************  FUNCTION  *********************/
bool CMRCodeTree::hasChild ( void ) const
{
	return firstChild != NULL;
}

/*******************  FUNCTION  *********************/
int CMRCodeTree::getDepth ( void ) const
{
	int tmp = 0;
	const CMRCodeTree * current = this;
	while (current->parent != NULL)
	{
		tmp++;
		current = current->parent;
	}
	return tmp;
}
