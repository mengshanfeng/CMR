/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 02/2013
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_MPI_DOMAIN_BUILDER_H
#define CMR_MPI_DOMAIN_BUILDER_H

/********************  HEADERS  *********************/
#include <string>
#include "CMRDomainBuilder.h"

/*********************  TYPES  **********************/
class CMRAbstractSpaceSplitter;

/*********************  CLASS  **********************/
/** @TODO -> Rename into XXXFactory **/
class CMRMPIDomainBuilder : public CMRDomainBuilder
{
	public:
		CMRMPIDomainBuilder(CMRAbstractSpaceSplitter * splitter);
		virtual CMRDomainStorage * buildDomain(const CMRVariable & variable);
	private:
		int hashName(const std::string & name) const;
		CMRAbstractSpaceSplitter * splitter;
};

#endif // CMR_MPI_DOMAIN_BUILDER_H
