/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <svUnitTest.h>
#include <CMRAbstractDomain.h>
#include <CMRCommSchem.h>
#include <MockAbstractDomain.h>
#include <MockCommunicator.h>

/**********************  USING  *********************/
using namespace svUnitTest;

/*********************  CONSTS  *********************/
static const int CST_CNT_1_1                           = 3;
static const int CST_REQUEST_1_1      [CST_CNT_1_1][2] = { {-1,-1} , {-1,0} , {1,1} };
static const int CST_EXPECTED_1_1_RECV[CST_CNT_1_1][4] = { {0,0,1,1} , {1,1,1,1} , {}};
static const int CST_EXPECTED_1_1_SEND[CST_CNT_1_1][4] = {};

/*********************  CLASS  **********************/
class TestAbstractDomain : public svutTestCase
{
	public:
		virtual void testMethodsRegistration ( void );
	protected:
		void testComputeGhostCommRect_1_1_recv(void);
		void testComputeGhostCommRect_1_1_send(void);
		void testComputeGhostCommRect_2_1_recv(void);
		void testComputeGhostCommRect_2_1_send(void);
		void testComputeGhostCommRect_2_2_recv(void);
		void testComputeGhostCommRect_2_2_send(void);
		void testFillWithUpdateComm(void);
};

/*******************  FUNCTION  *********************/
void TestAbstractDomain::testMethodsRegistration ( void )
{
	SVUT_REG_TEST_METHOD(testComputeGhostCommRect_1_1_recv);
	SVUT_REG_TEST_METHOD(testComputeGhostCommRect_1_1_send);
	SVUT_REG_TEST_METHOD(testComputeGhostCommRect_2_1_recv);
	SVUT_REG_TEST_METHOD(testComputeGhostCommRect_2_1_send);
	SVUT_REG_TEST_METHOD(testComputeGhostCommRect_2_2_recv);
	SVUT_REG_TEST_METHOD(testComputeGhostCommRect_2_2_send);
	SVUT_REG_TEST_METHOD(testFillWithUpdateComm);
}

/*******************  FUNCTION  *********************/
void TestAbstractDomain::testComputeGhostCommRect_1_1_recv ( void )
{
	//constants
	const int cnt              = 3;
	const int request [cnt][2] = { {-1,-1}   , {-1,0}      , {1,1} };
	const int expected[cnt][4] = { {0,0,1,1} , {0,1,1,600} , {801,601,1,1}};
	
	//create fake domain
    MockAbstractDomain domain(8,800,600,1,0,0);

	//loop on all
	for (int i = 0 ; i < cnt ; i++)
	{
		SVUT_SET_CONTEXT("i",i);
		SVUT_SET_CONTEXT("x",request[i][0]);
		SVUT_SET_CONTEXT("y",request[i][1]);
		//compute
		CMRRect2D exp(expected[i][0],expected[i][1],expected[i][2],expected[i][3]);
		CMRRect2D actual = domain.computeGhostCommRect(request[i][0],request[i][1],1,CMR_COMM_RECV);
		//test
		SVUT_ASSERT_EQUAL(exp,actual);
	}
}

/*******************  FUNCTION  *********************/
void TestAbstractDomain::testComputeGhostCommRect_1_1_send ( void )
{
	//constants
	const int cnt              = 3;
	const int request [cnt][2] = { {-1,-1}   , {-1,0}      , {1,1} };
	const int expected[cnt][4] = { {1,1,1,1} , {1,1,1,600} , {800,600,1,1}};

	//create fake domain
    MockAbstractDomain domain(8,800,600,1,0,0);

	//loop on all
	for (int i = 0 ; i < cnt ; i++)
	{
		SVUT_SET_CONTEXT("i",i);
		SVUT_SET_CONTEXT("x",request[i][0]);
		SVUT_SET_CONTEXT("y",request[i][1]);
		//compute
		CMRRect2D exp(expected[i][0],expected[i][1],expected[i][2],expected[i][3]);
		CMRRect2D actual = domain.computeGhostCommRect(request[i][0],request[i][1],1,CMR_COMM_SEND);
		//test
		SVUT_ASSERT_EQUAL(exp,actual);
	}
}

