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
/**
 * Short wrapper to get the current MPI rank.
 * @TODO Add communicator as option.
**/
int cmrGetMPIRank ( void )
{
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	return rank;
}

/*******************  FUNCTION  *********************/
/**
 * Short wrapper to check if the current process is MPI master.
 * It implies rank equal to CMR_MPI_MASTER which is arbitrary fixed to 0.
**/
bool cmrIsMPIMaster ( void )
{
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	return rank == CMR_MPI_MASTER;
}

/*******************  FUNCTION  *********************/
/**
 * Return the number of MPI tasks in current communicator.
**/
int cmrGetMPISize ( void )
{
	int size;
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	return size;
}

/*******************  FUNCTION  *********************/
/**
 * Short function to get the PGCD of two numbers.
**/
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
