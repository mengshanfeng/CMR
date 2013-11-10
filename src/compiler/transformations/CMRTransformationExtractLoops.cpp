/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cassert>
#include <iostream>
#include <sstream>
#include "CMRTransformationExtractLoops.h"
#include "../parsor/CMRLatexEntity2.h"
#include "../definitions/CMRProjectCode.h"
#include "common/MsgFormat.h"

/**********************  USING  *********************/
using namespace std;
using namespace CMRCompiler;

/*******************  FUNCTION  *********************/
CMRTransformationExtractLoops::CMRTransformationExtractLoops ( void ) 
	: CMRTransformationBasic("CMRTransformationExtractLoops")
{

}

/*******************  FUNCTION  *********************/
static std::string getTempName(int id)
{
	stringstream tmp;
	tmp << "\\CMRTMP^"<< id <<"{" << id << "}";
	return tmp.str();
}

/*******************  FUNCTION  *********************/
void CMRTransformationExtractLoops::transform ( CMRProjectCodeEquation& equation, CMRLatexEntity2& entity )
{
	//vars
	std::string op;

	//check type
	if (entity.getName() == "\\sum")
		op = "+=";

	//do it if non empty op
	if (op.empty() == false)
	{
		//errors
		assert(entity.indices.size() == 1);
		assert(entity.exponents.empty());
		assert(entity.parameters.size() == 1);
		
		//setup temp variable
		CMRTempNames tmp = equation.getContext().genTempName();
		equation.getParent()->addLocalVariable(tmp.shortName,tmp.longName,"int","0");
		
		//create loop
		MsgFormat("Replace loops with iterator (%1) and core (%2)").arg(*entity.indices[0]).arg(*entity.parameters[0]).debug();
		CMRProjectCodeIteratorLoop & loop = equation.addIteratorLoop(entity.indices[0]->getString(),CMR_INSERT_BEFORE);
		loop.addEquation(tmp.shortName,entity.parameters[0]->getString(),op);
		
		//replace current entity
		entity.parse(tmp.shortName);
		
		//recursive run on loop
		this->run(loop);
	}

	CMRTransformationBasic::transform ( equation, entity );
}
