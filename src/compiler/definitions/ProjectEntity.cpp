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
#include <iomanip>
#include "parsor/LatexFormula.h"
#include "ProjectEntity.h"

/**********************  USING  *********************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
ProjectEntity::ProjectEntity ( const string& latexName, const string& longName )
{
	this->capName = false;
	this->longName = longName;
	this->applyLatexName(latexName);
}

/*******************  FUNCTION  *********************/
ProjectEntity::~ProjectEntity ( void )
{
	//do nothing, but virtual for inheritance
}

/*******************  FUNCTION  *********************/
void ProjectEntity::applyLatexName ( const string& latexName )
{
	//check if empty
	if (latexName.empty())
		throw LatexException("ERROR: you profide an empty latex name to define new entity !");

	//parse latex name
	LatexFormulas f(latexName);
	
	//check if get a uniq entity
	if (f.isSimpleEntity() == false)
		throw LatexException(string("ERROR: you provide a complex equation to define a project entity, this is not valid : ") + latexName);
	
	//extract name
	this->shortName = f[0]->getName();
	
	//fill capture
	fillCapture(indices,f[0]->getIndices());
	fillCapture(exponents,f[0]->getExponents());
	fillCapture(parameters,f[0]->getParameters());
}

/*******************  FUNCTION  *********************/
void ProjectEntity::fillCapture ( ProjectCaptureDefMap& capture, LatexFormulasVector& formulaList )
{
	//clear old list
	capture.clear();
	
	//loop on elements and fill
	for (LatexFormulasVector::const_iterator it = formulaList.begin() ; it != formulaList.end() ; ++it)
	{
		addCapture(capture,**it,CAPTURE_NONE);
	}
}

/*******************  FUNCTION  *********************/
void ProjectEntity::addIndice ( const string& name, CaptureType captureType )
{
	addCapture(indices,name,captureType);
}

/*******************  FUNCTION  *********************/
void ProjectEntity::addExponent ( const string& name, CaptureType captureType )
{
	addCapture(exponents,name,captureType);
}

/*******************  FUNCTION  *********************/
void ProjectEntity::addParameter ( const string& name, CaptureType captureType )
{
	addCapture(parameters,name,captureType);
}

/*******************  FUNCTION  *********************/
void ProjectEntity::addCapture ( ProjectCaptureDefMap& capture, const string& value, CaptureType captureType )
{
	assert(captureType != CAPTURE_OPTIONS);
	LatexFormulas f(value);
	addCapture(capture,f,captureType);
}

/*******************  FUNCTION  *********************/
void ProjectEntity::addCapture ( ProjectCaptureDefMap& capture, const LatexFormulas& formula, CaptureType captureType )
{
	assert(captureType != CAPTURE_OPTIONS);
	//check if get a uniq entity
	if (captureType != CAPTURE_NONE && formula.isSimpleEntity() == false)
		throw LatexException(string("ERROR: you provide a complex equation to to capture fields : ") + formula.getString());
	if (captureType != CAPTURE_NONE)
		ensureUniqCapture(formula);
	capture.push_back(CaptureDef(formula.getString(),captureType));
	onUpdateCaptureType(formula.getString(),captureType);
}

/*******************  FUNCTION  *********************/
void ProjectEntity::changeCaptureType ( const string& name, CaptureType captureType )
{
	bool res;
	
	//regen the name without spaces and good order
	LatexFormulas f(name);
	string tmp = f.getString();
	
	//check if get a uniq entity
	if (captureType != CAPTURE_NONE && f.isSimpleEntity() == false)
		throw LatexException(string("ERROR: you provide a complex equation to to capture fields : ") + tmp);
	
	//indices
	res = changeCaptureType(indices,tmp,captureType);
	
	//expo
	if (res == false)
		res = changeCaptureType(exponents,tmp,captureType);
	
	//params
	if (res == false)
		res = changeCaptureType(parameters,tmp,captureType);
	
	if (res == false)
		throw LatexException(string("Invalid name to change capture type (") + tmp + string(") in : ") + getLatexName());
	
	//call user fonction
	onUpdateCaptureType(name,captureType);
}

