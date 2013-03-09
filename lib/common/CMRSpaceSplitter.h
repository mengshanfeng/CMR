/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_SPACE_SPLITTER_H
#define CMR_SPACE_SPLITTER_H

/********************  HEADERS  *********************/
#include <vector>
#include "CMRGeometry.h"
#include "CMRCommon.h"

/*********************  TYPES  **********************/
typedef std::vector<CMRRect> CMRSpaceSubDomains;

/*********************  CLASS  **********************/
class CMRAbstractSpaceSplitter
{
	public:
		CMRAbstractSpaceSplitter(int x, int y,int width,int height,int nbSubDomains,int firstDim);
		CMRAbstractSpaceSplitter(CMRRect rect,int nbSubDomains,int firstDim);
		int getNbSubdomains(void) const;
		virtual void setNbSubdomains(int nbSubDomains);
		virtual CMRRect getLocalDomain(int localId) const = 0;
		virtual CMRSpaceSubDomains getAllSubDomains(void) const = 0;
		virtual int getNeighbour(int localId,int deltaX,int deltaY) const = 0;
		virtual CMRVect2D getSplittingSize( void ) const = 0;
		void printDebug(int printOnRank = CMR_MPI_ALL) const;
		const CMRRect & getDomain(void) const;
	protected:
		int firstDim;
		int nbSubDomains;
		CMRRect rect;
};

/*********************  CLASS  **********************/
class CMRBasicSpaceSplitter : public CMRAbstractSpaceSplitter
{
	public:
		CMRBasicSpaceSplitter ( int x, int y,int width, int height,int nbSubDomains, int firstDim );
		CMRBasicSpaceSplitter ( CMRRect rect, int nbSubDomains, int firstDim );
		virtual CMRSpaceSubDomains getAllSubDomains ( void ) const;
		virtual CMRRect getLocalDomain ( int localId ) const;
		virtual int getNeighbour ( int localId, int deltaX, int deltaY ) const;
		virtual CMRVect2D getSplittingSize ( void ) const;
	private:
		CMRVect2D getPosition(int localId, CMRVect2D & splitting) const;
};

#endif // CMR_SPACE_SPLITTER_H
