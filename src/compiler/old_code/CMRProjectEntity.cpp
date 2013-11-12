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
#include "parsor/TexParsor.h"
#include "CMRProjectEntity.h"

/**********************  USING  *********************/
using namespace std;
using namespace CMRCompiler;

/********************  NAMESPACE  *******************/
namespace CMRCompilerOld
{

/*******************  FUNCTION  *********************/
CMRProjectEntity::CMRProjectEntity ( const string& latexName, const string& longName )
{
	this->longName = longName;
	this->applyLatexName(latexName);
	this->requiredIndices = 0;
	this->captureExponent = false;
}

/*******************  FUNCTION  *********************/
CMRProjectEntity::~CMRProjectEntity ( void )
{
	//nothing to do
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity::addIndice ( const string& name ,CMRCaptureType capture)
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
void CMRProjectEntity::applyLatexName ( const string& latexName )
{
	//vars
	LatexFormulasOld f;

	//errors
	assert(latexName.empty() == false);

	//parse
	cmrParseLatexFormula(f,latexName);
	
	//check only one element and extract it
	assert(f.childs.size() == 1);
	LatexEntityOld * entity = f.childs[0];
	
	//get name and exponent
	this->shortName = entity->name;
	this->exponent = entity->superscriptTotalValue;
	this->latexEntity = *entity;
	
	//extract subscript info
	for (LatexFormulasListOld::iterator it = entity->subscript.begin() ; it != entity->subscript.end() ; ++it)
		this->addIndice((*it)->string,CMR_CAPTURE_NONE);
	
	this->latexName = latexName;
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity::printDebug ( void ) const
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
bool CMRProjectEntity::match ( LatexEntityOld& entity, CMRProjectCaptureMap& capture) const
{
	if (entity.name != shortName || (entity.superscriptTotalValue != exponent && exponent.empty() == false))
		return false;
	
	assert(indices.size() == indicesCapture.size());
	
	LatexFormulasListOld tmp = entity.getIndices();
	if (tmp.size() <= indices.size() && tmp.size() >= requiredIndices)
	{
		//capture
		for (int i = 0 ; i < tmp.size() ; i++)
		{
			if (indicesCapture[i] != CMR_CAPTURE_NONE)
			{
				capture[indices[i]] = tmp[i];
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
			capture["cmrExponent"] = entity.superscript[0];
		return true;
	} else {
		fprintf(stderr,"Caution, not same indices on %s for matching %s\n",entity.getString().c_str(),latexName.c_str());
		return false;
	}
}

/*******************  FUNCTION  *********************/
bool CMRProjectEntity::haveCaptureFor ( const string name ) const
{
	for (int i = 0 ; i < indices.size() ; i++)
	{
		if (indices[i] == name)
			return indicesCapture[i] != CMR_CAPTURE_NONE;
	}
	return false;
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity::madeCaptureIndice ( const string name, CMRCaptureType capture )
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

/*******************  FUNCTION  *********************/
void CMRProjectEntity::setCaptureExponent(bool status)
{
	captureExponent = status;
}

}