/*******************  FUNCTION  *********************/
bool ProjectEntity::changeCaptureType ( ProjectCaptureDefMap& capture, const string& name, CaptureType captureType )
{
	CaptureDef * def = findCaptureDef(capture,name);
	assert(captureType != CAPTURE_OPTIONS);
	
	if (def == NULL)
	{
		return false;
	} else {
		def->captureType = captureType;
		return true;
	}
}

/*******************  FUNCTION  *********************/
std::string ProjectEntity::formatCaptureList ( const ProjectCaptureDefMap& value, const string& sep, const string& open, const string& close, bool forceOpenClose )
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
	for (ProjectCaptureDefMap::const_iterator it = value.begin() ; it != value.end() ; ++it)
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
string ProjectEntity::getLatexName ( void ) const
{
	string res = shortName;
	
	if (indices.empty() == false)
	{
		res += '_';
		res += formatCaptureList(indices,",","{","}",true);
	}
	
	if (exponents.empty() == false)
	{
		res += '^';
		res += formatCaptureList(exponents,",","{","}",true);
	}
	
	res += formatCaptureList(parameters,"}{","{","}",true);
	
	return res;
}

/*******************  FUNCTION  *********************/
const string& ProjectEntity::getLongName ( void ) const
{
	return longName;
}

/*******************  FUNCTION  *********************/
const string& ProjectEntity::getShortName ( void ) const
{
	return shortName;
}

/*******************  FUNCTION  *********************/
bool ProjectEntity::match ( const LatexEntity& entity ) const
{
	return internalMatch(entity,NULL);
}

/*******************  FUNCTION  *********************/
void ProjectEntity::capture ( const LatexEntity& entity, ProjectCaptureMap& capture ) const
{
	bool res = internalMatch(entity,&capture);
	if (res == false)
		throw LatexException(string("Try to capture values in non mathing entities : ")+entity.getString()+ string(" != ")+getLatexName());
}

/*******************  FUNCTION  *********************/
void ProjectEntity::onUpdateCaptureType ( const string& name, CaptureType captureType )
{
	assert(captureType != CAPTURE_OPTIONS);
}

/*******************  FUNCTION  *********************/
ostream& operator<< ( ostream& out, const ProjectEntity& value )
{
	out << value.getLatexName();
	return out;
}

/*******************  FUNCTION  *********************/
bool ProjectEntity::internalMatch ( const LatexEntity& entity, ProjectCaptureMap* capture ) const
{
	//check name
	if (entity.name != this->shortName && this->capName == false)
		return false;
	
	//check indices
	if (internalMatch(entity.indices,indices,capture) == false)
		return false;
	
	//check expo
	if (exponents.empty() && entity.exponents.size() == 1)
	{
		if (capture != NULL)
			(*capture)["cmrExponent"] = entity.exponents[0];
	} else if (internalMatch(entity.exponents,exponents,capture) == false) {
		return false;
	}
	
	//check params
	if (entity.name == "()" && this->capName == true)
	{
		if (capture != NULL)
			(*capture)[this->shortName] = entity.parameters[0];
	} else if (internalMatch(entity.parameters,parameters,capture) == false) {
			return false;
	}
	
	//capture name if wildcars
	if (capture != NULL && this->capName && entity.name != "()")
		(*capture)[this->shortName] = new LatexFormulas(entity.name);

	return true;
}

/*******************  FUNCTION  *********************/
bool ProjectEntity::internalMatch ( const LatexFormulasVector& formulaList, const ProjectCaptureDefMap& captureDef, ProjectCaptureMap* captureOut ) const
{
	//trivial
	if (formulaList.size() != captureDef.size())
		return false;
	
	//check elements
	for (int i = 0 ; i < formulaList.size() ; i++)
	{
		switch(captureDef[i].captureType)
		{
			case CAPTURE_NONE:
				if (captureDef[i].name != formulaList[i]->getString())
					return false;
				break;
			case CAPTURE_OPTIONS:
				assert(false);
				abort();
				break;
			case CAPTURE_REQUIRED:
				if (captureOut != NULL)
					(*captureOut)[captureDef[i].name] = formulaList[i];
				break;
		}
	}
	
	return true;
}

