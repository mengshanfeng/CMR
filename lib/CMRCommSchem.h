/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_COMM_SCHEM_H
#define CMR_COMM_SCHEM_H

/********************  HEADERS  *********************/
#include <vector>

/*********************  CLASS  **********************/
class CMRCommunication;

/*********************  TYPES  **********************/
typedef std::vector<CMRCommunication * > CMRCommList;

/*********************  CLASS  **********************/
class CMRCommSchem
{
	public:
		virtual ~CMRCommSchem(void);
		void addComm(CMRCommunication * comm);
		void clear(void);
	private:
		//clone is forbidden
		CMRCommSchem(const CMRCommSchem & orig);
		CMRCommSchem & operator = (const CMRCommSchem & orig);
	private:
		CMRCommList comms;
};

#endif // CMRCOMMSCHEM_H
