/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#ifdef HAVE_OPENMP
#include <omp.h>
#endif //HAVE_OPENMP
#include "CMRRunnerOMPLoops.h"
#include "application/CMRCmdOptions.h"
#include "operations/CMRMeshOperation.h"
#include "common/CMRDebug.h"

/*******************  FUNCTION  *********************/
//TODO include CMROpenMP.h instead of omp.h
#ifndef HAVE_OPENMP
	#define omp_get_num_threads() 1
#endif

/*******************  FUNCTION  *********************/
CMRRunnerOMPLoops::CMRRunnerOMPLoops ( CMRCmdOptions & options )
 : CMRRunnerSeq(options)
{
}

/*******************  FUNCTION  *********************/
void CMRRunnerOMPLoops::runOperationNode ( CMRMeshOperationNode& opNode )
{
	opNode.op->runOmpLoops(system,opNode.rect);
}
