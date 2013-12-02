/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <gmock/gmock.h>

/********************  MACRO  ***********************/
#define UNIT_TEST_USER_FRIENDS \
	FRIEND_TEST(TestAbstractDomain,testComputeGhostCommRect_2_2_send);\
	FRIEND_TEST(TestAbstractDomain,testComputeGhostCommRect_2_2_recv);\
	FRIEND_TEST(TestAbstractDomain,testComputeGhostCommRect_2_1_send);\
	FRIEND_TEST(TestAbstractDomain,testComputeGhostCommRect_2_1_recv);\
	FRIEND_TEST(TestAbstractDomain,testComputeGhostCommRect_1_1_send);\
	FRIEND_TEST(TestAbstractDomain,testComputeGhostCommRect_1_1_recv);

/********************  HEADERS  *********************/
#include <CMRAbstractDomain.h>
#include <communication/CMRCommSchem.h>
#include <communication/CMRComm.h>
#include "MockAbstractDomain.h"
#include "MockCommFactory.h"
#include "MockComm.h"

/**********************  USING  *********************/
using namespace testing;

/*********************  CONSTS  *********************/
static const int CST_CNT_1_1                           = 3;
static const int CST_REQUEST_1_1      [CST_CNT_1_1][2] = { {-1,-1} , {-1,0} , {1,1} };
static const int CST_EXPECTED_1_1_RECV[CST_CNT_1_1][4] = { {0,0,1,1} , {1,1,1,1} , {}};
static const int CST_EXPECTED_1_1_SEND[CST_CNT_1_1][4] = {};

/*******************  FUNCTION  *********************/
TEST(TestAbstractDomain,testComputeGhostCommRect_1_1_recv)
{
	//constants
	const int cnt              = 3;
	const int request [cnt][2] = { {-1,-1}     , {-1,0}       , {1,1} };
	const int expected[cnt][4] = { {-1,-1,1,1} , {-1,0,1,600} , {800,600,1,1}};
	
	//create fake domain
    MockAbstractDomain domain(8,CMRRect(0,0,800,600),1);

	//loop on all
	for (int i = 0 ; i < cnt ; i++)
	{
		RecordProperty("i",i);
		RecordProperty("x",request[i][0]);
		RecordProperty("y",request[i][1]);
		//compute
		CMRRect exp(expected[i][0],expected[i][1],expected[i][2],expected[i][3]);
		CMRRect actual = domain.computeGhostCommRect(request[i][0],request[i][1],1,CMR_COMM_RECV);
		//test
		EXPECT_EQ(exp,actual);
	}
}

/*******************  FUNCTION  *********************/
TEST(TestAbstractDomain,testComputeGhostCommRect_1_1_send)
{
	//constants
	const int cnt              = 3;
	const int request [cnt][2] = { {-1,-1}   , {-1,0}      , {1,1} };
	const int expected[cnt][4] = { {0,0,1,1} , {0,0,1,600} , {799,599,1,1}};

	//create fake domain
    MockAbstractDomain domain(8,CMRRect(0,0,800,600),1);

	//loop on all
	for (int i = 0 ; i < cnt ; i++)
	{
		RecordProperty("i",i);
		RecordProperty("x",request[i][0]);
		RecordProperty("y",request[i][1]);
		//compute
		CMRRect exp(expected[i][0],expected[i][1],expected[i][2],expected[i][3]);
		CMRRect actual = domain.computeGhostCommRect(request[i][0],request[i][1],1,CMR_COMM_SEND);
		//test
		EXPECT_EQ(exp,actual);
	}
}

