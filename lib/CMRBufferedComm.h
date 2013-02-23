/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_BUFFERED_COMM_H
#define CMR_BUFFERED_COMM_H

/********************  HEADERS  *********************/
#include "CMRGeometry.h"
#include "CMRCommon.h"
#include "CMRComm.h"


/*********************  CLASS  **********************/
class CMRAbstractDomain;

/*********************  CLASS  **********************/
class CMRBufferedComm : public CMRComm
{
	public:
		CMRBufferedComm(CMRAbstractDomain* domain, const CMRRect2D& rect,CMRCommType commType);
		virtual ~CMRBufferedComm ( void );
		virtual void run ( void );
	protected:
		virtual void runSend(void * buffer,size_t size) = 0;
		virtual void runReceive(void * buffer,size_t size) = 0;
		virtual void * allocateBuffer(size_t size);
		virtual void freeBuffer(void * buffer,size_t size);
		CMRCommType getCommType(void) const;
		size_t getBufferSize(void) const;
	private:
		void setupBuffer(void);
	private:
		CMRRect2D rect;
		void * buffer;
		size_t bufferSize;
		CMRCommType commType;
		CMRAbstractDomain* domain;
		CMRCommStatus commStatus;
		bool isTempBuffer;
};

#endif // CMRBUFFEREDCOMM_H
