/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <mpi.h>
#include <cassert>
#include <cstdlib>
#include "CMRMPIReduction.h"
#include "CMRReductionDescriptor.h"
#include "../common/CMRDebug.h"
#include "../common/CMRCommon.h"

/*******************  FUNCTION  *********************/
CMRMPIReduction::CMRMPIReduction ( CMRReductionDescriptor* descriptor,int root )
{
	this->descriptor = descriptor;
	this->root = root;
}

/*******************  FUNCTION  *********************/
std::string CMRMPIReduction::getDebugString ( void ) const
{
	return "MPI Reduction (TODO)";
}

/*******************  FUNCTION  *********************/
void CMRMPIReduction::run ( void )
{
	if (isNative())
		runNative();
	else
		runCustom();
	descriptor->flush();
}

/*******************  FUNCTION  *********************/
bool CMRMPIReduction::isNative ( void )
{
	return (descriptor->getOperation() != CMR_REDUCE_OP_CUSTOM && descriptor->getTyep() != CMR_REDUCE_TYPE_CUSTOM);
}

/*******************  FUNCTION  *********************/
void CMRMPIReduction::runNative ( void )
{
	//vars
	CMRReductionOp cmrOp = descriptor->getOperation();
	CMRReductionType cmrType = descriptor->getTyep();
	MPI_Op mpiOp = MPI_SUM;
	MPI_Datatype mpiType = MPI_INT;
	void * buffer = descriptor->getBuffer();
	size_t bufferSize = descriptor->getBufferSize();
	size_t size = descriptor->getSize();
	int status;
	
	//errors
	assert(buffer != NULL);
	assert(size > 0 && bufferSize > 0);
	assert(bufferSize % size == 0);
	assert(root == CMR_ALL || root < cmrGetMPISize());

	//get MPI OP
	switch(cmrOp)
	{
		case CMR_REDUCE_OP_SUM:
			mpiOp = MPI_SUM;
			break;
		default:	
			fatal("Invalid reduce operation for usage in MPI native mode : %d",cmrOp);
	}
	
	//get MPI_TYPE
	switch(cmrType)
	{
		case CMR_REDUCE_TYPE_INT:
			assert(bufferSize % sizeof(int) == 0);
			mpiType = MPI_INT;
			break;
		case CMR_REDUCE_TYPE_FLOAT:
			assert(bufferSize % sizeof(float) == 0);
			mpiType = MPI_FLOAT;
			break;
		default:
			fatal("Invalid reduce type for usage in MPI native mdoe : %d",cmrType);
	}
	
	//run mpi reduce
	if (root == CMR_ALL)
		status = MPI_Allreduce(buffer,buffer,size,mpiType,mpiOp,MPI_COMM_WORLD);
	else
		status = MPI_Reduce(buffer,buffer,size,mpiType,mpiOp,root,MPI_COMM_WORLD);
	
	//check
	assume(status == MPI_SUCCESS,"Error while doing MPI reduction !");
}

/*******************  FUNCTION  *********************/
void CMRMPIReduction::runCustom ( void )
{
	assert(false);
	abort();
}