/*******************  FUNCTION  *********************/
TEST(TestAbstractDomain,testComputeGhostCommRect_2_1_recv)
{
	//constants
	const int cnt              = 3;
	const int request [cnt][2] = { {-1,-1}     , {-1,0}       , {1,1} };
	const int expected[cnt][4] = { {-1,-1,1,1} , {-1,0,1,600} , {800,600,1,1}};

	//create fake domain
    MockAbstractDomain domain(8,CMRRect(0,0,800,600),2);

	//loop on all
	for (int i = 0 ; i < cnt ; i++)
	{
		RecordProperty("i",i);
		RecordProperty("x",request[i][0]);
		RecordProperty("y",request[i][1]);
		//compute
		CMRRect exp(expected[i][0],expected[i][1],expected[i][2],expected[i][3]);
		CMRRect actual = domain.computeGhostCommRect(request[i][0],request[i][1],1,CMR_COMM_RECV);
		//test
		EXPECT_EQ(exp,actual);
	}
}

/*******************  FUNCTION  *********************/
TEST(TestAbstractDomain,testComputeGhostCommRect_2_1_send)
{
	//constants
	const int cnt              = 3;
	const int request [cnt][2] = { {-1,-1}   , {-1,0}      , {1,1} };
	const int expected[cnt][4] = { {0,0,1,1} , {0,0,1,600} , {799,599,1,1}};

	//create fake domain
    MockAbstractDomain domain(8,CMRRect(0,0,800,600),2);

	//loop on all
	for (int i = 0 ; i < cnt ; i++)
	{
		RecordProperty("i",i);
		RecordProperty("x",request[i][0]);
		RecordProperty("y",request[i][1]);
		//compute
		CMRRect exp(expected[i][0],expected[i][1],expected[i][2],expected[i][3]);
		CMRRect actual = domain.computeGhostCommRect(request[i][0],request[i][1],1,CMR_COMM_SEND);
		//test
		EXPECT_EQ(exp,actual);
	}
}

/*******************  FUNCTION  *********************/
TEST(TestAbstractDomain,testComputeGhostCommRect_2_2_recv)
{
	//constants
	const int cnt              = 3;
	const int request [cnt][2] = { {-1,-1}     , {-1,0}       , {1,1} };
	const int expected[cnt][4] = { {-2,-2,2,2} , {-2,0,2,600} , {800,600,2,2}};

	//create fake domain
    MockAbstractDomain domain(8,CMRRect(0,0,800,600),2);

	//loop on all
	for (int i = 0 ; i < cnt ; i++)
	{
		RecordProperty("i",i);
		RecordProperty("x",request[i][0]);
		RecordProperty("y",request[i][1]);
		//compute
		CMRRect exp(expected[i][0],expected[i][1],expected[i][2],expected[i][3]);
		CMRRect actual = domain.computeGhostCommRect(request[i][0],request[i][1],2,CMR_COMM_RECV);
		//test
		EXPECT_EQ(exp,actual);
	}
}

/*******************  FUNCTION  *********************/
TEST(TestAbstractDomain,testComputeGhostCommRect_2_2_send)
{
	//constants
	const int cnt              = 3;
	const int request [cnt][2] = { {-1,-1}   , {-1,0}      , {1,1} };
	const int expected[cnt][4] = { {0,0,2,2} , {0,0,2,600} , {798,598,2,2}};

	//create fake domain
    MockAbstractDomain domain(8,CMRRect(0,0,800,600),2);

	//loop on all
	for (int i = 0 ; i < cnt ; i++)
	{
		RecordProperty("i",i);
		RecordProperty("x",request[i][0]);
		RecordProperty("y",request[i][1]);
		//compute
		CMRRect exp(expected[i][0],expected[i][1],expected[i][2],expected[i][3]);
		CMRRect actual = domain.computeGhostCommRect(request[i][0],request[i][1],2,CMR_COMM_SEND);
		//test
		EXPECT_EQ(exp,actual);
	}
}

