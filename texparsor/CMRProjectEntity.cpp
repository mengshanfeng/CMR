/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include "CMRTexParsor.h"
#include "CMRProjectEntity.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
CMREntity::CMREntity ( const string& latexName, const string& longName )
{
	this->latexName = latexName;
	this->longName = longName;
	this->applyLatexName(latexName);
	this->requiredIndices = 0;
}

/*******************  FUNCTION  *********************/
CMREntity::~CMREntity ( void )
{
	//nothing to do
}

/*******************  FUNCTION  *********************/
void CMREntity::addIndice ( const string& name ,CMRCaptureType capture)
{
	indices.push_back(name);
	if (name == "i" || name == "j")
	{
		printf("Caution, automatic capture for i/j\n");
		capture = CMR_CAPTURE_REQUIRED;
	}
	indicesCapture.push_back(capture);
	if (capture != CMR_CAPTURE_OPTIONS)
		this->requiredIndices++;
}

/*******************  FUNCTION  *********************/
void CMREntity::applyLatexName ( const string& latexName )
{
	//vars
	CMRLatexFormulas f;

	//errors
	assert(latexName.empty() == false);

	//parse
	cmrParseLatexFormula(f,latexName);
	
	//check only one element and extract it
	assert(f.childs.size() == 1);
	CMRLatexEntity * entity = f.childs[0];
	
	//get name and exponent
	this->shortName = entity->name;
	this->exponent = entity->superscriptTotalValue;
	
	//extract subscript info
	if (entity->subscript.childs.empty() == false)
	{
		if (entity->subscript.childs[0]->name == "\\COMMA_GROUP")
		{
			for (CMRLatexFormulasList::iterator it = entity->subscript.childs[0]->params.begin() ; it != entity->subscript.childs[0]->params.end() ; ++it)
				this->addIndice((*it)->string,CMR_CAPTURE_NONE);
		} else {
			this->addIndice(entity->subscriptTotalValue,CMR_CAPTURE_NONE);
		}
	}
}

/*******************  FUNCTION  *********************/
void CMREntity::printDebug ( void ) const
{
	printf("Entity : \n");
	printf("    - latexName : %s\n",latexName.c_str());
	printf("    - shortName : %s\n",shortName.c_str());
	printf("    - longName  : %s\n",longName.c_str());
	printf("    - exponent  : %s\n",exponent.c_str());
	printf("    - indices   : \n");
	for (int i = 0 ; i < indices.size() ; i++)
		printf("           * %s [%d]\n",indices[i].c_str(),indicesCapture[i]);
}

/*******************  FUNCTION  *********************/
bool CMREntity::match ( CMRLatexEntity& entity, CMRIndiceCaptureMap& capture )
{
	if (entity.name != shortName || (entity.superscriptTotalValue != exponent && exponent.empty() == false))
		return false;
	
	CMRLatexFormulasList tmp = entity.getIndices();
	if (tmp.size() <= indices.size() & tmp.size() >= requiredIndices)
	{
		//capture
		for (int i = 0 ; i < tmp.size() ; i++)
		{
			if (indicesCapture[i] != CMR_CAPTURE_NONE)
			{
				capture[indices[i]] = tmp[i]->string;
			} else {
				if (tmp[i]->string != indices[i])
				{
					fprintf(stderr,"Caution, not same indices on %s for matching %s\n",entity.getString().c_str(),latexName.c_str());
					return false;
				}
			}
		}
		//capture exponent
		if (entity.superscriptTotalValue.empty() == false && exponent.empty() == true)
			capture["cmrExponent"] = entity.superscriptTotalValue;
		return true;
	} else {
		fprintf(stderr,"Caution, not same indices on %s for matching %s\n",entity.getString().c_str(),latexName.c_str());
		return false;
	}
}

/*******************  FUNCTION  *********************/
bool CMREntity::haveCaptureFor ( const string name ) const
{
	for (int i = 0 ; i < indices.size() ; i++)
	{
		if (indices[i] == name)
			return indicesCapture[i] != CMR_CAPTURE_NONE;
	}
	return false;
}

/*******************  FUNCTION  *********************/
void CMREntity::madeCaptureIndice ( const string name, CMRCaptureType capture )
{
	for (int i = 0 ; i < indices.size() ; i++)
	{
		if (indices[i] == name)
		{
			indicesCapture[i] = capture;
			return;
		}
	}
	assert(false);
	abort();
}
