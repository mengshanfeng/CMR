/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <assert.h>
#include "CMRDebug.h"
#include "CMRCommon.h"
#include "CMRSpaceSplitter.h"

/*******************  FUNCTION  *********************/
CMRAbstractSpaceSplitter::CMRAbstractSpaceSplitter ( int x, int y,int width, int height,int nbSubDomains, int firstDim )
{
	//errors
	assert(width >= 1);
	assert(height >= 1);
	assert(nbSubDomains >= 1);
	assert(firstDim >= 0 && firstDim <= 1);
	
	this->rect.x = x;
	this->rect.y = y;
	this->rect.width = width;
	this->rect.height = height;
	this->firstDim = firstDim;
	this->nbSubDomains = nbSubDomains;
}

/*******************  FUNCTION  *********************/
int CMRAbstractSpaceSplitter::getNbSubdomains ( void ) const
{
	return this->nbSubDomains;
}

/*******************  FUNCTION  *********************/
void CMRAbstractSpaceSplitter::setNbSubdomains ( int nbSubDomains )
{
	assert(nbSubDomains >= 1);
	this->nbSubDomains = nbSubDomains;
}

/*******************  FUNCTION  *********************/
CMRAbstractSpaceSplitter::CMRAbstractSpaceSplitter ( CMRRect rect, int nbSubDomains,int firstDim )
{
	//errors
	assert(rect.width >= 1);
	assert(rect.height >= 1);
	assert(nbSubDomains >= 1);
	assert(firstDim >= 0 && firstDim <= 1);

	this->rect = rect;
	this->firstDim = firstDim;
	this->nbSubDomains = nbSubDomains;
}

/*******************  FUNCTION  *********************/
CMRBasicSpaceSplitter::CMRBasicSpaceSplitter ( int x, int y,int width, int height, int nbSubDomains, int firstDim ) 
	: CMRAbstractSpaceSplitter ( x,y,width, height,nbSubDomains, firstDim )
{
}

/*******************  FUNCTION  *********************/
CMRBasicSpaceSplitter::CMRBasicSpaceSplitter ( CMRRect rect,int nbSubDomains, int firstDim ) 
	: CMRAbstractSpaceSplitter ( rect,nbSubDomains,firstDim )
{
}

/*******************  FUNCTION  *********************/
CMRVect2D CMRBasicSpaceSplitter::getSplittingSize ( void ) const
{
	//vars
	CMRVect2D pbSize;
	CMRVect2D res;
	
	//errors
	assert(nbSubDomains >= 1);
	assert(this->firstDim == 0 || this->firstDim == 1);
	
	//setup input parameter (permut dims depending on dim priority)
	pbSize.x = this->rect.width;
	pbSize.y = this->rect.height;
	if (this->firstDim == 1)
		cmrPermut(pbSize.x,pbSize.y);
	
	//compute splitting, first split on coord 1 (contiguous), does max splitting
	//this is naive but work as first example, need improvement
	res.y = cmrPGCD(nbSubDomains,pbSize.y);
	res.x = nbSubDomains / res.y;

	//check
	assert(res.x * res.y == nbSubDomains);
	assert(pbSize.y % res.y == 0);
	if (pbSize.x % res.x != 0)
		fatal("Can't get a 2D cut for current problem size and number of processes.");
	
	//re-permut before returning
	if (this->firstDim == 1)
		cmrPermut(res.x,res.y);
	
	//return
	return res;
}

