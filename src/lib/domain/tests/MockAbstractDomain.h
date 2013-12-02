/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef TEST_MOCK_ABSTRACT_DOMAIN_H
#define TEST_MOCK_ABSTRACT_DOMAIN_H

/********************  HEADERS  *********************/
#include <gmock/gmock.h>
#include <CMRAbstractDomain.h>

/*********************  CLASS  **********************/
class MockAbstractDomain : public CMRAbstractDomain {
	public:
		MockAbstractDomain(size_t typeSize, const CMRRect & rect, int ghostDepth,int globalWidth = -1,int globalHeight = -1);
		MOCK_CONST_METHOD1(isContiguousGhost,bool(const CMRRect & rect));
		MOCK_CONST_METHOD1(getGhostSize,size_t(const CMRRect & rect));
		MOCK_METHOD3(copyGhostFromBuffer,int( const void* buffer, size_t size, const CMRRect& rect ));
		MOCK_CONST_METHOD3(copyGhostToBuffer,int( void* buffer, size_t size, const CMRRect& rect ));
		MOCK_METHOD1(getContiguousGhost,void *(const CMRRect & rect));
		MOCK_METHOD2(getCell,void*( int x, int y ));
		MOCK_CONST_METHOD1(isContiguous, bool( int directionID ));
};

/*******************  FUNCTION  *********************/
inline MockAbstractDomain::MockAbstractDomain(size_t typeSize, const CMRRect & rect, int ghostDepth,int globalWidth,int globalHeight)
	:CMRAbstractDomain ( typeSize, rect,ghostDepth,globalWidth,globalHeight)
{
}

#endif //TEST_MOCK_ABSTRACT_DOMAIN_H
