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
#include <iostream>
#include <sstream>
#include "CMRParsorBasics.h"
#include "CMRLatexParsorContext.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
CMRLatexParsorContext::CMRLatexParsorContext ( const string& value )
{
	//init
	this->value = value;
	this->position = 0;
	this->parent = NULL;
	this->relToParent = 0;
}

/*******************  FUNCTION  *********************/
CMRLatexParsorContext::CMRLatexParsorContext ( const CMRLatexParsorContext* parent, int start, int end )
{
	//errors
	assert(parent != NULL);
	assert(start >= 0);
	assert(start <= end);
	assert(end <= parent->value.size());
	
	//init
	this->parent = parent;
	this->value = parent->value.substr(start,end-start);
	this->position = 0;
	this->relToParent = start;
}

/*******************  FUNCTION  *********************/
char CMRLatexParsorContext::getCurrent ( void ) const
{
	//assert(this->position < value.size());
	if (this->position < value.size())
		return this->value[this->position];
	else
		return '\0';
}

/*******************  FUNCTION  *********************/
int CMRLatexParsorContext::getPosition ( void ) const
{
	return this->position;
}

/*******************  FUNCTION  *********************/
bool CMRLatexParsorContext::isEnd ( void ) const
{
	return this->position >= value.size();
}

/*******************  FUNCTION  *********************/
int CMRLatexParsorContext::move ( int delta )
{
	this->position += delta;
	assert(this->position >= 0);
	return this->position;
}

/*******************  FUNCTION  *********************/
CMRLatexParsorContext& CMRLatexParsorContext::operator++ ( void )
{
	this->move(1);
	return *this;
}

/*******************  FUNCTION  *********************/
void CMRLatexParsorContext::skipWhiteSpace ( void )
{
	while (cmrIsWhiteSpace(this->value[this->position]))
		this->position++;
}

/*******************  FUNCTION  *********************/
bool CMRLatexParsorContext::startBy ( const string& value ) const
{
	if (value.size() + this->position > this->value.size())
		return false;
	else
		return this->value.substr(0,value.size()) == value;
}

/*******************  FUNCTION  *********************/
bool CMRLatexParsorContext::startBy ( char value ) const
{
	if (this->position < this->value.size())
		return this->value[this->position] == value;
	else
		return false;
}

/*******************  FUNCTION  *********************/
CMRLatexParsorContext CMRLatexParsorContext::extractSubZone ( char delimOpen, char delimClose )
{
	//vars
	int start = position + 1;
	int cur = position;
	int cnt = 0;

	//errors
	assert(isEnd() == false);
	assert(value[position] == delimOpen);
	
	//search close
	do {
		//check delim
		if (value[cur] == delimOpen)
			cnt++;
		else if (value[cur] == delimClose)
			cnt--;
		
		//move
		if (cnt > 0)
			cur++;
		
		//error
		if (cur > value.size())
			this->fatal("Missing close.");
	} while (cnt > 0);
	
	//return
	this->position = cur+1;
	return CMRLatexParsorContext(this,start,cur);
}

/*******************  FUNCTION  *********************/
void CMRLatexParsorContext::fatal ( const string& message ) const throw(CMRLatexException)
{
	//vars
	const CMRLatexParsorContext * cur = this;
	int curPos = position;
	stringstream err;
	
	//errors
	err << "LATEX PARSING ERROR : " << message << endl;
	
	//display position
	while (cur != NULL)
	{
		//print local info
		err << cur->value << endl;
		for (int i = 0 ; i < curPos ; i++)
			err << ' ';
		err << '^' << endl;
		
		//move to parent
		curPos += cur->relToParent;
		cur = cur->parent;
	}
	
	//finish by exit
	throw CMRLatexException(err.str());
}

/*******************  FUNCTION  *********************/
char CMRLatexParsorContext::getCurAndMove ( int delta )
{
	char tmp = '\0';
	
	assert(this->position >= 0);
	
	if (position < value.size())
		tmp = value[position++];

	return tmp;
}

/*******************  FUNCTION  *********************/
const string& CMRLatexParsorContext::getValue ( void ) const
{
	return this->value;
}
