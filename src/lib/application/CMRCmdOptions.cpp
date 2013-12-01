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
#include <sstream>
#include <iniparser.h>
#include "CMRCmdOptions.h"

/*******************  FUNCTION  *********************/
CMRCmdOptions::CMRCmdOptions ( int width, int height, int iterations )
{
	//default values
	this->iniDic = NULL;
	this->width = width;
	this->height = height;
	this->iterations = iterations;
	
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
	this->width = getConfigInteger("mesh:width",width);
	this->height = getConfigInteger("mesh:height",height);
	this->iterations = getConfigInteger("mesh:iterations",iterations);
}

/*******************  FUNCTION  *********************/
int CMRCmdOptions::getConfigInteger ( const std::string& key, int defaultValue )
{
	return iniparser_getint(iniDic,key.c_str(),defaultValue);
}

/*******************  FUNCTION  *********************/
bool CMRCmdOptions::getConfigBoolean ( const std::string& key, bool defaultValue )
{
	return iniparser_getboolean(iniDic,key.c_str(),defaultValue);
}
