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
#include <CMRAbstractDomain.h>

/*********************  CLASS  **********************/
class MockAbstractDomain : public CMRAbstractDomain
{
	public:
		MockAbstractDomain(size_t typeSize,int width,int height,int ghostDepth,int originX,int originY);
		virtual bool isContiguousGhost(const CMRRect2D & rect) const;
		virtual size_t getGhostSize(const CMRRect2D & rect) const;
		virtual int copyGhostFromBuffer ( const void* buffer, size_t size, const CMRRect2D& rect );
		virtual int copyGhostToBuffer ( void* buffer, size_t size, const CMRRect2D& rect ) const;
		virtual void * getContiguousGhost(const CMRRect2D & rect);
		bool contiguous;
		void * buffer;
		int ghostSize;
};

#endif // MOCKABSTRACTDOMAIN_H
