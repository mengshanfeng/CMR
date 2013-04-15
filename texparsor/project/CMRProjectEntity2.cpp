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
#include "CMRProjectEntity2.h"
#include "../parsor/CMRLatexFormula.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
CMRProjectEntity2::CMRProjectEntity2 ( const string& latexName, const string& longName )
{
	this->longName = longName;
	this->applyLatexName(latexName);
}

/*******************  FUNCTION  *********************/
CMRProjectEntity2::~CMRProjectEntity2 ( void )
{
	//do nothing, but virtual for inheritance
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity2::applyLatexName ( const string& latexName )
{
	//check if empty
	if (latexName.empty())
		throw CMRLatexException("ERROR: you profide an empty latex name to define new entity !");

	//parse latex name
	CMRLatexFormulas2 f(latexName);
	
	//check if get a uniq entity
	if (f.isSimpleEntity())
		throw CMRLatexException(string("ERROR: you provide a complex equation to define a project entity, this is not valid : ") + latexName);
	
	//extract name
	this->shortName = f[0]->getName();
	
	//fill capture
	fillCapture(indices,f[0]->getIndices());
	fillCapture(exponents,f[0]->getExponents());
	fillCapture(parameters,f[0]->getParameters());
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity2::fillCapture ( CMRProjectCaptureDefMap& capture, CMRLatexFormulasVector2& formulaList )
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
void CMRProjectEntity2::addIndice ( const string& name, CMRCaptureType captureType )
{
	addCapture(indices,name,captureType);
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity2::addExponent ( const string& name, CMRCaptureType captureType )
{
	addCapture(exponents,name,captureType);
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity2::addParameter ( const string& name, CMRCaptureType captureType )
{
	addCapture(parameters,name,captureType);
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity2::addCapture ( CMRProjectCaptureDefMap& capture, const string& value, CMRCaptureType captureType )
{
	assert(captureType != CMR_CAPTURE_OPTIONS);
	CMRLatexFormulas2 f(value);
	addCapture(capture,f,captureType);
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity2::addCapture ( CMRProjectCaptureDefMap& capture, const CMRLatexFormulas2& formula, CMRCaptureType captureType )
{
	assert(captureType != CMR_CAPTURE_OPTIONS);
	//check if get a uniq entity
	if (formula.isSimpleEntity())
		throw CMRLatexException(string("ERROR: you provide a complex equation to to capture fields : ") + formula.getString());
	capture.push_back(CMRCaptureDef(formula.getString(),CMR_CAPTURE_NONE));
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity2::changeCaptureType ( const string& name, CMRCaptureType captureType )
{
	bool res;
	
	//regen the name without spaces and good order
	CMRLatexFormulas2 f(name);
	string tmp = f.getString();
	
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
}

/*******************  FUNCTION  *********************/
bool CMRProjectEntity2::changeCaptureType ( CMRProjectCaptureDefMap& capture, const string& name, CMRCaptureType captureType )
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
std::string CMRProjectEntity2::formatCaptureList ( const CMRProjectCaptureDefMap& value, const string& sep, const string& open, const string& close, bool forceOpenClose )
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
string CMRProjectEntity2::getLatexName ( void ) const
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
const string& CMRProjectEntity2::getLongName ( void ) const
{
	return longName;
}

/*******************  FUNCTION  *********************/
const string& CMRProjectEntity2::getShortName ( void ) const
{
	return shortName;
}

/*******************  FUNCTION  *********************/
bool CMRProjectEntity2::match ( CMRLatexEntity2& entity ) const
{
	return internalMatch(entity,NULL);
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity2::capture ( CMRLatexEntity2& entity, CMRProjectCaptureMap2& capture ) const
{
	bool res = internalMatch(entity,&capture);
	assert(res);
}

/*******************  FUNCTION  *********************/
void CMRProjectEntity2::onUpdateCaptureType ( const string& name, CMRCaptureType captureType )
{
	assert(captureType != CMR_CAPTURE_OPTIONS);
}

/*******************  FUNCTION  *********************/
ostream& operator<< ( ostream& out, const CMRProjectEntity2& value )
{
	out << value.getLatexName();
	return out;
}

/*******************  FUNCTION  *********************/
bool CMRProjectEntity2::internalMatch ( CMRLatexEntity2& entity, CMRProjectCaptureMap2* capture ) const
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
bool CMRProjectEntity2::internalMatch ( CMRLatexFormulasVector2& formulaList, const CMRProjectCaptureDefMap& captureDef, CMRProjectCaptureMap2* captureOut ) const
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
				if (captureDef[i].name == formulaList[i]->getString())
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
