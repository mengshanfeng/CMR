#ifndef FILE_HEADER_H
#define FILE_HEADER_H

#include <stdint.h>

#define RESULT_MAGICK 0x12345

/********************  STRUCT  **********************/
/** Structure des en-tête utilisé dans le fichier de sortie. **/
struct LBMFileHeader
{
	/** Pour validation du format du fichier. **/
	uint32_t magick;
	/** Taille totale du maillage simulé (hors mailles fantômes). **/
	uint32_t mesh_width;
	/** Taille totale du maillage simulé (hors mailles fantômes). **/
	uint32_t mesh_height;
	/** Taille totale du maillage simulé (hors mailles fantômes). **/
	uint32_t mesh_max_width;
	/** Taille totale du maillage simulé (hors mailles fantômes). **/
	uint32_t mesh_max_height;
	/** Number of vertical lines. **/
	uint32_t lines;
};

#endif //FILE_HEADER_H
