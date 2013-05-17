/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_TRANSFORMATION_H
#define CMR_TRANSFORMATION_H

/********************  HEADERS  *********************/
#include <string>

/*********************  TYPES  **********************/
class CMRProjectCodeEntry;

/*********************  CLASS  **********************/
class CMRTransformation
{
	public:
		CMRTransformation(const std::string & name);
		virtual ~CMRTransformation(void);
		virtual void run(CMRProjectCodeEntry & entry) = 0;
		const std::string & getName(void) const;
	private:
		std::string name;
};

#endif //CMR_TRANSFORMATION_H
