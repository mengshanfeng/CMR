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
#include "FileHeader.h"
#include "common/CMRDebug.h"
#include "common/CMRSpaceSplitter.h"
#include "CMRBasicOutputer.h"
#include <domain/CMRVarSystem.h>

/*******************  FUNCTION  *********************/
CMRBasicOutputer::CMRBasicOutputer(const std::string & fname,const CMRAbstractSpaceSplitter & splitter)
{
	this->fname = fname;

	//open result file
	fp = fopen(fname.c_str(),"w");

	//errors
	if (fp == NULL)
	{
		perror(fname.c_str());
		abort();
	}

	//write header
	writeFileHeader(splitter);
}

/*******************  FUNCTION  *********************/
CMRBasicOutputer::~CMRBasicOutputer ( void )
{
	if (fp != NULL)
		fclose(fp);
}

/*******************  FUNCTION  *********************/
void CMRBasicOutputer::writeFileHeader ( const CMRAbstractSpaceSplitter & splitter )
{
	//setup header values
	LBMFileHeader header;
	header.magick      = RESULT_MAGICK;
	header.mesh_height = splitter.getDomain().height;
	header.mesh_width  = splitter.getDomain().width;
	header.mesh_max_height = splitter.getDomain().height;
	header.mesh_max_width  = splitter.getDomain().width;
	header.lines       = splitter.getSplittingSize().y;

	//write file
	fwrite(&header,sizeof(header),1,fp);
}

/*******************  FUNCTION  *********************/
void CMRBasicOutputer::write ( CMRVarSystem* system, const CMRRect& global, const CMRRect& local )
{
	assert(fp != NULL);
	assert(system->getDomain(2,CMR_PREV_STEP)->isContiguousGhost(local));
	size_t size = system->getDomain(2,CMR_PREV_STEP)->getGhostSize(local);
	void * buffer = system->getDomain(2,CMR_PREV_STEP)->getContiguousGhost(local);
	assert(buffer != NULL);
	debug("Write size = %lu",size);
	fwrite(buffer,1,size,fp);
}
