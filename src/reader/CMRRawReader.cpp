/********************  HEADERS  *********************/
#include <cstring>
#include <cassert>
#include <unistd.h>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "outputer/FileHeader.h"
#include "CMRRawReader.h"

/**********************  USING  *********************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRReader
{

/*******************  FUNCTION  *********************/
CMRRawReader::CMRRawReader ( size_t entrySize )
{
	this->entrySize = entrySize;
}

/*******************  FUNCTION  *********************/
void CMRRawReader::registerEntryName ( std::string entryName, int entryId )
{
	this->entryDefs[entryName] = entryId;
}

/*******************  FUNCTION  *********************/
int CMRRawReader::run ( int argc, char** argv )
{
	//vars
	CMRReaderOutputFormat format = OUT_FORMAT_INFO;
	string filename;
	int frameId = -1;
	
	//arg error
	if (argc != 4)
	{
		fprintf(stderr,"Usage : %s {--gnuplot|--octave|--checksum|--info|--vtk} {file.raw} {frame_id}\n",argv[0]);
		abort();
	}

	//open
	filename = argv[2];
	frameId = atoi(argv[3]);

	//read args
	if (strcmp(argv[1],"--gnuplot") == 0)
		format = OUT_FORMAT_GNUPLOT;
	else if (strcmp(argv[1],"--checksum") == 0)
		format = OUT_FORMAT_CHECKSUM;
	else if (strcmp(argv[1],"--info") == 0)
		format = OUT_FORMAT_INFO;
	else if (strcmp(argv[1],"--vtk") == 0)
		format = OUT_FORMAT_VTK;
	else		
		fatal("Invalid format option.");

	//print
	return run(filename,format,frameId,0);

	return EXIT_SUCCESS;
}

/*******************  FUNCTION  *********************/
int CMRRawReader::run ( const std::string& filename, CMRReaderOutputFormat outputFormat,int frameId, int entryId )
{
	//errors
	assert(filename.empty() == false);

	//open file
	FILE * fp = fopen(filename.c_str(),"r");
	if (fp == NULL)
	{
		perror(filename.c_str());
		fatal("Fail to load file in CMRRawReader.");
	}

	int res = run(fp,outputFormat,frameId,entryId);

	fclose(fp);
	return res;
}

/*******************  FUNCTION  *********************/
int CMRRawReader::run ( FILE* fp, CMRReaderOutputFormat outputFormat, int frameId, int entryId )
{
	//locals
	LBMFileHeader header;
	
	//errors
	assert(frameId >= 0);
	assert(fp != NULL);
	
	//check header and read info
	readFileHeader(header,fp);

	//seek to frame
	if (seekToFrame(fp,header,frameId) == false)
		fatal("Can't seek to the requested frame.");
	
	//allocate buffer
	void * buffer = malloc(entrySize * header.mesh_max_height * header.mesh_max_width);

	//read
	if (readNextFrame(buffer,fp,header))
		printCurrentFrame(fp,header,buffer,outputFormat,frameId,entryId);
	
	//free buffer
	if (buffer != NULL)
		free(buffer);
	
	return EXIT_SUCCESS;
}

/*******************  FUNCTION  *********************/
void CMRRawReader::readFileHeader ( LBMFileHeader& header, FILE* fp )
{
	//errors
	assert(fp != NULL);
	
	//read header
	if( !fread(&header,sizeof(header),1,fp) )
	{
		if( ferror( fp ) )
		{
			perror( "fread" );
			abort();
		}

		if( feof( fp ) )
		{
			printf( "Error file is empty" );
			abort();
		}
	}

	//check magick
	if (header.magick != RESULT_MAGICK)
		fatal("Invalid file format.");
}

/*******************  FUNCTION  *********************/
bool CMRRawReader::seekToFrame ( FILE* fp,LBMFileHeader & header, int frameId )
{
	int res = fseek(fp,frameId * entrySize * header.mesh_max_height * header.mesh_max_width,SEEK_CUR);
	return (res == 0);
}

/*******************  FUNCTION  *********************/
bool CMRRawReader::readNextFrame ( void* buffer, FILE* fp, LBMFileHeader header )
{
	//vars
	size_t res;

	//errors
	assert(buffer != NULL);
	assert(fp != NULL);

	//load the frame
	res = fread(buffer,entrySize,header.mesh_max_height * header.mesh_max_width,fp);

	if (res != header.mesh_max_height * header.mesh_max_width)
	{
		if (feof(fp))
		{
			return false;
		} else {
			fatal("Error while reading the file.");
			return false;
		}
	} else {
		return true;
	}
}

