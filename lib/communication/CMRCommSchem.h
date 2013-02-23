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
#include <string>

/*********************  CLASS  **********************/
class CMRComm;

/*********************  TYPES  **********************/
typedef std::vector<CMRComm * > CMRCommList;

/*********************  CLASS  **********************/
class CMRCommSchem
{
	public:
		CMRCommSchem(const std::string & name = "Unknwon");
		virtual ~CMRCommSchem(void);
		void addComm(CMRComm * comm);
		void clear(void);
		int count(void) const;
		void run(void);
		CMRComm * getComm(int id);
		std::string getName(void) const;
		void printDebug(void) const;
	private:
		//clone is forbidden
		CMRCommSchem(const CMRCommSchem & orig);
		CMRCommSchem & operator = (const CMRCommSchem & orig);
	private:
		CMRCommList comms;
		std::string name;
};

#endif // CMRCOMMSCHEM_H
