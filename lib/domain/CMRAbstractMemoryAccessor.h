/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 02/2013
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_ABSTRACT_MEMORY_ACCESSOR_H
#define CMR_ABSTRACT_MEMORY_ACCESSOR_H

/********************  HEADERS  *********************/
#include <cstdlib>
#include "CMRDomainMemory.h"

/*********************  CLASS  **********************/
class CMRAbstractMemoryAccessor : public CMRDomainMemory
{
	public:
		CMRAbstractMemoryAccessor(void) {};
		CMRAbstractMemoryAccessor(CMRDomainMemory & memoryDomain) : CMRDomainMemory(memoryDomain) {};
		virtual ~CMRAbstractMemoryAccessor(void) {};
		virtual size_t getTypeSize(void) const = 0;
		virtual const void * getCell(int x,int y) const = 0;
		virtual void * getCell(int x,int y) = 0;
		virtual bool isContiguous(const CMRRect & rect) const = 0;
		virtual size_t copyFromBuffer ( const void* buffer, size_t size, const CMRRect & rect ) = 0;
		virtual size_t copyToBuffer ( void* buffer, size_t size, const CMRRect & rect ) const = 0;
		virtual size_t getBufferSize( const CMRRect & rect) const = 0;
};

#endif //CMR_ABSTRACT_MEMORY_ACCESSOR_H
