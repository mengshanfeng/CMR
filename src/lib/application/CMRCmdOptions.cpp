/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cstdlib>
#include <cassert>
#include <cstdio>
#include <sstream>
#include <iniparser.h>
#include "common/CMRCommon.h"
#include "common/CMRDebug.h"
#include "CMRCmdOptions.h"

/*******************  FUNCTION  *********************/
CMRCmdOptions::CMRCmdOptions ( int width, int height, int iterations )
{
	//default values
	this->iniDic = NULL;
	this->width = width;
	this->height = height;
	this->iterations = iterations;
	this->dumpUsedDic = NULL;
	
	//setup arg parsor
	setProjectName      ("CartesinMeshRuntime");
	setProjectVersion   ("0.0.0");
	setProjectBugAddress("sebastien.valat.dev@orange.fr");
	setProjectDescr     ("Runtime to quicly build parallel (MPI/Threads) stencil numerical simulations.");
	setProjectArgUsage  ("[-w WIDTH] [-h HEIGHT] [-i ITERATIONS] [-c FILE] [-?]");
	
	//declare options
	declareOption('w', "width"     , "WIDTH"     , "Define the mesh width.");
	declareOption('h', "height"    , "HEIGHT"    , "Define the mesh height.");
	declareOption('i', "iterations", "ITERATIONS", "Define the number of iterations to compute.");
	declareOption('c', "config"    , "FILE"      , "Define the configuration file to load.");
	declareOption('d', "dumpconfig", "FILE"      , "Dump the config in FILE at execution end.");
}

/*******************  FUNCTION  *********************/
CMRCmdOptions::~CMRCmdOptions ( void )
{
	//remove the old config file and load the new one
	if (iniDic != NULL)
	{
		iniparser_freedict(iniDic);
		iniDic = NULL;
	}
	if (dumpUsedDic != NULL)
	{
		iniparser_freedict(dumpUsedDic);
		dumpUsedDic = NULL;
	}
}

/*******************  FUNCTION  *********************/
void CMRCmdOptions::parseInit ( void ) throw (svUnitTest::svutExArgpError) 
{
}

/*******************  FUNCTION  *********************/
void CMRCmdOptions::parseOption ( char key, std::string arg, std::string value )  throw (svUnitTest::svutExArgpError)
{ 
	std::stringstream err;
	switch(key)
	{
		case 'w':
			this->width = atoi(value.c_str());
			if (width <= 0)
			{
				err << "Invalid width on -w/--width : " << value << std::endl;
				throw svUnitTest::svutExArgpError(err.str());
			}
			break;
		case 'h':
			this->height = atoi(value.c_str());
			if (width <= 0)
			{
				err << "Invalid height on -h/--height : " << value << std::endl;
				throw svUnitTest::svutExArgpError(err.str());
			}
			break;
		case 'i':
			this->iterations = atoi(value.c_str());
			if (width <= 0)
			{
				err << "Invalid number of iterations on -i/--iterations : " << value << std::endl;
				throw svUnitTest::svutExArgpError(err.str());
			}
			break;
		case 'c':
			this->configFile = value;
			loadConfigFile(configFile);
			break;
		case 'd':
			dumpUsedDic = dictionary_new(5);
			this->dumpConfigFile = value;
			break;
		default:
			err << "Unknown argument : " << arg << std::endl;
			throw svUnitTest::svutExArgpError(err.str());
			break;
	}
}

/*******************  FUNCTION  *********************/
void CMRCmdOptions::parseTerminate ( void )  throw (svUnitTest::svutExArgpError)
{

}

/*******************  FUNCTION  *********************/
const std::string& CMRCmdOptions::getConfigFile ( void ) const
{
	return configFile;
}

/*******************  FUNCTION  *********************/
int CMRCmdOptions::getHeight ( void ) const
{
	return height;
}

/*******************  FUNCTION  *********************/
int CMRCmdOptions::getWidth ( void ) const
{
	return width;
}

/*******************  FUNCTION  *********************/
int CMRCmdOptions::getIterations ( void ) const
{
	return iterations;
}

/*******************  FUNCTION  *********************/
void CMRCmdOptions::loadConfigFile ( const std::string& filename )
{
	//errors
	assert(filename.empty() == false);
	
	//remove the old config file and load the new one
	if (iniDic != NULL)
		iniparser_freedict(iniDic);
	
	//load
	iniDic = iniparser_load(filename.c_str());
	if (iniDic == NULL)
	{
		std::stringstream err;
		err << "Failed to load config file " << filename << std::endl;
		throw svUnitTest::svutExArgpError(err.str());
	}
	
	//extract local values
	overrideByConfigFile();
}

/*******************  FUNCTION  *********************/
void CMRCmdOptions::overrideByConfigFile ( void )
{
	assert(iniDic != NULL);
	this->width = getConfigInteger("mesh:width",width);
	this->height = getConfigInteger("mesh:height",height);
	this->iterations = getConfigInteger("mesh:iterations",iterations);
}

/*******************  FUNCTION  *********************/
int CMRCmdOptions::getConfigInteger ( const std::string& key, int defaultValue )
{
	if (dumpUsedDic != NULL)
	{
		char buffer[64];
		sprintf(buffer,"%d",defaultValue);
		setupDumpEntry(key.c_str(),buffer);
	}
	return iniparser_getint(iniDic,key.c_str(),defaultValue);
}

/*******************  FUNCTION  *********************/
bool CMRCmdOptions::getConfigBoolean ( const std::string& key, bool defaultValue )
{
	if (dumpUsedDic != NULL)
		setupDumpEntry(key.c_str(),defaultValue?"true":"false");
	return iniparser_getboolean(iniDic,key.c_str(),defaultValue);
}

/*******************  FUNCTION  *********************/
void CMRCmdOptions::setupDumpEntry ( const std::string& key, const char* value )
{
	iniparser_set(dumpUsedDic,extractSectionName(key).c_str(),NULL);
	iniparser_set(dumpUsedDic,key.c_str(),value);
}

/*******************  FUNCTION  *********************/
std::string CMRCmdOptions::extractSectionName ( const std::string& key )
{
	std::string tmp;
	int i = 0;
	while (key[i] != ':' && key[i] != '\0')
		tmp += key[i++];
	return tmp;
}

/*******************  FUNCTION  *********************/
void CMRCmdOptions::dumpUsedConfigFile ( void )
{
	//trivial
	if (dumpUsedDic == NULL || dumpConfigFile.empty())
		return;

	//dump only on master
	if (cmrIsMPIMaster())
	{
		info("Dump used config file in '%s'.",dumpConfigFile.c_str());
		overrideByConfigFile();
		if (dumpConfigFile == "-")
		{
			iniparser_dump_ini(dumpUsedDic,stdout);
		} else {
			FILE * fp = fopen(dumpConfigFile.c_str(),"w");
			assume(fp != NULL,"Fail to open file : '%s'",dumpConfigFile.c_str());
			iniparser_dump_ini(dumpUsedDic,fp);
			fclose(fp);
		}
	}
}
