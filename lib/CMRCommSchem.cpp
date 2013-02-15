/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cstdlib>
#include <cassert>
#include "CMRCommSchem.h"
#include "CMRComm.h"

/*******************  FUNCTION  *********************/
CMRCommSchem::CMRCommSchem ( void )
{

}

/*******************  FUNCTION  *********************/
CMRCommSchem::~CMRCommSchem ( void )
{
	this->clear();
}

/*******************  FUNCTION  *********************/
void CMRCommSchem::addComm ( CMRComm * comm )
{
	//errors
	assert(comm != NULL);

	//insert
	comms.push_back(comm);
}

/*******************  FUNCTION  *********************/
void CMRCommSchem::clear ( void )
{
	for (CMRCommList::iterator it = comms.begin() ; it != comms.end() ; ++it)
	{
		assert(*it != NULL);
		delete *it;
	}
	comms.clear();
}

/*******************  FUNCTION  *********************/
CMRComm* CMRCommSchem::getComm ( int id )
{
	if ((size_t)id >= comms.size())
		return NULL;
	else
		return comms[id];
}

/*******************  FUNCTION  *********************/
int CMRCommSchem::count ( void ) const
{
	return comms.size();
}

/*******************  FUNCTION  *********************/
void CMRCommSchem::run ( void )
{
	for (CMRCommList::iterator it = comms.begin() ; it != comms.end() ; ++it)
	{
		(*it)->run();
	}
}
