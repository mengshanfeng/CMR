/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 02/2013
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_ABSTRACT_TYPE_H
#define CMR_ABSTRACT_TYPE_H

/********************  HEADERS  *********************/
#include <typeinfo>

/*********************  TYPES  **********************/
typedef void * CMRTypeId;

/*********************  CLASS  **********************/
class CMRAbstractType
{
	public:
		virtual ~CMRAbstractType(void) {};
		virtual const std::string getName(void) const = 0;
		virtual size_t getSize(void) const = 0;
		virtual CMRTypeId getTypeId(void) const = 0;
		bool operator==(const CMRAbstractType & type) const { return getTypeId() == type.getTypeId();};
};

/*********************  CLASS  **********************/
//Maybe move it as singleton
template <class T>
class CMRGenericType : public CMRAbstractType
{
	public:
		virtual const std::string getName ( void ) const { return typeid(T).name();};
		virtual size_t getSize ( void ) const {return sizeof(T);};
		virtual CMRTypeId getTypeId ( void ) const {return (void*)(this->typeId);};
	private:
		static void typeId() {};
};

#endif //CMR_ABSTRACT_TYPE_H
