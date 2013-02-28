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
class CMRRect;

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

/********************  ENUM  ************************/
enum CMRAxisId
{
	CMR_AXIS_X = 0,
	CMR_AXIS_Y = 1
};

/*********************  CLASS  **********************/
class CMRAbstractDomain
{
	ASSIST_UNIT_TEST( TestAbstractDomain )
	public:
		CMRAbstractDomain(size_t typeSize,int width,int height,int ghostDepth,int originX,int originY,int globalWidth,int globalHeight);
		virtual ~CMRAbstractDomain(void);
		virtual bool isContiguousGhost(const CMRRect & rect) const = 0;
		virtual bool isContiguous(int directionID) const = 0;
		virtual size_t getGhostSize(const CMRRect & rect) const = 0;
		virtual int copyGhostToBuffer(void * buffer,size_t size,const CMRRect & rect) const = 0;
		virtual int copyGhostFromBuffer(const void * buffer,size_t size,const CMRRect & rect) = 0;
		virtual void * getContiguousGhost(const CMRRect & rect) = 0;
		virtual void * getCell(int x,int y) = 0;
		virtual void setCommunicator(int x,int y,CMRCommFactory * commFactory);
		virtual void fillWithUpdateComm(CMRCommSchem & commSchema,int x,int y,int requestedDepth,CMRCommType commType);
		size_t getTypeSize(void) const;
		int getDimensions(void) const;
		int getSize(int axis) const;
		int getOrigin(int axis) const;
		int getGhostDepth(void) const;
		CMRUpdateStatus getGhostStatus(int x,int y) const;
		void setGhostStatus(int x,int y,CMRUpdateStatus status);
		bool isFullyInDomain(const CMRRect & rect) const;
		bool isFullyInDomainMemory(const CMRRect & rect) const;
		CMRVect2D getAbsPos(int x = 0,int y = 0) const;
		CMRVect2D getGlobalSize(void) const;
		CMRRect getLocalRect(void) const;
		CMRRect getGlobalRect(void) const;
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
		/** Size along each dimensions. **/
		int sizes[2];
		/** Coordinate of the current domain on the global one. **/
		int origin[2];
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
