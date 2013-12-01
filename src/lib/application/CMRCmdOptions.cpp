/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cstdlib>
#include <sstream>
#include "CMRCmdOptions.h"

/*******************  FUNCTION  *********************/
CMRCmdOptions::CMRCmdOptions ( int width, int height, int iterations )
{
	//default values
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
void CMRCmdOptions::parseInit ( void ) throw (svUnitTest::svutExArgpError) 
{
	this->width = 800;
	this->height = 600;
	this->iterations = 8000;
}

/*******************  FUNCTION  *********************/
void CMRCmdOptions::parseOption ( char key, std::string arg, std::string value )  throw (svUnitTest::svutExArgpError)
{ 
	std::stringstream err;
	switch(key)
	{
		case 'w':
			this->width = atoi(value.c_str());
			err << "Invalid width on -w/--width : " << value << std::endl;
			  throw svUnitTest::svutExArgpError(err.str());
			break;
		case 'h':
			this->height = atoi(value.c_str());
			err << "Invalid height on -h/--height : " << value << std::endl;
			  throw svUnitTest::svutExArgpError(err.str());
			break;
		case 'i':
			this->iterations = atoi(value.c_str());
			err << "Invalid number of iterations on -i/--iterations : " << value << std::endl;
			  throw svUnitTest::svutExArgpError(err.str());
			break;
		case 'c':
			this->configFile = value;
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


