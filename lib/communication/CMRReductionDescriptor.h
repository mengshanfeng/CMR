/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_ABSTRACT_REDUCTION_H
#define CMR_ABSTRACT_REDUCTION_H

/********************  HEADERS  *********************/
#include <string>
#include "CMRComm.h"

/********************  ENUM  ************************/
enum CMRReductionType
{
	CMR_REDUCE_TYPE_INT,
	CMR_REDUCE_TYPE_FLOAT,
	CMR_REDUCE_TYPE_CUSTOM
};

/********************  ENUM  ************************/
enum CMRReductionOp
{
	CMR_REDUCE_OP_SUM,
	CMR_REDUCE_OP_CUSTOM
};

/*********************  CLASS  **********************/
class CMRReductionDescriptor
{
	public:
		virtual ~CMRReductionDescriptor(void) {};
		virtual void * getBuffer(void) = 0;
		virtual size_t getBufferSize(void) const = 0;
		virtual CMRReductionType getTyep(void) const = 0;
		virtual CMRReductionOp getOperation(void) const = 0;
		virtual size_t getSize(void) const = 0;
		virtual void doCustomReduce(void) = 0;
		virtual void flush(void) = 0;
		virtual bool getCommute(void) = 0;
};

/*********************  CLASS  **********************/
template <class T>
class CMRGenericReductionDescriptor : public CMRReductionDescriptor
{
	public:
		CMRGenericReductionDescriptor ( T * reduction );
		virtual void doCustomReduce ( void );
		virtual void* getBuffer ( void );
		virtual size_t getBufferSize ( void ) const;
		virtual CMRReductionOp getOperation ( void ) const;
		virtual CMRReductionType getType ( void ) const;
		virtual size_t getSize ( void ) const;
		virtual void flush(void);
		virtual bool getCommute ( void );
	protected:
		T * reduction;
		T tmpReduction;
};

#endif // CMR_ABSTRACT_REDUCTION_H
