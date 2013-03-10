/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 02/2013
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_DOMAIN_MEMORY_H
#define CMR_DOMAIN_MEMORY_H

/********************  HEADERS  *********************/
#include "../common/CMRGeometry.h"

/*********************  CLASS  **********************/
class CMRDomainMemory
{
	public:
		inline CMRDomainMemory(void);
		inline CMRDomainMemory(void * baseAddr,const CMRRect & memoryRect);
		inline void set(void * baseAddr,const CMRRect & memoryRect);
		inline void set(CMRDomainMemory & domainMemory);
		inline const CMRVect2D getAbsPosition( int dx = 0, int dy = 0 ) const;
	protected:
		void * ptr;
		CMRRect memoryRect;
};

/*******************  FUNCTION  *********************/
inline CMRDomainMemory::CMRDomainMemory ( void )
{
	this->ptr = NULL;
}

/*******************  FUNCTION  *********************/
inline CMRDomainMemory::CMRDomainMemory ( void* baseAddr, const CMRRect& memoryRect)
{
	this->set(baseAddr,memoryRect);
}

/*******************  FUNCTION  *********************/
inline void CMRDomainMemory::set ( void* baseAddr, const CMRRect& memoryRect )
{
	this->ptr = baseAddr;
	this->memoryRect = memoryRect;
}

/*******************  FUNCTION  *********************/
inline const CMRVect2D CMRDomainMemory::getAbsPosition ( int dx,int dy ) const
{
	return CMRVect2D(memoryRect.x+dx,memoryRect.y+dy);
}

/*******************  FUNCTION  *********************/
inline void CMRDomainMemory::set ( CMRDomainMemory& domainMemory )
{
	*this = domainMemory;
}

#endif // CMR_DOMAIN_MEMORY_H
