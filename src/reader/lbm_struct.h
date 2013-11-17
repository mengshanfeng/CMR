#ifndef LBM_STRUCT_H
#define LBM_STRUCT_H

/********************  HEADERS  *********************/
#include <stdint.h>
#include <stdio.h>
//#include "lbm_config.h"

/********************  MACRO  ***********************/
/* Some configs */
#define DIMENSIONS 2
#define DIRECTIONS 9
#define RESULT_MAGICK 42

/********************** TYPEDEF *********************/
/**
 * Une cellule est un tableau de DIRECTIONS doubles pour stoquer les
 * probabilités microscopiques (f_i).
**/
typedef double * lbm_mesh_cell_t;
/** Représentation d'un vecteur pour la manipulation des vitesses macroscopiques. **/
typedef double Vector[DIMENSIONS];

/********************  STRUCT  **********************/
/**
 * Definit un maillage pour le domaine local. Ce maillage contient une bordure de une cellule
 * contenant les mailles fantômes.
**/
typedef struct Mesh
{
	/** Cellules du maillages (MESH_WIDTH * MESH_HEIGHT). **/
	lbm_mesh_cell_t cells;
	/** Largeur du maillage local (mailles fantome comprises). **/
	int width;
	/** Largeur du maillage local (mailles fantome comprises). **/
	int height;
	/** Largeur du maillage local (mailles fantome comprises + extra pour avoir une représentation unique sur tous les noeuds). **/
	int max_height;
} Mesh;

/*********************  ENUM  ***********************/
/**
 * Definition des différents type de cellule pour savoir quel traitement y appliquer
 * lors du calcule.
**/
typedef enum lbm_cell_type_e
{
	/** Cellule de fluide standard, uniquement application des collisions. **/
	CELL_FUILD,
	/** Cellules de l'obstacle ou des bordure supérieures et inférieurs. Application de réflexion. **/
	CELL_BOUNCE_BACK,
	/** Cellule de la paroie d'entrée. Application de Zou/He avec V fixé. **/
	CELL_LEFT_IN,
	/** Cellule de la paroie de sortie. Application de Zou/He avec gradiant de densité constant. **/
	CELL_RIGHT_OUT
} lbm_cell_type_t;

/********************  STRUCT  **********************/
/**
 * Tableau maitnenant les informations de type pour les cellules.
**/
typedef struct lbm_mesh_type_s
{
	/** Type des cellules du maillages (MESH_WIDTH * MESH_HEIGHT). **/
	lbm_cell_type_t * types;
	/** Largeur du maillage local (mailles fantome comprises). **/
	int width;
	/** Largeur du maillage local (mailles fantome comprises). **/
	int height;
} lbm_mesh_type_t;

/********************  STRUCT  **********************/
/** Structure des en-tête utilisé dans le fichier de sortie. **/
typedef struct lbm_file_header_s
{
	/** Pour validation du format du fichier. **/
	uint32_t magick;
	/** Taille totale du maillage simulé (hors mailles fantômes). **/
	uint32_t mesh_width;
	/** Taille totale du maillage simulé (hors mailles fantômes). **/
	uint32_t mesh_height;
	/** Taille totale du maillage sauvé (hors mailles fantômes). **/
	uint32_t mesh_max_width;
	/** Taille totale du maillage sauvé (hors mailles fantômes). **/
	uint32_t mesh_max_height;
	/** Number of vertical lines. **/
	uint32_t lines;
} lbm_file_header_t;

/********************  STRUCT  **********************/
/** Une entrée du fichier, avec les deux grandeurs macroscopiques. **/
typedef struct lbm_file_entry_s
{
	float v;
	float density;
} lbm_file_entry_t;

/********************  STRUCT  **********************/
/** Pour la lecture du fichier de sortie. **/
typedef struct lbm_data_file_s
{
	FILE * fp;
	lbm_file_header_t header;
	lbm_file_entry_t * entries;
} lbm_data_file_t;

 struct lbm_comm_t_s;

/*******************  FUNCTION  *********************/
void Mesh_init( Mesh * mesh, struct lbm_comm_t_s * mesh_comm );
void Mesh_release( Mesh * mesh );

/*******************  FUNCTION  *********************/
void lbm_mesh_type_t_init( lbm_mesh_type_t * mesh, struct lbm_comm_t_s * mesh_comm );
void lbm_mesh_type_t_release( lbm_mesh_type_t * mesh );

/*******************  FUNCTION  *********************/
void save_frame(FILE * fp,const Mesh * mesh);

/*******************  FUNCTION  *********************/
void fatal(const char * message);

/*******************  FUNCTION  *********************/
/**
 * Fonction à utiliser pour récupérer une cellule du maillage en fonction de ses coordonnées.
**/
static inline lbm_mesh_cell_t Mesh_get_cell( const Mesh *mesh, int x, int y)
{
	return &mesh->cells[ (x * mesh->max_height + y) * DIRECTIONS ];
}

/*******************  FUNCTION  *********************/
/**
 * Fonction à utiliser pour récupérer une colonne (suivant y, x fixé) du maillage en fonction de ses coordonnées.
**/
static inline lbm_mesh_cell_t Mesh_get_col( const Mesh * mesh, int x )
{
	//+DIRECTIONS to skip the first (ghost) line
	return &mesh->cells[ x * mesh->max_height * DIRECTIONS + DIRECTIONS];
}

/*******************  FUNCTION  *********************/
/**
 * Fonction à utiliser pour récupérer un pointeur sur le type d'une cellule du maillage en fonction de ses coordonnées.
**/
static inline lbm_cell_type_t * lbm_cell_type_t_get_cell( const lbm_mesh_type_t * meshtype, int x, int y)
{
	return &meshtype->types[ x * meshtype->height + y];
}

#endif //LBM_STRUCT_H
