/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_DOMAIN_STORAGE_H
#define CMR_DOMAIN_STORAGE_H

/********************  HEADERS  *********************/
#include "CMRAbstractDomain.h"

/*********************  CLASS  **********************/
class CMRDomainStorage : public CMRAbstractDomain
{
	ASSIST_UNIT_TEST( TestDomainStorage )
	public:
		CMRDomainStorage(size_t typeSize,int width,int height,int ghostDepth,int originX,int originY);
		virtual int copyGhostFromBuffer ( const void* buffer, size_t size, const CMRRect2D& rect );
		virtual int copyGhostToBuffer ( void* buffer, size_t size, const CMRRect2D& rect ) const;
		virtual void* getContiguousGhost ( const CMRRect2D& rect );
		virtual bool isContiguousGhost ( const CMRRect2D& rect ) const;
		virtual size_t getGhostSize ( const CMRRect2D& rect ) const;
		virtual void* getCell ( int x, int y );
		virtual bool isContiguous ( int directionID ) const;
	protected:
		int getCoord(int x,int y) const;
	private:
		char * data;
};

#endif // CMR_DOMAIN_STORAGE_H
