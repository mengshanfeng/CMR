#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <cassert>
#include <common/CMRDebug.h>
#include <communication/CMRMPIComm.h>
#include <MockAbstractDomain.h>
#include <communication/CMRMPICommFactory.h>
#include <communication/CMRCommSchem.h>
#include <domain/CMRDomainStorage.h>
#include <common/CMRSpaceSplitter.h>

using namespace std;

int main(int argc, char * argv[])
{
	//init MPI
	MPI_Init(&argc,&argv);
	info_on_master("Start with np = %d",cmrGetMPISize());
	MPI_Barrier(MPI_COMM_WORLD);

	CMRDomainStorage domain(8,800,600,1,0,0);
	
	CMRBasicSpaceSplitter splitter(0,0,800,600,cmrGetMPISize(),0);
	splitter.printDebug(CMR_MPI_MASTER);
	
	assert(cmrGetMPISize() == 2);
	
	CMRCommSchem schem("sync_left_right");
	switch (cmrGetMPIRank())
	{
		case 0:
			domain.setCommunicator(1,0,new CMRMPICommFactory(1,1,1));
			domain.fillWithUpdateComm(schem,1,0,1,CMR_COMM_SEND);
			break;
		case 1:
			domain.setCommunicator(-1,0,new CMRMPICommFactory(0,0,1));
			domain.fillWithUpdateComm(schem,-1,0,1,CMR_COMM_RECV);
			break;
		default:
			fatal("This test only work with exactly 2 MPI tasks.");
	}

	//to comm
	schem.printDebug();
	schem.run();

	//Finish
	MPI_Barrier(MPI_COMM_WORLD);
	info_on_master("Finish");	
	MPI_Finalize();
	return EXIT_SUCCESS;
}
