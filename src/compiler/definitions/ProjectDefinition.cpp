/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "transformations/CMRTransformation.h"
#include "common/Common.h"
#include "ProjectDefinition.h"

/**********************  USING  *********************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
ProjectDefinition::ProjectDefinition ( const string& latexName, const string& longName, ProjectContext* parentContext ) 
	: ProjectEntity ( latexName, longName )
	,parametersContext(parentContext), ops(&parametersContext)
{
	this->ops.addLocalVariable(latexName,"result","double","0");
}

/*******************  FUNCTION  *********************/
void ProjectDefinition::printDebug ( std::ostream& out ) const
{
	ProjectEntity::printDebug ( out );
	out << "    - code : TODO" << endl;
}

/*******************  FUNCTION  *********************/
void ProjectDefinition::genDefinitionCCode ( ostream& out , const ProjectContext& context, int padding) const
{
	doIndent(out,padding) << "//Definition : " << this->getLatexName() << " : " << getLongName() << endl;
	doIndent(out,padding) << "double compute_" << this->getLongName() << "(const VarSystem::CellAccessor & in,VarSystem::CellAccessor & out,int x,int y";
	
	genParameterListForDef(out,getIndices());
	genParameterListForDef(out,getExponents());
	genParameterListForDef(out,getParameters());

	out << ")" << endl;
	doIndent(out,padding) << "{" << endl;
	ops.genCCode(out,padding);
	doIndent(out,padding+1) << "return result;" << endl;
	doIndent(out,padding) << "}" << endl;
	out << endl;
}

/*******************  FUNCTION  *********************/
void ProjectDefinition::genParameterListForDef ( ostream& out, const ProjectCaptureDefMap& map) const
{
	for (ProjectCaptureDefMap::const_iterator it = map.begin() ; it != map.end() ; ++it)
	{
		assert(it->captureType != CAPTURE_OPTIONS);
		if (it->captureType == CAPTURE_REQUIRED)
		{
			const ProjectEntity * entity = parametersContext.find(it->name);
			assert(entity != NULL);
			out << ", int " << entity->getLongName();
		}
	}
}

/*******************  FUNCTION  *********************/
void ProjectDefinition::genParameterListForUsage ( ostream& out, const ProjectCaptureDefMap& map,ProjectCaptureMap & capture ) const
{
	for (ProjectCaptureDefMap::const_iterator it = map.begin() ; it != map.end() ; ++it)
	{
		assert(it->captureType != CAPTURE_OPTIONS);
		if (it->captureType == CAPTURE_REQUIRED)
			out << ", (" << *capture[it->name] << ")";
	}
}

/*******************  FUNCTION  *********************/
void ProjectDefinition::genUsageCCode ( ostream& out, const ProjectContext& context, const LatexEntity& entity, bool write ) const
{
	//extract matching
	ProjectCaptureMap capture;
	
	//extract matching
	this->capture(entity,capture);
	
	out << "compute_" << this->getLongName() << "(in,out,x,y";
	genParameterListForUsage(out,getIndices(),capture);
	genParameterListForUsage(out,getExponents(),capture);
	genParameterListForUsage(out,getParameters(),capture);
	out << ")";
}

/*******************  FUNCTION  *********************/
void ProjectDefinition::runTransformation(CMRTransformation & transf)
{
	transf.run(ops);
}

/*******************  FUNCTION  *********************/
CMRProjectCodeEquation& ProjectDefinition::addEquation ( const string& eq )
{
	//search position of =
	int equalPos = -1;
	for (int i = 0 ; i < eq.size() ; i++)
	{
		if (eq[i] == '=')
		{
			equalPos = i;
			break;
		}
	}
	
	//split
	string left = eq.substr(0,equalPos);
	string op = "=";
	string right = eq.substr(equalPos+1, string::npos);
	//cout << "Splitting of eq : ==>" << left << "<== , operator '" << op << "' ==>" << right << "<==" << endl;
	
	return addEquation(left,right,op);
}

/*******************  FUNCTION  *********************/
CMRProjectCodeEquation& ProjectDefinition::addEquation ( const string& latexName, const string& compute, const string& op )
{
	return ops.addEquation(latexName,compute,op);
}

/*******************  FUNCTION  *********************/
CMRProjectCodeIteratorLoop& ProjectDefinition::addIteratorLoop ( const string& iterator )
{
	return ops.addIteratorLoop(iterator);
}

/*******************  FUNCTION  *********************/
CMRProjectLocalVariable& ProjectDefinition::addLocalVariable ( const string& latexName, const string& longName, const string& type, const string& defaultValue )
{
	return ops.addLocalVariable(latexName,longName,type,defaultValue);
}

/*******************  FUNCTION  *********************/
ProjectIterator& ProjectDefinition::addIterator ( const string& latexName, const string& longName, int start, int end )
{
	return ops.addIterator(latexName,longName,start,end);
}

/*******************  FUNCTION  *********************/
void ProjectDefinition::onUpdateCaptureType ( const string& name, CaptureType captureType )
{
	if (captureType == CAPTURE_REQUIRED)
	{
		//TODO : create a parameter entity
		string longName = parametersContext.genTempName("param").longName;
		this->parametersContext.addEntry(new CMRProjectLocalVariable(name,longName,"int"));
	}
}

/*******************  FUNCTION  *********************/
ProjectContext& ProjectDefinition::getContext ( void )
{
	return ops.getContext();
}

/*******************  FUNCTION  *********************/
CMRProjectCodeEntry* ProjectDefinition::insert ( CMRProjectCodeEntry* entry, ProjectCodeTreeInsert location )
{
	return ops.insert(entry,location);
}

}
