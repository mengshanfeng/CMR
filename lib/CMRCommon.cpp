/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <mpi.h>
#include "CMRCommon.h"

/*******************  FUNCTION  *********************/
int cmrGetMPIRank ( void )
{
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	return rank;
}

/*******************  FUNCTION  *********************/
bool cmrIsMPIMaster ( void )
{
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	return rank == CMR_MPI_MASTER;
}

/*******************  FUNCTION  *********************/
int cmrGetMPISize ( void )
{
	int size;
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	return size;
}

/*******************  FUNCTION  *********************/
int cmrPGCD(int a, int b)
{
	int c;
	while(b!=0)
	{
		c = a % b;
		a = b;
		b = c;
	}
	return a;
}
