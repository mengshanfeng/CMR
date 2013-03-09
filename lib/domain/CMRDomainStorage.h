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
#include <cassert>
#include "../common/CMRDebug.h"
#include "CMRAbstractDomain.h"
#include "CMRAbstractMemoryAccessor.h"

/*********************  CLASS  **********************/
class CMRDomainStorage : public CMRAbstractDomain
{
	ASSIST_UNIT_TEST( TestDomainStorage )
	public:
		CMRDomainStorage(size_t typeSize,const CMRRect & localDomain,int ghostDepth,int globalWidth = -1,int globalHeight = -1);
		virtual ~CMRDomainStorage(void);
		virtual int copyGhostFromBuffer ( const void* buffer, size_t size, const CMRRect& rect );
		virtual int copyGhostToBuffer ( void* buffer, size_t size, const CMRRect& rect ) const;
		virtual void* getContiguousGhost ( const CMRRect& rect );
		virtual bool isContiguousGhost ( const CMRRect& rect ) const;
		virtual size_t getGhostSize ( const CMRRect& rect ) const;
		virtual void* getCell ( int x, int y );
		int getMemoryWidth(void) const;
		CMRAbstractMemoryAccessor & getMemoryAccessor(void);
		void setMemoryAccessor(CMRAbstractMemoryAccessor * acc);
		bool hasMemoryAccessor(void) const;
	protected:
		int getMemoryCoord(int x,int y) const;
	private:
		unsigned char * data;
		CMRAbstractMemoryAccessor * acc;
};

#endif // CMR_DOMAIN_STORAGE_H
