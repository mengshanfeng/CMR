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
#include "common/CMRGeometry.h"
#include "domain/CMRVarSystem.h"
#include "runner/CMRRunnerSeq.h"
#include "common/CMRSpaceSplitter.h"
#include "mpi/CMRMPIDomainBuilder.h"
#include "outputer/CMRBasicOutputer.h"
#include "CMRApplicationSeq.h"
#include "CMRCmdOptions.h"

/*******************  FUNCTION  *********************/
CMRApplicationSeq::CMRApplicationSeq ( int& argc, char**& argv,CMRVarSystem * varSystem,int width,int height,int writeInterval )
	:options(width,height)
{
	this->initLibs(argc,argv);
	options.parse(argc,(const char**)argv);
	this->init(varSystem,options.getWidth(),options.getHeight(),writeInterval);
}

/*******************  FUNCTION  *********************/
CMRApplicationSeq::~CMRApplicationSeq ( void )
{
	this->finish();
	this->finishLibs();
}

/*******************  FUNCTION  *********************/
void CMRApplicationSeq::initLibs ( int& argc, char**& argv )
{
	initMPI(argc,argv);
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
	info_on_master("Create global mesh ( %d x %d )",width,height);
	
	CMRRect domainSize(0,0,width,height);
	CMRBasicSpaceSplitter * splitter = new CMRBasicSpaceSplitter(domainSize,1,0);
	CMRMPIDomainBuilder * domainBuilder = new CMRMPIDomainBuilder(splitter);
	varSystem->setDomainBuilder(domainBuilder);
	CMRRunnerSeq * runner = new CMRRunnerSeq();
	runner->init(varSystem,domainSize,splitter->getLocalDomain(domainBuilder->getLocalId()));
	
	//print local splitting
	if (options.getConfigBoolean("debug:mpi_domains",false))
		cmrPrint("MPI local domain of process %d is ( %d x %d )",cmrGetMPIRank(),width,height);
	
	//setup write system
	runner->setWriter(new CMRBasicOutputer("output-runner.raw",*splitter),writeInternval);
	
	this->setRunner(runner);
}

/*******************  FUNCTION  *********************/
void CMRApplicationSeq::run ( int iterations )
{
	if (iterations == -1)
		iterations = options.getIterations();
	getRunner().run(iterations);
}

/*******************  FUNCTION  *********************/
void CMRApplicationSeq::finish ( void )
{
	//TODO
}
