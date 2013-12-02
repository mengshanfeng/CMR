/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef TEST_MOCK_COMM_H
#define TEST_MOCK_COMM_H

/********************  HEADERS  *********************/
#include <gmock/gmock.h>
#include <communication/CMRComm.h>

/*********************  CLASS  **********************/
class MockComm : public CMRComm
{
	public:
		MOCK_METHOD0(run,void(void));
};

#endif //TEST_MOCK_COMM_H