/*******************  FUNCTION  *********************/
TEST(TestAbstractDomain,testFillWithUpdateComm)
{
	//create fake domain
	MockAbstractDomain domain(8,CMRRect(0,0,800,600),1);
	
	//setup expected rect
	CMRRect rect(-1,-1,1,1);

	//setup communicator
	MockCommFactory * comm = new MockCommFactory;
	EXPECT_CALL(*comm,createComm(&domain,rect,CMR_COMM_RECV)).WillOnce(Return(new MockComm));

// 	CMRComm* ( CMRAbstractDomain* domain, const CMRRect& rect, CMRCommType commType 
	domain.setCommunicator(-1,-1,comm);
	
	//try comm
	CMRCommSchem schem;
	domain.fillWithUpdateComm(schem,-1,-1,1,CMR_COMM_RECV);

	//compare with answer
	EXPECT_EQ(1,schem.count());
	//MockComm * comm = (MockComm *)schem.getComm(0); <---------------------------------------
	CMRComm * res = schem.getComm(0);
	EXPECT_NE((void*)NULL,res);
}

/*******************  FUNCTION  *********************/
TEST(TestAbstractDomain,testIsFullyInDomain_true)
{
	//create fake domain
	MockAbstractDomain domain(8,CMRRect(0,0,800,600),1);

	//create some rects to test
	CMRRect rect1(0,0,800,600);
	CMRRect rect2(100,100,200,200);

	//check
	EXPECT_TRUE(domain.isFullyInLocalDomain(rect1));
	EXPECT_TRUE(domain.isFullyInLocalDomain(rect2));
}

/*******************  FUNCTION  *********************/
TEST(TestAbstractDomain,testIsFullyInDomain_false)
{
		//create fake domain
	MockAbstractDomain domain(8,CMRRect(0,0,800,600),1);

	//create some rects to test
	CMRRect rect1(-1,0,800,600);
	CMRRect rect2(0,-1,800,600);
	CMRRect rect3(0,0,801,600);
	CMRRect rect4(0,0,800,601);

	//check
	EXPECT_FALSE(domain.isFullyInLocalDomain(rect1));
	EXPECT_FALSE(domain.isFullyInLocalDomain(rect2));
	EXPECT_FALSE(domain.isFullyInLocalDomain(rect3));
	EXPECT_FALSE(domain.isFullyInLocalDomain(rect4));
}

/*******************  FUNCTION  *********************/
TEST(TestAbstractDomain,testIsFullyInDomainMemory_true)
{
	//create fake domain
	MockAbstractDomain domain(8,CMRRect(0,0,800,600),1);

	//create some rects to test
	CMRRect rect1(-1,-1,802,602);
	CMRRect rect2(-1,-1,200,200);
	CMRRect rect3(100,100,200,200);
	CMRRect rect4(-1,0,800,600);
	CMRRect rect5(0,-1,800,600);
	CMRRect rect6(0,0,801,600);
	CMRRect rect7(0,0,800,601);

	//check
	EXPECT_TRUE(domain.isFullyInDomainMemory(rect1));
	EXPECT_TRUE(domain.isFullyInDomainMemory(rect2));
	EXPECT_TRUE(domain.isFullyInDomainMemory(rect3));
	EXPECT_TRUE(domain.isFullyInDomainMemory(rect4));
	EXPECT_TRUE(domain.isFullyInDomainMemory(rect5));
	EXPECT_TRUE(domain.isFullyInDomainMemory(rect6));
	EXPECT_TRUE(domain.isFullyInDomainMemory(rect7));
}

/*******************  FUNCTION  *********************/
TEST(TestAbstractDomain,testIsFullyInDomainMemory_false)
{
		//create fake domain
	MockAbstractDomain domain(8,CMRRect(0,0,800,600),1);

	//create some rects to test
	CMRRect rect1(-2,0,800,600);
	CMRRect rect2(0,-2,800,600);
	CMRRect rect3(0,0,802,600);
	CMRRect rect4(0,0,800,602);

	//check
	EXPECT_FALSE(domain.isFullyInDomainMemory(rect1));
	EXPECT_FALSE(domain.isFullyInDomainMemory(rect2));
	EXPECT_FALSE(domain.isFullyInDomainMemory(rect3));
	EXPECT_FALSE(domain.isFullyInDomainMemory(rect4));
}
