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
#include "../common/CMRGeometry.h"

/*********************  CLASS  **********************/
class CMRMemoryModel
{
	public:
		virtual ~CMRMemoryModel(void) {};
		virtual int getTypeSize(void) = 0;
		virtual bool isContiguous(const CMRRect & memoryRect,const CMRRect & rect) = 0;
		virtual void * getCell(void * baseAddr,int x,int y,int width,int height) = 0;
		virtual int copyGhostFromBuffer ( void * mesh, const void* buffer, size_t size, const CMRRect & memoryRect, const CMRRect & rect ) = 0;
		virtual int copyGhostToBuffer ( const void * mesh,void* buffer, size_t size, const CMRRect & memoryRect, const CMRRect & rect ) = 0;
};

/*********************  CLASS  **********************/
template <class T>
class CMRMemoryModelRowMajorAccessor
{
	public:
		CMRMemoryModelRowMajorAccessor(T * baseAddr, const CMRRect & memoryRect,const CMRVect2D & absPosition);
		CMRMemoryModelRowMajorAccessor(const CMRMemoryModelRowMajorAccessor<T> & orig,int relX,int relY);
		T & getCell(int dx,int dy);
		CMRVect2D getAbsPosition(int dx = 0,int dy = 0) const;
	private:
		T * ptr;
		CMRRect memoryRect;
		CMRVect2D absPosition;
};

/*********************  CLASS  **********************/
template <class T>
class CMRMemoryModelRowMajor : public CMRMemoryModel
{
	public:
		typedef CMRMemoryModelRowMajorAccessor<T> Accessor;
	public:
		virtual int getTypeSize ( void );
		virtual int getCellId(int x,int y,int width,int height);
		virtual bool isContiguous(const CMRRect & memoryRect,const CMRRect& rect);
		virtual void* getCell ( void* baseAddr, int x, int y, int width, int height);
		virtual int copyGhostFromBuffer ( void * mesh, const void* buffer, size_t size, const CMRRect & memoryRect, const CMRRect & rect );
		virtual int copyGhostToBuffer ( const void * mesh,void* buffer, size_t size, const CMRRect & memoryRect, const CMRRect & rect );
		static T & getTypedCell( void* baseAddr, int x, int y, int width, int height);
};

/*******************  FUNCTION  *********************/
template <class T>
void* CMRMemoryModelRowMajor<T>::getCell(void* baseAddr, int x, int y, int width, int height)
{
	return &CMRMemoryModelRowMajor<T>::getTypedCell(baseAddr,x,y,width,height);
}

/*******************  FUNCTION  *********************/
template <class T>
T& CMRMemoryModelRowMajor<T>::getTypedCell(void* baseAddr, int x, int y, int width, int height)
{
	T * ptr = (T*)baseAddr;
	return ptr[x + y * width];
}

/*******************  FUNCTION  *********************/
template <class T>
int CMRMemoryModelRowMajor<T>::getCellId(int x, int y, int width, int height)
{
	return x + y * width;
}

/*******************  FUNCTION  *********************/
template <class T>
int CMRMemoryModelRowMajor<T>::getTypeSize(void )
{
	 return sizeof(T);
}

/*******************  FUNCTION  *********************/
template <class T>
CMRMemoryModelRowMajorAccessor<T>::CMRMemoryModelRowMajorAccessor(T* baseAddr, const CMRRect& memoryRect, const CMRVect2D& absPosition)
{
	this->ptr = baseAddr;
	this->memoryRect = memoryRect;
	this->absPosition = absPosition;
	//move to position
	this->ptr = &this->getCell(absPosition.x - memoryRect.x,absPosition.y - memoryRect.y);
}

/*******************  FUNCTION  *********************/
template <class T>
CMRMemoryModelRowMajorAccessor<T>::CMRMemoryModelRowMajorAccessor(const CMRMemoryModelRowMajorAccessor<T>& orig, int relX, int relY)
{
	this->ptr = &orig.getCell(relX,relY);
	this->memoryRect = orig.memoryRect;
	this->absPosition = orig.absPosition.getRel(relX,relY);
}

/*******************  FUNCTION  *********************/
template <class T>
T& CMRMemoryModelRowMajorAccessor<T>::getCell(int dx, int dy)
{
	assert(memoryRect.contains(absPosition.getRel(dx,dy)));
	return ptr[dx + memoryRect.width * dy];
}

/*******************  FUNCTION  *********************/
template <class T>
CMRVect2D CMRMemoryModelRowMajorAccessor<T>::getAbsPosition(int dx,int dy) const
{
	return absPosition.getRel(dx,dy);
}

/*******************  FUNCTION  *********************/
template <class T>
bool CMRMemoryModelRowMajor<T>::isContiguous(const CMRRect & memoryRect, const CMRRect& rect)
{
	return rect.height == 1 || rect.width == memoryRect.width;
}

/*******************  FUNCTION  *********************/
template <class T>
int CMRMemoryModelRowMajor<T>::copyGhostFromBuffer(void* mesh, const void* buffer, size_t size, const CMRRect& memoryRect, const CMRRect& rect)
{

}

/*******************  FUNCTION  *********************/
template <class T>
int CMRMemoryModelRowMajor<T>::copyGhostToBuffer(const void* mesh, void* buffer, size_t size, const CMRRect& memoryRect, const CMRRect& rect)
{

}

#endif // CMR_SPACE_SPLITTER_H
