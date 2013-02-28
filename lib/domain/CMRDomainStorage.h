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

/*********************  CLASS  **********************/
class CMRDomainStorage : public CMRAbstractDomain
{
	ASSIST_UNIT_TEST( TestDomainStorage )
	public:
		CMRDomainStorage(size_t typeSize,int width,int height,int ghostDepth,int originX,int originY,int globalWidth = -1,int globalHeight = -1);
		virtual int copyGhostFromBuffer ( const void* buffer, size_t size, const CMRRect& rect );
		virtual int copyGhostToBuffer ( void* buffer, size_t size, const CMRRect& rect ) const;
		virtual void* getContiguousGhost ( const CMRRect& rect );
		virtual bool isContiguousGhost ( const CMRRect& rect ) const;
		virtual size_t getGhostSize ( const CMRRect& rect ) const;
		virtual void* getCell ( int x, int y );
		virtual bool isContiguous ( int directionID ) const;
		int getMemoryWidth(void) const;
	protected:
		int getCoord(int x,int y) const;
	private:
		char * data;
};

/*********************  CLASS  **********************/
//TODO cleanup this test code
template <class T>
class CMRTypedDomainStorage : public CMRDomainStorage
{
	public:
		class CellAccessor
		{
			public:
				CellAccessor(T * baseAddr,int width) {this->baseAddr = baseAddr; this->width = width;assert(width > 0);};
				CellAccessor(CellAccessor & orig,int relX,int relY) {this->width = orig.width; this->baseAddr = &orig.getCell(relX,relY);};
				T & getCell(int dx,int dy) {return baseAddr[dx + dy * width];};
			private:
				T * baseAddr;
				int width;
				///@TODO add max dx/dy to debug
				///@TODO add origX/origY in debug mode to help but of course, not necessary for normal run
		};

	public:
		CMRTypedDomainStorage ( int width, int height, int ghostDepth, int originX, int originY ):CMRDomainStorage(sizeof(T),width,height,ghostDepth,originX,originY){};
		CellAccessor getCellAccessor(int x,int y){CMRTypedDomainStorage<T>::CellAccessor acc((T*)getCell(x,y),getMemoryWidth()/sizeof(T));return acc;};
};

#endif // CMR_DOMAIN_STORAGE_H
