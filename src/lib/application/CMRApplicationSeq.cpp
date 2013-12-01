/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <mpi.h>
#include "common/CMRDebug.h"
#include <common/CMRGeometry.h>
#include <common/CMRSpaceSplitter.h>
#include "CMRApplicationSeq.h"
#include <mpi/CMRMPIDomainBuilder.h>
#include <domain/CMRVarSystem.h>
#include <runner/CMRRunnerSeq.h>
#include <outputer/CMRBasicOutputer.h>

/*******************  FUNCTION  *********************/
CMRApplicationSeq::CMRApplicationSeq ( void )
{

}

/*******************  FUNCTION  *********************/
CMRApplicationSeq::~CMRApplicationSeq ( void )
{

}

/*******************  FUNCTION  *********************/
void CMRApplicationSeq::initLibs ( int& argc, char**& argv )
{
	initMPI(argc,argv);
	this->markStep(CMR_INIT_STATE_INIT_DEPS);
}

/*******************  FUNCTION  *********************/
void CMRApplicationSeq::initMPI ( int& argc, char**& argv )
{
	//check MPI status
	int res;
	MPI_Initialized(&res);
	
	//init MPI
	if (res == 0)
	{
		MPI_Init(&argc,&argv);
		info_on_master("Start MPI with np = %d",cmrGetMPISize());
		MPI_Barrier(MPI_COMM_WORLD);
	}
}

/*******************  FUNCTION  *********************/
void CMRApplicationSeq::finishLibs ( void )
{
	finishMPI();
	this->markStep(CMR_INIT_STATE_FINISH_DEPS);
}

/*******************  FUNCTION  *********************/
void CMRApplicationSeq::finishMPI ( void )
{
	//check MPI status
	int res;
	MPI_Finalized(&res);
	
	//Finish
	if (res == 0)
	{	
		MPI_Barrier(MPI_COMM_WORLD);
		info_on_master("Finish MPI");	
		MPI_Finalize();
	}
}

/*******************  FUNCTION  *********************/
void CMRApplicationSeq::init ( CMRVarSystem * varSystem,int width,int height, int writeInternval )
{
	CMRRect domainSize(0,0,width,height);
	CMRBasicSpaceSplitter * splitter = new CMRBasicSpaceSplitter(domainSize,1,0);
	CMRMPIDomainBuilder * domainBuilder = new CMRMPIDomainBuilder(splitter);
	varSystem->setDomainBuilder(domainBuilder);
	CMRRunnerSeq * runner = new CMRRunnerSeq();
	runner->init(varSystem,domainSize,splitter->getLocalDomain(domainBuilder->getLocalId()));
	
	//setup write system
	runner->setWriter(new CMRBasicOutputer("output-runner.raw",*splitter),writeInternval);
	
	this->markInitStep(runner);
}

/*******************  FUNCTION  *********************/
void CMRApplicationSeq::run ( int iterations )
{
	getRunner().run(iterations);
}

/*******************  FUNCTION  *********************/
void CMRApplicationSeq::finish ( void )
{
	//TODO
}
