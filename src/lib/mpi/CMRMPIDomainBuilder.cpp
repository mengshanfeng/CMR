/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 02/2013
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "../domain/CMRVarSystem.h"
#include "../domain/CMRDomainStorage.h"
#include "CMRMPIDomainBuilder.h"
#include "../common/CMRSpaceSplitter.h"
#include "../mpi/CMRMPICommFactory.h"

/*******************  FUNCTION  *********************/
CMRMPIDomainBuilder::CMRMPIDomainBuilder ( CMRAbstractSpaceSplitter* splitter )
{
	this->splitter = splitter;
}

/*******************  FUNCTION  *********************/
int CMRMPIDomainBuilder::hashName ( const std::string& name ) const
{
	int res = 0;
	for (int i = 0 ; i < name.size() ; i++)
	{
		res += i*name[i];
	}
	
	return res;
}

/*******************  FUNCTION  *********************/
CMRDomainStorage* CMRMPIDomainBuilder::buildDomain ( const CMRVariable& variable )
{
	//get local id
	int localId = cmrGetMPIRank();
	
	//get local domain rect
	CMRRect rect = this->splitter->getLocalDomain(localId);
	CMRRect globalRect = this->splitter->getDomain();
	assert(globalRect.x == 0 && globalRect.y == 0);
	
	//build domain
	CMRDomainStorage * res = new CMRDomainStorage(variable.typeSize,rect,variable.ghostDepth,globalRect.width,globalRect.height);
	
	//plug MPI comms
	for (int dx = -1 ; dx <= 1 ; dx++)
	{
		for (int dy = -1 ; dy <= 1 ; dy++)
		{
			int remoteId = splitter->getNeighbour(localId,dx,dy);
			if (remoteId >= 0 && remoteId != localId)
				res->setCommunicator(dx,dy,new CMRMPICommFactory(remoteId,remoteId,hashName(variable.name)));
		}
	}
	
	//return
	return res;
}
