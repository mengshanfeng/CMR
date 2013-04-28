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
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "CMRProjectEntity.h"
#include "../parsor/CMRLatexFormula.h"
#include "../parsor/CMRParsorBasics.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
CMRProjectEntity::CMRProjectEntity ( const string& latexName, const string& longName )
{
	this->longName = longName;
	this->applyLatexName(latexName);
}

/*******************  FUNCTION  *********************/
CMRProjectEntity::~CMRProjectEntity ( void )
{
	//do nothing, but virtual for inheritance
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity::applyLatexName ( const string& latexName )
{
	//check if empty
	if (latexName.empty())
		throw CMRLatexException("ERROR: you profide an empty latex name to define new entity !");

	//parse latex name
	CMRLatexFormulas2 f(latexName);
	
	//check if get a uniq entity
	if (f.isSimpleEntity() == false)
		throw CMRLatexException(string("ERROR: you provide a complex equation to define a project entity, this is not valid : ") + latexName);
	
	//extract name
	this->shortName = f[0]->getName();
	
	//fill capture
	fillCapture(indices,f[0]->getIndices());
	fillCapture(exponents,f[0]->getExponents());
	fillCapture(parameters,f[0]->getParameters());
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity::fillCapture ( CMRProjectCaptureDefMap& capture, CMRLatexFormulasVector2& formulaList )
{
	//clear old list
	capture.clear();
	
	//loop on elements and fill
	for (CMRLatexFormulasVector2::const_iterator it = formulaList.begin() ; it != formulaList.end() ; ++it)
	{
		addCapture(capture,**it,CMR_CAPTURE_NONE);
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity::addIndice ( const string& name, CMRCaptureType captureType )
{
	addCapture(indices,name,captureType);
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity::addExponent ( const string& name, CMRCaptureType captureType )
{
	addCapture(exponents,name,captureType);
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity::addParameter ( const string& name, CMRCaptureType captureType )
{
	addCapture(parameters,name,captureType);
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity::addCapture ( CMRProjectCaptureDefMap& capture, const string& value, CMRCaptureType captureType )
{
	assert(captureType != CMR_CAPTURE_OPTIONS);
	CMRLatexFormulas2 f(value);
	addCapture(capture,f,captureType);
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity::addCapture ( CMRProjectCaptureDefMap& capture, const CMRLatexFormulas2& formula, CMRCaptureType captureType )
{
	assert(captureType != CMR_CAPTURE_OPTIONS);
	//check if get a uniq entity
	if (captureType != CMR_CAPTURE_NONE && formula.isSimpleEntity() == false)
		throw CMRLatexException(string("ERROR: you provide a complex equation to to capture fields : ") + formula.getString());
	if (captureType != CMR_CAPTURE_NONE)
		ensureUniqCapture(formula);
	capture.push_back(CMRCaptureDef(formula.getString(),captureType));
	onUpdateCaptureType(formula.getString(),captureType);
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity::changeCaptureType ( const string& name, CMRCaptureType captureType )
{
	bool res;
	
	//regen the name without spaces and good order
	CMRLatexFormulas2 f(name);
	string tmp = f.getString();
	
	//check if get a uniq entity
	if (captureType != CMR_CAPTURE_NONE && f.isSimpleEntity() == false)
		throw CMRLatexException(string("ERROR: you provide a complex equation to to capture fields : ") + tmp);
	
	//indices
	res = changeCaptureType(indices,tmp,captureType);
	
	//expo
	if (res == false)
		res = changeCaptureType(exponents,tmp,captureType);
	
	//params
	if (res == false)
		res = changeCaptureType(parameters,tmp,captureType);
	
	if (res == false)
		throw CMRLatexException(string("Invalid name to change capture type (") + tmp + string(") in : ") + getLatexName());
	
	//call user fonction
	onUpdateCaptureType(name,captureType);
}

/*******************  FUNCTION  *********************/
bool CMRProjectEntity::changeCaptureType ( CMRProjectCaptureDefMap& capture, const string& name, CMRCaptureType captureType )
{
	CMRCaptureDef * def = findCaptureDef(capture,name);
	assert(captureType != CMR_CAPTURE_OPTIONS);
	
	if (def == NULL)
	{
		return false;
	} else {
		def->captureType = captureType;
		return true;
	}
}

/*******************  FUNCTION  *********************/
std::string CMRProjectEntity::formatCaptureList ( const CMRProjectCaptureDefMap& value, const string& sep, const string& open, const string& close, bool forceOpenClose )
{
	//vars
	string res;

	//trivial
	if (value.empty())
		return "";
	
	//open
	if (forceOpenClose || value.size() > 1)
		res += open;
	
	//print elements
	for (CMRProjectCaptureDefMap::const_iterator it = value.begin() ; it != value.end() ; ++it)
	{
		if (it != value.begin())
			res += sep;
		res += it->name;
	}

	//close
	if (forceOpenClose || value.size() > 1)
		res += close;
	
	return res;
}

/*******************  FUNCTION  *********************/
string CMRProjectEntity::getLatexName ( void ) const
{
	string res = shortName;
	
	if (indices.empty() == false)
	{
		res += '_';
		res += formatCaptureList(indices,",","{","}",false);
	}
	
	if (exponents.empty() == false)
	{
		res += '^';
		res += formatCaptureList(exponents,",","{","}",false);
	}
	
	res += formatCaptureList(parameters,"}{","{","}",true);
	
	return res;
}

/*******************  FUNCTION  *********************/
const string& CMRProjectEntity::getLongName ( void ) const
{
	return longName;
}

/*******************  FUNCTION  *********************/
const string& CMRProjectEntity::getShortName ( void ) const
{
	return shortName;
}

/*******************  FUNCTION  *********************/
bool CMRProjectEntity::match ( const CMRLatexEntity2& entity ) const
{
	return internalMatch(entity,NULL);
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity::capture ( const CMRLatexEntity2& entity, CMRProjectCaptureMap& capture ) const
{
	bool res = internalMatch(entity,&capture);
	if (res == false)
		throw CMRLatexException(string("Try to capture values in non mathing entities : ")+entity.getString()+ string(" != ")+getLatexName());
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity::onUpdateCaptureType ( const string& name, CMRCaptureType captureType )
{
	assert(captureType != CMR_CAPTURE_OPTIONS);
}

/*******************  FUNCTION  *********************/
ostream& operator<< ( ostream& out, const CMRProjectEntity& value )
{
	out << value.getLatexName();
	return out;
}

/*******************  FUNCTION  *********************/
bool CMRProjectEntity::internalMatch ( const CMRLatexEntity2& entity, CMRProjectCaptureMap* capture ) const
{
	//check name
	if (entity.name != this->shortName)
		return false;
	
	//check indices
	if (internalMatch(entity.indices,indices,capture) == false)
		return false;
	
	//check expo
	if (internalMatch(entity.exponents,exponents,capture) == false)
		return false;
	
	//check params
	if (internalMatch(entity.parameters,parameters,capture) == false)
		return false;
	
	return true;
}

/*******************  FUNCTION  *********************/
bool CMRProjectEntity::internalMatch ( const CMRLatexFormulasVector2& formulaList, const CMRProjectCaptureDefMap& captureDef, CMRProjectCaptureMap* captureOut ) const
{
	//trivial
	if (formulaList.size() != captureDef.size())
		return false;
	
	//check elements
	for (int i = 0 ; i < formulaList.size() ; i++)
	{
		switch(captureDef[i].captureType)
		{
			case CMR_CAPTURE_NONE:
				if (captureDef[i].name != formulaList[i]->getString())
					return false;
				break;
			case CMR_CAPTURE_OPTIONS:
				assert(false);
				abort();
				break;
			case CMR_CAPTURE_REQUIRED:
				if (captureOut != NULL)
					(*captureOut)[captureDef[i].name] = formulaList[i];
				break;
		}
	}
	
	return true;
}

/*******************  FUNCTION  *********************/
CMRCaptureDef::CMRCaptureDef ( const string& name, CMRCaptureType captureType )
{
	this->name = name;
	this->captureType = captureType;
}

/*******************  FUNCTION  *********************/
CMRCaptureDef* CMRProjectEntity::findCaptureDef ( CMRProjectCaptureDefMap& value, const string& name, bool beCaptured )
{
	for (CMRProjectCaptureDefMap::iterator it = value.begin() ; it != value.end() ; ++it)
		if (name == it->name && (beCaptured == false || it->captureType != CMR_CAPTURE_NONE))
			return &(*it);
		
	return NULL;
}

/*******************  FUNCTION  *********************/
CMRCaptureDef* CMRProjectEntity::findCaptureDef ( const string& name, bool beCaptured )
{
	//vars
	CMRCaptureDef * res;
	
	//search in lists
	res = findCaptureDef(indices,name,beCaptured);
	if (res == NULL)
		res = findCaptureDef(exponents,name,beCaptured);
	if (res== NULL)
		res = findCaptureDef(parameters,name,beCaptured);
	
	//return final res
	return res;
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity::ensureUniqCapture ( const CMRLatexFormulas2& f )
{
	//vars
	std::string value = f.getString();
	
	//do capture
	CMRCaptureDef * def = this->findCaptureDef(value,true);
	
	//cehck error
	if (def != NULL)
		throw CMRLatexException(string("Caution, cannot add capture for ")+value+string(" as it was already defined !"));
}

/*******************  FUNCTION  *********************/
bool CMRProjectEntity::match ( const string& value ) const
{
	//convert
	CMRLatexEntity2 e(value);
	
	//do mathcing
	return match(e);
}

/*******************  FUNCTION  *********************/
bool CMRProjectEntity::haveCapture ( const string& name )
{
	//do capture
	CMRCaptureDef * def = this->findCaptureDef(name,true);
	
	return (def != NULL);
}

/*******************  FUNCTION  *********************/
CMRStringVector CMRProjectEntity::getCapturedIndices ( void ) const
{
	//vars
	CMRStringVector res;
	
	//loop to search capture
	for(CMRProjectCaptureDefMap::const_iterator it = indices.begin() ; it != indices.end() ; ++it)
	{
		assert(it->captureType != CMR_CAPTURE_OPTIONS);
		if (it->captureType == CMR_CAPTURE_REQUIRED)
			res.push_back(it->name);
	}
	
	return res;
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity::printDebug ( ostream& out, const string& name, const CMRProjectCaptureDefMap& map ) 
{
	out << "    - " << setw(11) << left << name << ": ";
	for (int i = 0 ; i < map.size() ; i++)
	{
		if (i > 0)
			out << ", ";
		if (map[i].captureType == CMR_CAPTURE_REQUIRED)
			out << "[" << map[i].name << "]";
		else
			out << map[i].name;
	}
	out << endl;
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity::printDebug ( ostream& out ) const
{
	out << "Entity :" << endl;
	out << "    - latexName  : " << getLatexName() << endl;
	out << "    - shortName  : " << getShortName() << endl;
	out << "    - longName   : " << getLongName() << endl;
	printDebug(out,"exponent",exponents);
	printDebug(out,"indices",indices);
	printDebug(out,"parameters",parameters);
// 	for (int i = 0 ; i < indices.size() ; i++)
// 		printf("           * %s [%d]\n",indices[i].c_str(),indicesCapture[i]);
}
