/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_ABSTRACT_DOMAIN_H
#define CMR_ABSTRACT_DOMAIN_H

/********************  HEADERS  *********************/
#include <stdlib.h>

/*********************  CLASS  **********************/
class CMRCommunicator;
class CMRCommSchem;

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
		CMRAbstractDomain(int typeSize,int width,int height,int ghostDepth,int originX,int originY);
		virtual ~CMRAbstractDomain(void);
		virtual bool isContiguousGhost(int axis,CMROrientation orientation,int ghostDepth) const = 0;
		virtual size_t getGhostSize(int axis,CMROrientation orientation,int ghostDepth) const = 0;
		virtual int copyGhostToBuffer(void * buffer,size_t size,int axis,CMROrientation orientation,int ghostDepth) const = 0;
		virtual int copyGhostFromBuffer(const void * buffer,size_t size,int axis,CMROrientation orientation,int ghostDepth) = 0;
		virtual void setCommunicator(int x,int y,CMRCommunicator * communicator);
		virtual void fillWithUpdateComm(CMRCommSchem & commSchema,int x,int y) const;
		int getTypeSize(void) const;
		int getDimensions(void) const;
		int getSize(int axis) const;
		int getOrigin(int axis) const;
		int getGhostDepth(void) const;
		CMRUpdateStatus getGhostStatus(int x,int y) const;
		void setGhostStatus(int x,int y,CMRUpdateStatus status);
	private:
		/** Size of the type used to describe each cells of the mesh. **/
		int typeSize;
		/** Dimensions (only 2 is supported up to now). **/
		int dimensions;
		/** Size along each dimensions. **/
		int sizes[2];
		/** Coordinate of the current domain on the global one. **/
		int origin[2];
		/** Number of ghost cells to add arround each dimensions. **/
		int ghostDepth;
		/** Current update status of each ghost cells. **/
		CMRUpdateStatus ghostStaus[3][3];
		/** Communicator to sync the ghost cells. **/
		CMRCommunicator * communicators[3][3];
};

#endif //CMR_ABSTRACT_DOMAIN_H
