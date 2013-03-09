/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 02/2013
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_DOMAIN_BUILDER_H
#define CMR_DOMAIN_BUILDER_H

/*********************  TYPES  **********************/
class CMRDomainStorage;
struct CMRVariable;

/*********************  CLASS  **********************/
/** @TODO -> Rename into XXXFactory **/
class CMRDomainBuilder
{
	public:
		virtual ~CMRDomainBuilder(void) {};
		virtual CMRDomainStorage * buildDomain(const CMRVariable & variable) = 0;
};

#endif // CMR_DOMAIN_BUILDER_H
