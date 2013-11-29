#ifndef LBM_STRUCTS_H
#define LBM_STRUCTS_H

/*********************  ENUM  ***********************/
/**
 * Definition des différents type de cellule pour savoir quel traitement y appliquer
 * lors du calcule.
**/
enum LBMCellType
{
	/** Cellule de fluide standard, uniquement application des collisions. **/
	CELL_FLUID,
	/** Cellules de l'obstacle ou des bordure supérieures et inférieurs. Application de réflexion. **/
	CELL_BOUNCE_BACK,
	/** Cellule de la paroie d'entrée. Application de Zou/He avec V fixé. **/
	CELL_LEFT_IN,
	/** Cellule de la paroie de sortie. Application de Zou/He avec gradiant de densité constant. **/
	CELL_RIGHT_OUT
};

#endif //LBM_STRUCTS_H

