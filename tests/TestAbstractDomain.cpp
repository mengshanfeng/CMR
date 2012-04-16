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

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestAbstractDomain,testCornerLeft_800x600_1_1_recv)
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
SVUT_USE_DEFAULT_MAIN
