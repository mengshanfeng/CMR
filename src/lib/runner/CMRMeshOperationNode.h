/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_MESH_OPERATION_NODE_H
#define CMR_MESH_OPERATION_NODE_H

/********************  HEADERS  *********************/
#include "common/CMRGeometry.h"

/*********************  TYPES  **********************/
class CMRVarSystem;
class CMRMeshOperation;

/*********************  CLASS  **********************/
struct CMRMeshOperationNode
{
	CMRMeshOperationNode(CMRMeshOperation *op,const CMRRect & rect) {this->op = op; this->rect = rect;};
	CMRMeshOperation * op;
	CMRRect rect;
};

#endif //CMR_MESH_OPERATION_NODE_H