/*******************  FUNCTION  *********************/
CaptureDef::CaptureDef ( const string& name, CaptureType captureType )
{
	this->name = name;
	this->captureType = captureType;
}

/*******************  FUNCTION  *********************/
CaptureDef* ProjectEntity::findCaptureDef ( ProjectCaptureDefMap& value, const string& name, bool beCaptured )
{
	for (ProjectCaptureDefMap::iterator it = value.begin() ; it != value.end() ; ++it)
		if (name == it->name && (beCaptured == false || it->captureType != CAPTURE_NONE))
			return &(*it);
		
	return NULL;
}

/*******************  FUNCTION  *********************/
CaptureDef* ProjectEntity::findCaptureDef ( const string& name, bool beCaptured )
{
	//vars
	CaptureDef * res;
	
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
void ProjectEntity::ensureUniqCapture ( const LatexFormulas& f )
{
	//vars
	std::string value = f.getString();
	
	//do capture
	CaptureDef * def = this->findCaptureDef(value,true);
	
	//cehck error
	if (def != NULL)
		throw LatexException(string("Caution, cannot add capture for ")+value+string(" as it was already defined !"));
}

/*******************  FUNCTION  *********************/
bool ProjectEntity::match ( const string& value ) const
{
	//convert
	LatexEntity e(value);
	
	//do mathcing
	return match(e);
}

/*******************  FUNCTION  *********************/
bool ProjectEntity::haveCapture ( const string& name )
{
	//do capture
	CaptureDef * def = this->findCaptureDef(name,true);
	
	return (def != NULL);
}

/*******************  FUNCTION  *********************/
StringVector ProjectEntity::getCapturedIndices ( void ) const
{
	//vars
	StringVector res;
	
	//loop to search capture
	for(ProjectCaptureDefMap::const_iterator it = indices.begin() ; it != indices.end() ; ++it)
	{
		assert(it->captureType != CAPTURE_OPTIONS);
		if (it->captureType == CAPTURE_REQUIRED)
			res.push_back(it->name);
	}
	
	return res;
}

/*******************  FUNCTION  *********************/
void ProjectEntity::printDebug ( ostream& out, const string& name, const ProjectCaptureDefMap& map ) 
{
	out << "    - " << setw(11) << left << name << ": ";
	for (int i = 0 ; i < map.size() ; i++)
	{
		if (i > 0)
			out << ", ";
		if (map[i].captureType == CAPTURE_REQUIRED)
			out << "[" << map[i].name << "]";
		else
			out << map[i].name;
	}
	out << endl;
}

/*******************  FUNCTION  *********************/
void ProjectEntity::printDebug ( ostream& out ) const
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

/*******************  FUNCTION  *********************/
const ProjectCaptureDefMap& ProjectEntity::getExponents(void ) const
{
	return exponents;
}

/*******************  FUNCTION  *********************/
const ProjectCaptureDefMap& ProjectEntity::getIndices(void ) const
{
	return indices;
}

/*******************  FUNCTION  *********************/
const ProjectCaptureDefMap& ProjectEntity::getParameters(void ) const
{
	return parameters;
}

/*******************  FUNCTION  *********************/
void ProjectEntity::markAllCaptureAs(CaptureType capture)
{
	markAllCaptureAs(indices,capture);
	markAllCaptureAs(exponents,capture);
	markAllCaptureAs(parameters,capture);
}

/*******************  FUNCTION  *********************/
void ProjectEntity::markAllCaptureAs(ProjectCaptureDefMap& map, CaptureType capture)
{
	for (ProjectCaptureDefMap::iterator it = map.begin() ; it != map.end() ; ++it)
		it->captureType = capture;
}

/*******************  FUNCTION  *********************/
void ProjectEntity::captureName ( void )
{
	this->capName = true;
}

/*******************  FUNCTION  *********************/
bool ProjectEntity::isWildcardName ( void )
{
	return this->capName;
}

}
