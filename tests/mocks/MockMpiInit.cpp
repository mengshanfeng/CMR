/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cstdlib>
#include <mpi.h>

/*******************  FUNCTION  *********************/
int MPI_Comm_rank(MPI_Comm comm, int *rank)
{
	*rank = 0;
	return MPI_SUCCESS;
}

/*******************  FUNCTION  *********************/
int MPI_Abort(MPI_Comm comm, int errorcode)
{
	abort();
	return MPI_SUCCESS;
}

/*******************  FUNCTION  *********************/
int MPI_Comm_size(MPI_Comm comm, int *size)
{
	*size = 1;
	return MPI_SUCCESS;
}
