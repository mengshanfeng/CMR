/*****************************************************
             PROJECT  : CartesinMeshRuntime (CMR)
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef MOCK_ABSTRACT_DOMAIN_H
#define MOCK_ABSTRACT_DOMAIN_H

/********************  HEADERS  *********************/
#include <stdlib.h>
#include <domain/CMRAbstractDomain.h>

/*********************  CLASS  **********************/
class MockAbstractDomain : public CMRAbstractDomain
{
	public:
		MockAbstractDomain(size_t typeSize,int width,int height,int ghostDepth,int originX,int originY,int globalWidth = -1,int globalHeight = -1);
		virtual bool isContiguousGhost(const CMRRect & rect) const;
		virtual size_t getGhostSize(const CMRRect & rect) const;
		virtual int copyGhostFromBuffer ( const void* buffer, size_t size, const CMRRect& rect );
		virtual int copyGhostToBuffer ( void* buffer, size_t size, const CMRRect& rect ) const;
		virtual void * getContiguousGhost(const CMRRect & rect);
		virtual void* getCell ( int x, int y );
		virtual bool isContiguous ( int directionID ) const;
		bool contiguous;
		void * buffer;
		size_t ghostSize;
};

#endif // MOCKABSTRACTDOMAIN_H
