/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_BASIC_SEQ_RUNNER_H
#define CMR_BASIC_SEQ_RUNNER_H

/********************  HEADERS  *********************/
#include <vector>
#include "common/CMRGeometry.h"
#include <common/CMRSpaceSplitter.h>

//TODO cleanup
#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <cassert>
#include <common/CMRDebug.h>
#include <mpi/CMRMPIComm.h>
#include <mpi/CMRMPICommFactory.h>
#include <communication/CMRCommSchem.h>
#include <domain/CMRDomainStorage.h>
#include <common/CMRSpaceSplitter.h>
#include <operations/CMRMeshOperation.h>
#include <domain/CMRMemoryModels.h>
#include <domain/CMRCellAccessor.h>
#include <domain/CMRVarSystem.h>
#include <mpi/CMRMPIDomainBuilder.h>
#include <domain/CMRMemoryModels.h>
#include <math.h>
#include <cstdio>
#include <stdint.h>
#include "outputer/CMROutputer.h"
#include "CMRRunner.h"
#include "CMRRunnerSeq.h"
#include "CMRRunnerInitFactory.h"

/*********************  TYPES  **********************/
class CMRMeshOperation;
class CMRVarSystem;

/*********************  CLASS  **********************/
template <class TVarSystem>
class CMRBasicSeqRunner : public CMRRunnerSeq
{
	public:
		CMRBasicSeqRunner( int& argc, char**& argv,const CMRRect & globalDomainSize);
};

/*******************  FUNCTION  *********************/
template <class TVarSystem>
CMRBasicSeqRunner<TVarSystem>::CMRBasicSeqRunner ( int& argc, char**& argv, const CMRRect& globalDomainSize ) 
	: CMRRunnerSeq ( argc, argv, globalDomainSize, new CMRRunnerInitFactorySeqGeneric<CMRBasicSpaceSplitter,CMRMPIDomainBuilder,TVarSystem>())
{

}

#endif //CMR_BASIC_SEQ_RUNNER_H
