/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "CMRTransformation.h"
#include "../definitions/CMRProjectCode.h"

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
CMRTransformation::CMRTransformation(const std::string& name)
{
	this->name = name;
}

/*******************  FUNCTION  *********************/
CMRTransformation::~CMRTransformation(void)
{
}

/*******************  FUNCTION  *********************/
const std::string& CMRTransformation::getName(void ) const
{
	return name;
}

}