/*******************  FUNCTION  *********************/
void CMRRawReader::printCurrentFrame ( FILE * fp,LBMFileHeader header, void* buffer, CMRReaderOutputFormat outputFormat, int frameId, int entryId )
{
	switch(outputFormat)
	{
		case OUT_FORMAT_GNUPLOT:
			printCurrentFrameGnuplot(buffer,header,entryId);
			break;
		case OUT_FORMAT_OCTAVE :
			printf("Not implemented \n");
			abort();
			break;
		case OUT_FORMAT_VTK:
			printCurrentFrameVtk(buffer,header,entryId);
			break;
		case OUT_FORMAT_INFO:
			printInfo(header,fp);
			break;
		case OUT_FORMAT_CHECKSUM :
			doChecksum(buffer,header,entryId);
			break;
	}
}

/*******************  FUNCTION  *********************/
void CMRRawReader::doChecksum ( void* buffer, LBMFileHeader header, int entryId )
{
	printf("TODO\n");
}

/*******************  FUNCTION  *********************/
void CMRRawReader::printInfo ( LBMFileHeader header, FILE * fp )
{
	printf("width=%d\n",header.mesh_width);
	printf("height=%d\n",header.mesh_height);
	printf("frames=%lu\n",getFrameCount(header,fp));
}

/*******************  FUNCTION  *********************/
size_t CMRRawReader::getFrameCount ( LBMFileHeader header, FILE* fp )
{
	struct stat info;
	if (fstat(fileno(fp), &info) == 0)
		return info.st_size / (header.mesh_max_height * header.mesh_max_width * entrySize);
	else
		return 0;
}

/*******************  FUNCTION  *********************/
void CMRRawReader::printCurrentFrameVtk ( void* buffer, LBMFileHeader header, int entryId )
{
	// Define your scalars DATA[x][y][z] and dimensions NX, NY and NZ here!
	uint32_t i,j,l;
	uint32_t line_height;
	int pos;

	 line_height = header.mesh_height / header.lines;

	//vtk header
	printf("# vtk DataFile Version 3.0\n");
	printf("vtkfile\n");
	printf("ASCII\n");
	printf("DATASET STRUCTURED_POINTS\n");
	printf("DIMENSIONS %d %d 1\n",header.mesh_width,line_height);
	printf("ORIGIN 0 0 0\n");
	printf("SPACING 1 1 1\n");
	printf("POINT_DATA %d\n",header.mesh_width * line_height);
	printf("SCALARS scalars float 1\n");
	printf("LOOKUP_TABLE default\n");
	
	//loop on datas
	for ( j = 0 ; j < line_height ; j++)
	{
		for ( i = 0 ; i < header.mesh_width ; i++)
		{
			for ( l = 0 ; l < header.lines ; l++)
			{
				//for ( j = 0 ; j < line_height ; j++)
				{
					stringstream stream;
					pos = line_height * i + j + l * line_height * header.mesh_width;
					//printf("%f %f\n",i,j+l * line_height,file->entries[pos].density,file->entries[pos].v);
					printValue(stream,(void*)(((char*)buffer)+entrySize * pos),entryId);
					printf("%s ",stream.str().c_str());
				}
			}
			printf("\n");
		}
	}
	printf("\n");
}

/*******************  FUNCTION  *********************/
void CMRRawReader::printCurrentFrameGnuplot ( void* buffer, LBMFileHeader header, int entryId )
{
	//vars
	uint32_t i,j,l;
	uint32_t line_height;
	int pos;

	//calc line_height
	line_height = header.mesh_height / header.lines;
	
	//loop on datas
	for ( i = 0 ; i < header.mesh_width ; i++)
	{
		for ( l = 0 ; l < header.lines ; l++)
		{
			for ( j = 0 ; j < line_height ; j++)
			{
				stringstream stream;
				pos = line_height * i + j + l * line_height * header.mesh_width;
				stream << i << "\t" << j+l * line_height;
				for (CMRRawReaderEntryMap::iterator it = entryDefs.begin() ; it != entryDefs.end() ; ++it)
				{
					stream << "\t";
					printValue(stream,(void*)(((char*)buffer)+entrySize * pos),it->second);
				}
				printf("%s\n",stream.str().c_str());
			}
		}
		printf("\n");
	}
	printf("\n");
}

/*******************  FUNCTION  *********************/
void CMRRawReader::fatal(const char * message)
{
	fprintf(stderr,"FATAL ERROR : %s\n",message);
	abort();
}

}
