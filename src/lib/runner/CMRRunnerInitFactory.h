#ifndef CMR_RUNNER_INIT_H
#define CMR_RUNNER_INIT_H

/********************  HEADERS  *********************/
#include <common/CMRCommon.h>

/*********************  TYPES  **********************/
class CMRAbstractSpaceSplitter;
struct CMRRect;
class CMRDomainBuilder;
class CMRVarSystem;

/*********************  CLASS  **********************/
class CMRRunnerInitFactory
{
	public:
		virtual CMRAbstractSpaceSplitter * createSplitter(const CMRRect & globalRect) = 0;
		virtual CMRDomainBuilder * createDomainBuilder(CMRAbstractSpaceSplitter * splitter) = 0;
		virtual CMRVarSystem * createVarSystem(CMRDomainBuilder * domainBuilder) = 0;
		virtual int getLocalId(void) = 0;
		virtual int getSplittingSize(void) = 0;
};

/*********************  CLASS  **********************/
template <class TSplitter,class TBuilder,class TVarSys>
class CMRRunnerInitFactoryGeneric : public CMRRunnerInitFactory
{
	public:
		virtual CMRDomainBuilder* createDomainBuilder ( CMRAbstractSpaceSplitter* splitter ) { return new TBuilder(splitter);};
		virtual CMRAbstractSpaceSplitter* createSplitter ( const CMRRect& globalRect ) {return new TSplitter(globalRect,getSplittingSize(),0);};
		virtual CMRVarSystem* createVarSystem ( CMRDomainBuilder* domainBuilder ) {return new TVarSys(domainBuilder);};
};

/*********************  CLASS  **********************/
template <class TSplitter,class TBuilder,class TVarSys>
class CMRRunnerInitFactorySeqGeneric : public CMRRunnerInitFactoryGeneric<TSplitter,TBuilder,TVarSys>
{
	public:
		virtual int getLocalId ( void ) {return 0;};
		virtual int getSplittingSize ( void ) {return 1;};
};

/*********************  CLASS  **********************/
template <class TSplitter,class TBuilder,class TVarSys>
class CMRRunnerInitFactoryMPIGeneric : public CMRRunnerInitFactoryGeneric<TSplitter,TBuilder,TVarSys>
{
	public:
		virtual int getLocalId ( void ) {return cmrGetMPIRank();};
		virtual int getSplittingSize ( void ) {return cmrGetMPISize();};
};

#endif //CMR_RUNNER_INIT_H
