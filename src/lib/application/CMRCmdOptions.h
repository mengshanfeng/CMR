/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_CMD_OPTIONS_H
#define CMR_CMD_OPTIONS_H

/********************  HEADERS  *********************/
#include "svutArgp.h"

/*********************  TYPES  **********************/
typedef struct _dictionary_ dictionary;

/*********************  CLASS  **********************/
class CMRCmdOptions : public svUnitTest::svutArgp
{
	public:
		CMRCmdOptions(int width = 800, int height = 100, int iterations = 1000);
		virtual ~CMRCmdOptions ( void );
		virtual void parseInit ( void )  throw (svUnitTest::svutExArgpError);
		virtual void parseOption ( char key, std::string arg, std::string value )  throw (svUnitTest::svutExArgpError);
		virtual void parseTerminate ( void )  throw (svUnitTest::svutExArgpError);
		const std::string & getConfigFile(void) const;
		int getWidth(void) const;
		int getHeight(void) const;
		int getIterations(void) const;
		int getConfigInteger(const std::string & key,int defaultValue = 0);
		bool getConfigBoolean(const std::string & key,bool defaultValue = false);
	protected:
		void loadConfigFile(const std::string & filename);
	protected:
		std::string configFile;
		int width;
		int height;
		int iterations;
		dictionary * iniDic;
};

#endif //CMR_CMD_OPTIONS_H
