/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_FACTORY_REGISTRY_H
#define CMR_FACTORY_REGISTRY_H

/********************  HEADERS  *********************/
#include <map>
#include <string>

/*********************  TYPES  **********************/
class CMRObjectFactory;
class CMRCmdOptions;

/*********************  TYPES  **********************/
typedef std::map<std::string,CMRObjectFactory*> CMRFactoryMap;

/*********************  CLASS  **********************/
class CMRFactoryRegistry
{
	public:
		CMRFactoryRegistry(void);
		~CMRFactoryRegistry(void);
		void registerFactory(const std::string & name,CMRObjectFactory * factory);
		template <class T> void registerGenericFactory(const std::string &name);
		template <class T> T * createObject(const std::string & name,CMRCmdOptions & options);
		bool hasObject(const std::string & name) const;
	private:
		CMRFactoryMap map;
};

/********************  HEADERS  *********************/
#include "CMRFactoryRegistry_impl.h"

#endif //CMR_FACTORY_REGISTRY_H
