/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_ABSTRACT_DOMAIN_H
#define CMR_ABSTRACT_DOMAIN_H

/********************  HEADERS  *********************/
#include <stdlib.h>
#include <stddef.h>
#include <vector>
#include <iostream>
#include "../common/CMRCommon.h"
#include "../common/CMRGeometry.h"

/*********************  CLASS  **********************/
class CMRCommFactory;
class CMRCommSchem;
struct CMRRect;

/********************  ENUM  ************************/
enum CMRUpdateStatus
{
	CMR_UPDATE_STATUS_UPDATED,
	CMR_UPDATE_STATUS_IN_UPDATE,
	CMR_UPDATE_STATUS_CHANGED
};

/********************  ENUM  ************************/
enum CMROrientation
{
	CMR_ORIENTATION_POSITIV = 1,
	CMR_ORIENTATION_NEGATIV = -1
};

/*********************  CLASS  **********************/
class CMRAbstractDomain
{
	ASSIST_UNIT_TEST( TestAbstractDomain )
	public:
		CMRAbstractDomain(size_t typeSize,const CMRRect & localDomain,int ghostDepth,int globalWidth = -1,int globalHeight = -1);
		virtual ~CMRAbstractDomain(void);
		virtual bool isContiguousGhost(const CMRRect & rect) const = 0;
		virtual size_t getGhostSize(const CMRRect & rect) const = 0;
		virtual int copyGhostToBuffer(void * buffer,size_t size,const CMRRect & rect) const = 0;
		virtual int copyGhostFromBuffer(const void * buffer,size_t size,const CMRRect & rect) = 0;
		virtual void * getContiguousGhost(const CMRRect & rect) = 0;
		virtual void * getCell(int x,int y) = 0;
		virtual void setCommunicator(int x,int y,CMRCommFactory * commFactory);
		virtual void fillWithUpdateComm(CMRCommSchem & commSchema,int x,int y,int requestedDepth,CMRCommType commType);
		size_t getTypeSize(void) const;
		int getDimensions(void) const;
		const CMRRect & getLocalDomainRect(void) const;
		const CMRRect & getGlobalRect(void) const;
		const CMRRect & getMemoryRect(void) const;
		int getGhostDepth(void) const;
		CMRUpdateStatus getGhostStatus(int x,int y) const;
		void setGhostStatus(int x,int y,CMRUpdateStatus status);
		CMRVect2D getAbsPos(int x = 0,int y = 0) const;
		CMRVect2D getAbsPos(const CMRVect2D & vect) const;
		bool isFullyInLocalDomain(const CMRRect & rect) const;
		bool isFullyInDomainMemory(const CMRRect & rect) const;
	private:
		CMRRect computeGhostCommRect(int x,int y,int requestedDepth,CMRCommType commType) const;
		//copy is forbidden so ensure compile error by making related function private
		CMRAbstractDomain(const CMRAbstractDomain & orig);
		CMRAbstractDomain & operator = (const CMRAbstractDomain & orig);
	protected:
		/** Size of the type used to describe each cells of the mesh. **/
		size_t typeSize;
		/** Dimensions (only 2 is supported up to now). **/
		int dimensions;
		/** Size of the domain. **/
		CMRRect localRect;
		CMRRect memoryRect;
		/** Number of ghost cells to add arround each dimensions. **/
		int ghostDepth;
		/** Current update status of each ghost cells. **/
		CMRUpdateStatus ghostStatus[3][3];
		/** Communicator to sync the ghost cells. **/
		CMRCommFactory * commFactories[3][3];
		/** Size of global mesh. **/
		CMRRect globalRect;
};

#endif //CMR_ABSTRACT_DOMAIN_H
