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
#include "CMRCommon.h"
#include "CMRGeometry.h"

/*********************  CLASS  **********************/
class CMRCommunicator;
class CMRCommSchem;
class CMRRect2D;

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
	public:
		CMRAbstractDomain(size_t typeSize,int width,int height,int ghostDepth,int originX,int originY);
		virtual ~CMRAbstractDomain(void);
		virtual bool isContiguousGhost(const CMRRect2D & rect) const = 0;
		virtual size_t getGhostSize(const CMRRect2D & rect) const = 0;
		virtual int copyGhostToBuffer(void * buffer,size_t size,const CMRRect2D & rect) const = 0;
		virtual int copyGhostFromBuffer(const void * buffer,size_t size,const CMRRect2D & rect) = 0;
		virtual void * getContiguousGhost(const CMRRect2D & rect) = 0;
		virtual void setCommunicator(int x,int y,CMRCommunicator * commFactory);
		virtual void fillWithUpdateComm(CMRCommSchem & commSchema,int x,int y,int requestedDepth,CMRCommType commType);
		size_t getTypeSize(void) const;
		int getDimensions(void) const;
		int getSize(int axis) const;
		int getOrigin(int axis) const;
		int getGhostDepth(void) const;
		CMRUpdateStatus getGhostStatus(int x,int y) const;
		void setGhostStatus(int x,int y,CMRUpdateStatus status);
	private:
		//copy is forbidden so ensure compile error by making related function private
		CMRAbstractDomain(const CMRAbstractDomain & orig);
		CMRAbstractDomain & operator = (const CMRAbstractDomain & orig);
		CMRRect2D computeGhostCommRect(int x,int y,int requestedDepth,CMRCommType commType) const;
	private:
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
		CMRCommunicator * commFactories[3][3];
};

#endif //CMR_ABSTRACT_DOMAIN_H