/*******************  FUNCTION  *********************/
void TestAbstractDomain::testComputeGhostCommRect_2_1_recv ( void )
{
	//constants
	const int cnt              = 3;
	const int request [cnt][2] = { {-1,-1}   , {-1,0}      , {1,1} };
	const int expected[cnt][4] = { {1,1,1,1} , {1,2,1,600} , {802,602,1,1}};

	//create fake domain
    MockAbstractDomain domain(8,800,600,2,0,0);

	//loop on all
	for (int i = 0 ; i < cnt ; i++)
	{
		SVUT_SET_CONTEXT("i",i);
		SVUT_SET_CONTEXT("x",request[i][0]);
		SVUT_SET_CONTEXT("y",request[i][1]);
		//compute
		CMRRect2D exp(expected[i][0],expected[i][1],expected[i][2],expected[i][3]);
		CMRRect2D actual = domain.computeGhostCommRect(request[i][0],request[i][1],1,CMR_COMM_RECV);
		//test
		SVUT_ASSERT_EQUAL(exp,actual);
	}
}

/*******************  FUNCTION  *********************/
void TestAbstractDomain::testComputeGhostCommRect_2_1_send ( void )
{
	//constants
	const int cnt              = 3;
	const int request [cnt][2] = { {-1,-1}   , {-1,0}      , {1,1} };
	const int expected[cnt][4] = { {2,2,1,1} , {2,2,1,600} , {801,601,1,1}};

	//create fake domain
    MockAbstractDomain domain(8,800,600,2,0,0);

	//loop on all
	for (int i = 0 ; i < cnt ; i++)
	{
		SVUT_SET_CONTEXT("i",i);
		SVUT_SET_CONTEXT("x",request[i][0]);
		SVUT_SET_CONTEXT("y",request[i][1]);
		//compute
		CMRRect2D exp(expected[i][0],expected[i][1],expected[i][2],expected[i][3]);
		CMRRect2D actual = domain.computeGhostCommRect(request[i][0],request[i][1],1,CMR_COMM_SEND);
		//test
		SVUT_ASSERT_EQUAL(exp,actual);
	}
}

/*******************  FUNCTION  *********************/
void TestAbstractDomain::testComputeGhostCommRect_2_2_recv ( void )
{
	//constants
	const int cnt              = 3;
	const int request [cnt][2] = { {-1,-1}   , {-1,0}      , {1,1} };
	const int expected[cnt][4] = { {0,0,2,2} , {0,2,2,600} , {802,602,2,2}};

	//create fake domain
    MockAbstractDomain domain(8,800,600,2,0,0);

	//loop on all
	for (int i = 0 ; i < cnt ; i++)
	{
		SVUT_SET_CONTEXT("i",i);
		SVUT_SET_CONTEXT("x",request[i][0]);
		SVUT_SET_CONTEXT("y",request[i][1]);
		//compute
		CMRRect2D exp(expected[i][0],expected[i][1],expected[i][2],expected[i][3]);
		CMRRect2D actual = domain.computeGhostCommRect(request[i][0],request[i][1],2,CMR_COMM_RECV);
		//test
		SVUT_ASSERT_EQUAL(exp,actual);
	}
}

/*******************  FUNCTION  *********************/
void TestAbstractDomain::testComputeGhostCommRect_2_2_send ( void )
{
	//constants
	const int cnt              = 3;
	const int request [cnt][2] = { {-1,-1}   , {-1,0}      , {1,1} };
	const int expected[cnt][4] = { {2,2,2,2} , {2,2,2,600} , {800,600,2,2}};

	//create fake domain
    MockAbstractDomain domain(8,800,600,2,0,0);

	//loop on all
	for (int i = 0 ; i < cnt ; i++)
	{
		SVUT_SET_CONTEXT("i",i);
		SVUT_SET_CONTEXT("x",request[i][0]);
		SVUT_SET_CONTEXT("y",request[i][1]);
		//compute
		CMRRect2D exp(expected[i][0],expected[i][1],expected[i][2],expected[i][3]);
		CMRRect2D actual = domain.computeGhostCommRect(request[i][0],request[i][1],2,CMR_COMM_SEND);
		//test
		SVUT_ASSERT_EQUAL(exp,actual);
	}
}

/*******************  FUNCTION  *********************/
void TestAbstractDomain::testFillWithUpdateComm ( void )
{
	//create fake domain
	MockAbstractDomain domain(8,800,600,1,0,0);

	//setup communicator
	domain.setCommunicator(-1,-1,new MockCommunicator);
	
	//try comm
	CMRCommSchem schem;
	domain.fillWithUpdateComm(schem,-1,-1,1,CMR_COMM_RECV);

	//setup expected rect
	CMRRect2D rect(0,0,1,1);

	//compare with answer
	SVUT_ASSERT_EQUAL(1,schem.count());
	MockCommunication * comm = (MockCommunication *)schem.getComm(0);
	SVUT_ASSERT_EQUAL(rect,comm->rect);
}

/********************  MACRO  ***********************/
SVUT_REGISTER_STANDELONE(TestAbstractDomain)
