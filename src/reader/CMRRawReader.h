#ifndef CMR_RAW_READER
#define CMR_RAW_READER

/********************  HEADERS  *********************/
#include <cstdlib>
#include <string>
#include <cstdio>
#include <ostream>
#include <map>

/*********************  TYPES  **********************/
struct LBMFileHeader;

/********************  NAMESPACE  *******************/
namespace CMRReader
{

/*******************  ENUM  *********************/
enum CMRReaderOutputFormat
{
	OUT_FORMAT_GNUPLOT,
	OUT_FORMAT_OCTAVE,
	OUT_FORMAT_CHECKSUM,
	OUT_FORMAT_INFO,
	OUT_FORMAT_VTK
};

/*********************  TYPES  **********************/
typedef std::map<std::string,int> CMRRawReaderEntryMap;

/*********************  CLASS  **********************/
class CMRRawReader
{
	public:
		CMRRawReader(size_t entrySize);
		int run(int argc,char ** argv);
		int run( const std::string& filename, CMRReader::CMRReaderOutputFormat outputFormat, int frameId, int entryId );
		int run( FILE* fp, CMRReader::CMRReaderOutputFormat outputFormat, int frameId, int entryId );
	protected:
		void registerEntryName(std::string entryName,int entryId);
		virtual void printValue(std::ostream & out, const void * data,int entryId) = 0;
	private:
		bool seekToFrame( FILE* fp, LBMFileHeader& header, int frameId );
		void readFileHeader( LBMFileHeader& header, FILE* fp );
		void fatal(const char * message);
		bool readNextFrame ( void* buffer, FILE* fp, LBMFileHeader header );
		void printCurrentFrame ( FILE* fp, LBMFileHeader header, void* buffer, CMRReader::CMRReaderOutputFormat outputFormat, int frameId, int entryId );
		void printCurrentFrameGnuplot ( void* buffer, LBMFileHeader header, int entryId );
		void printCurrentFrameVtk ( void* buffer, LBMFileHeader header, int entryId );
		void printInfo ( LBMFileHeader header, FILE* fp );
		size_t getFrameCount ( LBMFileHeader header, FILE* fp );
		void doChecksum ( void* buffer, LBMFileHeader header, int entryId );
	private:
		CMRRawReaderEntryMap entryDefs;
		size_t entrySize;
};

};

#endif //CMR_RAW_READER