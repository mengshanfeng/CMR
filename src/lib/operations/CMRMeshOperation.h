/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 02/2013
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_MESH_OPERATIONS_H
#define CMR_MESH_OPERATIONS_H

/********************  HEADERS  *********************/

/*********************  TYPES  **********************/
class CMRVarSystem;
struct CMRRect;

/*********************  CLASS  **********************/
class CMRMeshOperation
{
	public:
		CMRMeshOperation(bool needPermut = false){this->needPermut = needPermut;}
		virtual ~CMRMeshOperation(void){}
		virtual void run(CMRVarSystem * sys, const CMRRect & zone) = 0;
		bool checkNeedPermut(void) const {return needPermut;};
	private:
		bool needPermut;
};


#endif //CMR_MESH_OPERATIONS_H
