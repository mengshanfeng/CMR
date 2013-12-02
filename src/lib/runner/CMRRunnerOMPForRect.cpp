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
#include "CMRRunnerOMPForRect.h"
#include "common/CMRSpaceSplitter.h"
#include "application/CMRCmdOptions.h"
#include "operations/CMRMeshOperation.h"
#include "common/CMRDebug.h"

/*******************  FUNCTION  *********************/
//TODO include CMROpenMP.h instead of omp.h
#ifndef HAVE_OPENMP
	#define omp_get_num_threads() 1
#endif

/*******************  FUNCTION  *********************/
CMRRunnerOMPForRect::CMRRunnerOMPForRect ( CMRCmdOptions & options )
 : CMRRunnerSeq(options)
{
	#ifndef HAVE_OPENMP
	warning("Caution, you are using 'CMRRunnerOMPForRect' but CMR wasn't compile with OpenMP support, it will run sequentialy !");
	#endif//HAVE_OPENMP

	nbThreads = omp_get_num_threads();
	multiplier = options.getConfigInteger("runner_omp_for_rect:multiplier",4);
	minCells = options.getConfigInteger("runner_omp_for_rect:min_cells",1000);
}

/*******************  FUNCTION  *********************/
void CMRRunnerOMPForRect::runOperationNode ( CMRMeshOperationNode& opNode )
{	
	//if have enougth job, split in sub elements
	int jobs = nbThreads * multiplier;
	int cellsPerThread = opNode.rect.surface() / jobs;
	if (cellsPerThread <= minCells)
	{
		//sequential
		opNode.op->run(system,opNode.rect);
	} else {
		//ensure to have the allocation
		opNode.op->forceMeshAllocation(system,opNode.rect);
		//split and omp
		CMRBasicSpaceSplitter splitter(opNode.rect,jobs,0);
		//splitter.printDebug(0);
		#pragma omp parallel for
		for (int i = 0 ; i < jobs ; i++)
			opNode.op->run(system,splitter.getLocalDomain(i));
	}
}
