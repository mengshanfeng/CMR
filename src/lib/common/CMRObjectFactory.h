/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_OBJECT_FACTORY_H
#define CMR_OBJECT_FACTORY_H

/*********************  TYPES  **********************/
class CMRCmdOptions;

/*********************  CLASS  **********************/
class CMRObjectFactory
{
	public:
		virtual ~CMRObjectFactory(void) {};
		virtual void * create(CMRCmdOptions & options) = 0;
};

/*********************  CLASS  **********************/
template <class T>
class CMRObjectFactoryGeneric : public CMRObjectFactory
{
	public:
		virtual void * create(CMRCmdOptions & options) { return new T(options);};
};

#endif //CMR_OBJECT_FACTORY_H