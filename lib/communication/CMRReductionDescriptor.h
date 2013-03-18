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
#include <cassert>
#include <string>
#include "../common/CMRDebug.h"
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
		virtual void * getBufferIn(void) = 0;
		virtual void * getBufferOut(void) = 0;
		virtual size_t getBufferSize(void) const = 0;
		virtual CMRReductionType getType(void) const = 0;
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
		virtual void* getBufferIn ( void );
		virtual void* getBufferOut ( void );
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

/*******************  FUNCTION  *********************/
template <class T>
CMRGenericReductionDescriptor<T>::CMRGenericReductionDescriptor ( T* reduction )
{
	this->reduction = reduction;
}

/*******************  FUNCTION  *********************/
template <class T>
void CMRGenericReductionDescriptor<T>::doCustomReduce ( void )
{
	assert(false);
}

/*******************  FUNCTION  *********************/
template <class T>
void CMRGenericReductionDescriptor<T>::flush ( void )
{
	*reduction = tmpReduction;
}

/*******************  FUNCTION  *********************/
template <class T>
void* CMRGenericReductionDescriptor<T>::getBufferOut ( void )
{
	warning("Improve!");
	return &tmpReduction;
}

/*******************  FUNCTION  *********************/
template <class T>
void* CMRGenericReductionDescriptor<T>::getBufferIn ( void )
{
	warning("Improve!");
	return reduction;
}


/*******************  FUNCTION  *********************/
template <class T>
size_t CMRGenericReductionDescriptor<T>::getBufferSize ( void ) const
{
	warning("Improve!");
	return sizeof(tmpReduction);
}

/*******************  FUNCTION  *********************/
template <class T>
bool CMRGenericReductionDescriptor<T>::getCommute ( void )
{
	warning("Not implemented!");
	return true;
}

/*******************  FUNCTION  *********************/
template <class T>
CMRReductionOp CMRGenericReductionDescriptor<T>::getOperation ( void ) const
{
	warning("Improve!");
	return CMR_REDUCE_OP_SUM;
}

/*******************  FUNCTION  *********************/
template <class T>
size_t CMRGenericReductionDescriptor<T>::getSize ( void ) const
{
	warning("Improve!");
	return sizeof(tmpReduction) / sizeof(T);
}

/*******************  FUNCTION  *********************/
template <class T>
CMRReductionType CMRGenericReductionDescriptor<T>::getType ( void ) const
{
	warning("Improve!");
	return CMR_REDUCE_TYPE_FLOAT;
}

#endif // CMR_ABSTRACT_REDUCTION_H
