/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 02/2013
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_MEMORY_MODEL_H
#define CMR_MEMORY_MODEL_H

/********************  HEADERS  *********************/
#include <vector>
#include <cassert>
#include "CMRAbstractDomain.h"
#include "../common/CMRDebug.h"

/*********************  CLASS  **********************/
class CMRMemoryModel
{
	public:
		virtual ~CMRMemoryModel(void) {};
		virtual int getTypeSize(void) = 0;
		virtual bool isContiguous(int directionId) = 0;
		virtual void * getCell(void * baseAddr,int x,int y,int width,int height) = 0;
		//virtual int copyGhostFromBuffer ( void * mesh, const void* buffer, size_t size, const CMRRect2D& rect ) = 0;
		//virtual int copyGhostToBuffer ( const void * mesh,void* buffer, size_t size, const CMRRect2D& rect ) = 0;
};

/*********************  CLASS  **********************/
template <class T>
class CMRMemoryModelRowMajor : public CMRMemoryModel
{
	class Accessor
	{
		public:
			Accessor(T * baseAddr,int width,int absX,int absY) {this->baseAddr = baseAddr; this->width = width;assert(width > 0);this->absX = absX; this->absY = absY;};
			Accessor(const Accessor & orig,int relX,int relY) {this->width = orig.width; this->baseAddr = &orig.getCell(relX,relY); this->absX = orig.absX + relX; this->absY = orig.absY +relY;};
			T & getCell(int dx,int dy) {return baseAddr[dx + dy * width];};
			Vect2D getCoord(void) const;
		private:
			T * baseAddr;
			int width;
			int absX;
			int absY;
			///@TODO add max dx/dy to debug
			///@TODO add origX/origY in debug mode to help but of course, not necessary for normal run
	};
	
	public:
		virtual int getTypeSize ( void ) {return sizeof(T);};
		virtual int getCellId(int x,int y,int width,int height) {return (x+y*width);};
		virtual bool isContiguous ( int directionId ) { return (directionId == 0);};
		virtual void* getCell ( void* baseAddr, int x, int y, int width, int height) {return (void*)((unsigned long)baseAddr + (getCellId() * getTypeSize()));};
		T & getTypedCell( void* baseAddr, int x, int y, int width, int height) {return *(T*)getCell(baseAddr,x,y,width,height);};
};

/*
ComposedDomains
{
	Domains * State1[NB_MAX];
	Domains * State2[NB_MAX];
	
	//avec par default (sauf si étape none locales) st1=st2 pour chaque var
};


class System
{
	Variable<MemoryModel<float>> a;
	Variable<MemoryModel<int>> b;
	
	buildDomains();
};

class SystemAccessor
{
	getA(int dx = 0,int dy = 0);
	getB(int dx = 0,int dy = 0);
	void * a;
	void * b;
	int x;
	int y;
};
*/

#endif // CMR_SPACE_SPLITTER_H
