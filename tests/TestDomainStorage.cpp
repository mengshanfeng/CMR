/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <svUnitTest.h>
#include <domain/CMRDomainStorage.h>
#include <communication/CMRCommSchem.h>
#include <MockCommunicator.h>
#include <domain/CMRDomainStorage.h>
#include <mpi.h>

/**********************  USING  *********************/
using namespace svUnitTest;

/*********************  CLASS  **********************/
class TestDomainStorage : public svutTestCase
{
	public:
		virtual void testMethodsRegistration ( void );
		virtual void setUp ( void );
		virtual void tearDown ( void );
	protected:
		void testIsContiguousGhost_true(void);
		void testIsContiguousGhost_false(void);
		void testGetContiguousGhost(void);
		void testGetCell(void);
		void testCopyGhostFromBuffer(void);
		void testCopyGhostToBuffer(void);

		CMRDomainStorage * domain;
};

/*******************  FUNCTION  *********************/
void TestDomainStorage::setUp ( void )
{
	domain = new CMRDomainStorage(8,CMRRect(0,0,800,600),2);
}

/*******************  FUNCTION  *********************/
void TestDomainStorage::tearDown ( void )
{
	delete domain;
}

/*******************  FUNCTION  *********************/
void TestDomainStorage::testMethodsRegistration ( void )
{
	SVUT_REG_TEST_METHOD(testIsContiguousGhost_true);
	SVUT_REG_TEST_METHOD(testIsContiguousGhost_false);
	SVUT_REG_TEST_METHOD(testGetContiguousGhost);
	SVUT_REG_TEST_METHOD(testGetCell);
	SVUT_REG_TEST_METHOD(testCopyGhostFromBuffer);
	SVUT_REG_TEST_METHOD(testCopyGhostToBuffer);
}

/*******************  FUNCTION  *********************/
void TestDomainStorage::testIsContiguousGhost_true ( void )
{
	CMRRect rect(0,0,800,1);
	SVUT_ASSERT_TRUE(domain->isContiguousGhost(rect));
}

/*******************  FUNCTION  *********************/
void TestDomainStorage::testIsContiguousGhost_false ( void )
{
	CMRRect rect1(0,0,800,2);
	CMRRect rect2(0,0,1,600);
	SVUT_ASSERT_FALSE(domain->isContiguousGhost(rect1));
	SVUT_ASSERT_FALSE(domain->isContiguousGhost(rect2));
}

/*******************  FUNCTION  *********************/
void TestDomainStorage::testGetCell ( void )
{
	SVUT_ASSERT_SAME(domain->data,domain->getCell(-2,-2));
}

/*******************  FUNCTION  *********************/
void TestDomainStorage::testGetContiguousGhost ( void )
{
	CMRRect rect(-2,-2,802,1);
	SVUT_ASSERT_SAME(domain->data,domain->getContiguousGhost(rect));
}

/*******************  FUNCTION  *********************/
void TestDomainStorage::testCopyGhostFromBuffer ( void )
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
			SVUT_SET_CONTEXT("i",i);
			SVUT_SET_CONTEXT("k",k);
			SVUT_ASSERT_EQUAL(i,(int)(((unsigned char*)domain->getCell(i,0))[k]));
			SVUT_ASSERT_EQUAL(2*i,(int)(((unsigned char*)domain->getCell(i,1))[k]));
		}
	}
}

/*******************  FUNCTION  *********************/
void TestDomainStorage::testCopyGhostToBuffer ( void )
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
			SVUT_SET_CONTEXT("i",i);
			SVUT_SET_CONTEXT("k",k);
			SVUT_ASSERT_EQUAL(i,(int)(buffer[i*8 + k]));
			SVUT_ASSERT_EQUAL(2*i,(int)(buffer[10 * 8 + i * 8 + k]));
		}
	}
}

/*******************  FUNCTION  *********************/
int MPI_Comm_rank(MPI_Comm comm, int *rank)
{
	*rank = 0;
}

/*******************  FUNCTION  *********************/
int MPI_Abort(MPI_Comm comm, int errorcode)
{
	abort();
}

/*******************  FUNCTION  *********************/
int MPI_Comm_size(MPI_Comm comm, int *size)
{
	*size = 1;
}

/********************  MACRO  ***********************/
SVUT_REGISTER_STANDELONE(TestDomainStorage)
