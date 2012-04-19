#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <cassert>
#include <CMRDebug.h>
#include <CMRMPIComm.h>
#include <MockAbstractDomain.h>
#include <CMRMPICommFactory.h>
#include <CMRCommSchem.h>

using namespace std;

int main(int argc, char * argv[])
{
	//init MPI
	MPI_Init(&argc,&argv);
	info_on_master("Start with np = %d",cmrGetMPISize());
	MPI_Barrier(MPI_COMM_WORLD);

	assert(cmrGetMPISize() == 2);
	MockAbstractDomain domain(8,800,600,2,0,0);
	char buffer[1024];
	domain.buffer = buffer;
	domain.ghostSize = 1024;

	CMRCommSchem schem;
	if (cmrGetMPIRank() == 0)
	{
		domain.setCommunicator(1,0,new CMRMPICommFactory(1,0,1));
		domain.fillWithUpdateComm(schem,1,0,2,CMR_COMM_SEND);
	} else {
		domain.setCommunicator(-1,0,new CMRMPICommFactory(1,0,1));
		domain.fillWithUpdateComm(schem,-1,0,2,CMR_COMM_RECV);
	}

	//to comm
	schem.run();

	//Finish
	MPI_Barrier(MPI_COMM_WORLD);
	info_on_master("Finish");	
	MPI_Finalize();
	return EXIT_SUCCESS;
}