/*******************  FUNCTION  *********************/
int CMRBasicSpaceSplitter::getNeighbour ( int localId, int deltaX, int deltaY ) const
{
	//vars
	int id;

	//errors
	assert(localId >= 0 && localId < nbSubDomains);
	
	//trivial
	if (deltaX == 0 && deltaY == 0)
		return localId;
	
	//get splitting
	CMRVect2D split = getSplittingSize();
	CMRVect2D pos = getPosition(localId, split);
	CMRRect rect(0,0,split.x,split.y);
	CMRVect2D npos(pos);
	
	//compute new id
	switch(this->firstDim)
	{
		case 0:
			id = localId + deltaX + deltaY * split.x;
			break;
		case 1:
			id = localId + deltaX * split.y + deltaY;
			break;
		default:
			id = -1;
			fatal("Invalid value for firstDim : %d",firstDim);
	}
	
	//current pos
	npos.x += deltaX;
	npos.y += deltaY;
	
	//errors
	if (id >= 0 && localId < nbSubDomains && rect.contains(npos))
	{
		//debug("Get neigtboor of %d [ %d , %d ] -> ( %2d , %2d ) -> %d [ %d,  %d ] <-- OK",localId,pos.x,pos.y,deltaX,deltaY,id,npos.x,npos.y);
		return id;
	} else {
		//debug("Get neigtboor of %d [ %d , %d ] -> ( %2d , %2d ) -> %d [ %d,  %d ]",localId,pos.x,pos.y,deltaX,deltaY,id,npos.x,npos.y);
		return -1;
	}
}

/*******************  FUNCTION  *********************/
CMRSpaceSubDomains CMRBasicSpaceSplitter::getAllSubDomains ( void ) const
{
	//vars
	CMRSpaceSubDomains res;

	//loop on all
	for (int i = 0 ; i < this->nbSubDomains ; i++)
	{
		res.push_back(getLocalDomain(i));
	}
	
	//return it
	return res;
}

/*******************  FUNCTION  *********************/
CMRVect2D CMRBasicSpaceSplitter::getPosition ( int localId, CMRVect2D& splitting ) const
{
	CMRVect2D res;

	//errors
	assert(this->firstDim == 0 || this->firstDim == 1);
	assert(localId >= 0 && localId < nbSubDomains);

	//calc current rank position (ID)
	switch(this->firstDim)
	{
		case 0:
			res.x = localId % splitting.x;
			res.y = localId / splitting.x;
			break;
		case 1:
			res.x = localId / splitting.y;
			res.y = localId % splitting.y;
			break;
		default:
			fatal("Invalid value for firstDim : %d",firstDim);
	}

	return res;
}

/*******************  FUNCTION  *********************/
CMRRect CMRBasicSpaceSplitter::getLocalDomain ( int localId ) const
{
	//vars
	CMRRect res;
	
	//errors
	assert(localId >= 0 && localId < nbSubDomains);
	
	//get splitting
	CMRVect2D split = getSplittingSize();
	CMRVect2D pos = getPosition(localId,split);
	
	//compute position
	res.x = (rect.width * pos.x) / split.x;
	res.y = (rect.height * pos.y) / split.y;
	res.width = rect.width / split.x;
	res.height = rect.height / split.y;
	
	//this way to compute may support non multiple, but current splitting ensure mutiple...
	//res.width = ((rect.width * (rankX+1)) / split.x) - res.x;
	//res.height = ((rect.width * (rankY+1)) / split.y) - res.y;
	
	//apply global offset
	res.x += rect.x;
	res.y += rect.y;
	
	return res;
}

/*******************  FUNCTION  *********************/
void CMRAbstractSpaceSplitter::printDebug ( int printOnRank  ) const
{
	//nothing to do
	if (printOnRank != CMR_MPI_ALL && printOnRank != cmrGetMPIRank())
		return;
	
	//get splitting
	CMRVect2D splitting = getSplittingSize();
	
	debug("Domain splitting of [ %4d , %4d : %4d x %4d ] into ( %d x %d ) :",rect.x,rect.y,rect.width,rect.height,splitting.x,splitting.y);
	//loop on all
	for (int i = 0 ; i < this->nbSubDomains ; i++)
	{
		CMRRect tmp = getLocalDomain(i);
		debug("   - Subdomain %-2d : [ %4d , %4d : %4d x %4d ]",i, tmp.x,tmp.y,tmp.width, tmp.height);
	}
}

/*******************  FUNCTION  *********************/
const CMRRect& CMRAbstractSpaceSplitter::getDomain ( void ) const
{
	return rect;
}
