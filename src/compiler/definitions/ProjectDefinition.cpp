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
#include "common/Debug.h"
#include "ProjectDefinition.h"
#include "GenCode.h"

/**********************  USING  *********************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
ProjectDefinition::ProjectDefinition ( const string& latexName, const string& longName, ProjectContext* parentContext ) 
	: ProjectEntityOld ( latexName, longName )
	,parametersContext(parentContext), ops(&parametersContext)
{
	this->ops.addLocalVariable(latexName,"result","double","0");
}

/*******************  FUNCTION  *********************/
void ProjectDefinition::printDebug ( std::ostream& out ) const
{
	out << "/*********************** Entity info ******************************" << endl;
	ProjectEntityOld::printDebug ( out );
	out << "-------------------------------------------------------------------" << endl;
	out << "Parameter context : " << endl;
	this->parametersContext.printDebug(out);
	out << "******************************************************************/" << endl;
	out << this->getLongName() << "[" << this->getLatexName() << "]" << "()" << endl;
	out << "{" << endl;
	this->ops.printDebug(out,0);
	out << "}" << endl;
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
			//TODO remove this hack
			string name = it->name;
			if (sizes.find(name) != sizes.end())
				name += "_*";
			const IProjectEntity * entity = parametersContext.find(name);
			assert(entity != NULL);
			if (types.find(it->name) == types.end())
				out << ", int " << entity->getLongName();
			else
				out << ", "<< types.find(it->name)->second << " " << entity->getLongName();
			if (sizes.find(it->name) != sizes.end())
				out << "[" << sizes.find(it->name)->second << "]";
		}
	}
}

/*******************  FUNCTION  *********************/
void ProjectDefinition::genParameterListForUsage ( ostream& out, const ProjectCaptureDefMap& map,ProjectCaptureMap & capture,const ProjectContext & context) const
{
	for (ProjectCaptureDefMap::const_iterator it = map.begin() ; it != map.end() ; ++it)
	{
		assert(it->captureType != CAPTURE_OPTIONS);
		if (it->captureType == CAPTURE_REQUIRED)
		{
			out << ", (";
			LatexFormulas f(*capture[it->name]);
			cmrGenEqCCode(out,context,f);
			out << ")";
		}
	}
}

/*******************  FUNCTION  *********************/
void ProjectDefinition::genUsageCCode ( ostream& out, const ProjectContext& context, const LatexEntity& entity, bool write ) const
{
	//extract matching
	ProjectCaptureMap capture;
	const std::string & loopType = context.readKey("CMRActionLoopType");
	
	//extract matching
	this->capture(entity,capture);
	
	out << "compute_" << this->getLongName() << "(";
	if (loopType == "CMRMeshOperationSimpleLoop" || loopType == "CMRMeshOperationSimpleLoopWithPos" || loopType.empty())
	{
		out << "in,out";
	} else if (loopType == "CMRMeshOperationSimpleLoopInPlace" || loopType == "CMRMeshOperationSimpleLoopInPlaceWithPos") {
		out << "cell,cell";
	} else {
		cmrFatal("Invalid action loop type : %s",loopType.c_str());
	}
	
	out << ",x,y";
	genParameterListForUsage(out,getIndices(),capture,context);
	genParameterListForUsage(out,getExponents(),capture,context);
	genParameterListForUsage(out,getParameters(),capture,context);
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
		CMRProjectLocalVariable * var = new CMRProjectLocalVariable(name,longName,"int");
		this->parametersContext.addEntry(var);
		if (sizes.find(name) != sizes.end())
			var->addDim(sizes[name]);
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

/*******************  FUNCTION  *********************/
void ProjectDefinition::setCaptureSize ( const string& name, const string& type, int size )
{
	if (size > 0)
		sizes[name] = size;
	if (type.empty())
		types[name] = "int";
	else
		types[name] = type;
}

}
