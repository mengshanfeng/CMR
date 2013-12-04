/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>

/********************  MACRO  ***********************/
#define UNIT_TEST_USER_FRIENDS \
	FRIEND_TEST(TestDomainStorage,testIsContiguousGhost_false);\
	FRIEND_TEST(TestDomainStorage,testGetContiguousGhost);\
	FRIEND_TEST(TestDomainStorage,testGetCell);

/********************  HEADERS  *********************/
#include <domain/CMRDomainStorage.h>
#include <communication/CMRCommSchem.h>
#include "MockCommFactory.h"
#include <CMRDomainStorage.h>
#include <CMRGenericMemoryAccessor.h>
#include <CMRMemoryModels.h>
#include <mpi.h>

/**********************  USING  *********************/
using namespace testing;

/*********************  CLASS  **********************/
class TestDomainStorage : public Test
{
	public:
		virtual void SetUp ( void );
		virtual void TearDown ( void );
	protected:
		CMRDomainStorage * domain;
};

/*******************  FUNCTION  *********************/
void TestDomainStorage::SetUp ( void )
{
	domain = new CMRDomainStorage(8,CMRRect(0,0,800,600),2);
	domain->setMemoryAccessor(new CMRGenericMemoryAccessor<unsigned long,CMRMemoryModelRowMajor>());
}

/*******************  FUNCTION  *********************/
void TestDomainStorage::TearDown ( void )
{
	delete domain;
}

/*******************  FUNCTION  *********************/
TEST_F(TestDomainStorage,testIsContiguousGhost_true)
{
	CMRRect rect(0,0,800,1);
	EXPECT_TRUE(domain->isContiguousGhost(rect));
}

/*******************  FUNCTION  *********************/
TEST_F(TestDomainStorage,testIsContiguousGhost_false)
{
	CMRRect rect1(0,0,800,2);
	CMRRect rect2(0,0,1,600);
	EXPECT_FALSE(domain->isContiguousGhost(rect1));
	EXPECT_FALSE(domain->isContiguousGhost(rect2));
}

/*******************  FUNCTION  *********************/
TEST_F(TestDomainStorage,testGetCell)
{
	EXPECT_EQ(domain->data,domain->getCell(-2,-2));
}

/*******************  FUNCTION  *********************/
TEST_F(TestDomainStorage,testGetContiguousGhost)
{
	CMRRect rect(-2,-2,802,1);
	EXPECT_EQ(domain->data,domain->getContiguousGhost(rect));
}

/*******************  FUNCTION  *********************/
TEST_F(TestDomainStorage,testCopyGhostFromBuffer)
{
	//decl buffer
	unsigned char buffer[2*8*10];

	//setup default values
	for (int i = 0 ; i < 10 ; i++)
	{
		for (int k = 0 ; k < 8 ; k++)
		{
			buffer[i*8 + k] = i;
			buffer[10 * 8 + i * 8 + k] = 2*i;
		}
	}

	CMRRect rect(0,0,10,2);
	domain->copyGhostToBuffer(buffer,sizeof(buffer),rect);

	//check res
	for (int i = 0 ; i < 10 ; i++)
	{
		for (int k = 0 ; k < 8 ; k++)
		{
			((unsigned char*)domain->getCell(i,0))[k] = i;
			((unsigned char*)domain->getCell(i,1))[k] = 2*i;
			RecordProperty("i",i);
			RecordProperty("k",k);
			EXPECT_EQ(i,(int)(((unsigned char*)domain->getCell(i,0))[k]));
			EXPECT_EQ(2*i,(int)(((unsigned char*)domain->getCell(i,1))[k]));
		}
	}
}

/*******************  FUNCTION  *********************/
TEST_F(TestDomainStorage,testCopyGhostToBuffer)
{
	//setup default values
	for (int i = 0 ; i < 10 ; i++)
	{
		for (int k = 0 ; k < 8 ; k++)
		{
			((unsigned char*)domain->getCell(i,0))[k] = i;
			((unsigned char*)domain->getCell(i,1))[k] = 2*i;
		}
	}

	//decl buffer
	unsigned char buffer[2*8*10];
	CMRRect rect(0,0,10,2);
	domain->copyGhostToBuffer(buffer,sizeof(buffer),rect);

	//check res
	for (int i = 0 ; i < 10 ; i++)
	{
		for (int k = 0 ; k < 8 ; k++)
		{
			RecordProperty("i",i);
			RecordProperty("k",k);
			EXPECT_EQ(i,(int)(buffer[i*8 + k]));
			EXPECT_EQ(2*i,(int)(buffer[10 * 8 + i * 8 + k]));
		}
	}
}
