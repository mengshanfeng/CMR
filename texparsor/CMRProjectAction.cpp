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
#include "CMRProjectAction.h"

using namespace std;

/*******************  FUNCTION  *********************/
CMRProjectAction::CMRProjectAction ( CMRProjectContext* parentContext, string name, string descr )
	:context(parentContext)
{
	assert(parentContext != NULL);
	this->name = name;
	this->description = descr;
}

/*******************  FUNCTION  *********************/
CMRProjectEquation& CMRProjectAction::addEquation ( const string& latexName, const string& longName, const string& compute )
{
	CMRProjectActionBlock * tmpBlock = new CMRProjectActionBlock(&context);
	CMRProjectEquation * tmp = tmpBlock->eq = new CMRProjectEquation(latexName,longName,compute);
	tmpBlock->loopDescr = "cmrEquation";
	context.entities.push_back(tmp);
	return *tmp;
}

/*******************  FUNCTION  *********************/
CMRProjectActionBlock& CMRProjectAction::addSubBlock ( string loopDescr, string parameter )
{
	CMRProjectActionBlock * tmpBlock = new CMRProjectActionBlock(&context);
	tmpBlock->loopDescr = loopDescr;
	tmpBlock->parameter = parameter;
	return *tmpBlock;
}

/*******************  FUNCTION  *********************/
void CMRProjectAction::replaceLoops(void )
{
	for (CMRProjectActionBlockVector::iterator it = blocks.begin() ; it != blocks.end() ; ++it)
		(*it)->replaceLoops();
}

/*******************  FUNCTION  *********************/
void CMRProjectActionBlock::replaceLoops(void )
{
	CMRLatexEntity * term;

	if (loopDescr == "cmrEquation")
	{
		if (eq != NULL)
		{
			while ((term = eq->extractNextInnerLoop()) != NULL)
			{
				
			}
		}
	} else {
		for (CMRProjectActionBlockVector::iterator it = subblocks.begin() ; it != subblocks.end() ; ++it)
			(*it)->replaceLoops();
	}
}
