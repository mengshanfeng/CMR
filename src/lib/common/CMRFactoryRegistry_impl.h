/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_FACTORY_REGISTRY_IMPL_H
#define CMR_FACTORY_REGISTRY_IMPL_H

/********************  HEADERS  *********************/
#include <cassert>
#include "CMRDebug.h"
#include "CMRFactoryRegistry.h"
#include "CMRObjectFactory.h"

/*******************  FUNCTION  *********************/
inline CMRFactoryRegistry::CMRFactoryRegistry ( void )
{
}

/*******************  FUNCTION  *********************/
inline CMRFactoryRegistry::~CMRFactoryRegistry ( void )
{
	for (CMRFactoryMap::iterator it = map.begin() ; it != map.end() ; ++it)
		delete it->second;
	map.clear();
}

/*******************  FUNCTION  *********************/
inline bool CMRFactoryRegistry::hasObject(const std::string & name) const
{
	CMRFactoryMap::const_iterator it = map.find(name);
	return ( it != map.end() );
}

/*******************  FUNCTION  *********************/
inline void CMRFactoryRegistry::registerFactory ( const std::string& name, CMRObjectFactory* factory )
{
	//errors
	assert(name.empty() == false);
	assert(factory != NULL);
	
	//check if exist
	assume(hasObject(name) == false,"CMRFactoryRegistry already contain an object type named : %s",name.c_str());
	
	//add entry
	map[name] = factory;
}

/*******************  FUNCTION  *********************/
template <class T>
void CMRFactoryRegistry::registerGenericFactory ( const std::string& name )
{
	this->registerFactory(name,new CMRObjectFactoryGeneric<T>());
}

/*******************  FUNCTION  *********************/
template <class T>
T* CMRFactoryRegistry::createObject ( const std::string& name , CMRCmdOptions & options)
{
	//search object def
	CMRFactoryMap::iterator it = map.find(name);
	
	//check
	assume(it != map.end(),"Faild to find object '%s' in CMRFactoryRegistry.",name.c_str());
	assert(it->second != NULL);

	//create
	void * tmp = it->second->create(options);
	assume(tmp != NULL,"Factory of '%s' failed to allocate object, get NULL.",name.c_str());
	
	//cehck type
	T * res = (T*)tmp;
// 	T * res = dynamic_cast<T*>(tmp);
// 	assume(res != NULL,"Factory of %s provide an incompatible type rejected by dynamic_cast().",name.c_str());
	
	return res;
}

#endif //CMR_FACTORY_REGISTRY_IMPL_H
