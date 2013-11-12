/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "CMRProjectDefinition.h"
#include "common/Common.h"
#include "../transformations/CMRTransformation.h"

/**********************  USING  *********************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
CMRProjectDefinition::CMRProjectDefinition ( const string& latexName, const string& longName, CMRProjectContext* parentContext ) 
	: CMRProjectEntity ( latexName, longName )
	,parametersContext(parentContext), ops(&parametersContext)
{
	this->ops.addLocalVariable(latexName,"result","double","0");
}

/*******************  FUNCTION  *********************/
void CMRProjectDefinition::printDebug ( std::ostream& out ) const
{
	CMRProjectEntity::printDebug ( out );
	out << "    - code : TODO" << endl;
}

/*******************  FUNCTION  *********************/
void CMRProjectDefinition::genDefinitionCCode ( ostream& out , const CMRProjectContext& context, int padding) const
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
void CMRProjectDefinition::genParameterListForDef ( ostream& out, const CMRProjectCaptureDefMap& map) const
{
	for (CMRProjectCaptureDefMap::const_iterator it = map.begin() ; it != map.end() ; ++it)
	{
		assert(it->captureType != CMR_CAPTURE_OPTIONS);
		if (it->captureType == CMR_CAPTURE_REQUIRED)
		{
			const CMRProjectEntity * entity = parametersContext.find(it->name);
			assert(entity != NULL);
			out << ", int " << entity->getLongName();
		}
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectDefinition::genParameterListForUsage ( ostream& out, const CMRProjectCaptureDefMap& map,CMRProjectCaptureMap & capture ) const
{
	for (CMRProjectCaptureDefMap::const_iterator it = map.begin() ; it != map.end() ; ++it)
	{
		assert(it->captureType != CMR_CAPTURE_OPTIONS);
		if (it->captureType == CMR_CAPTURE_REQUIRED)
			out << ", (" << *capture[it->name] << ")";
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectDefinition::genUsageCCode ( ostream& out, const CMRProjectContext& context, const LatexEntity& entity, bool write ) const
{
	//extract matching
	CMRProjectCaptureMap capture;
	
	//extract matching
	this->capture(entity,capture);
	
	out << "compute_" << this->getLongName() << "(in,out,x,y";
	genParameterListForUsage(out,getIndices(),capture);
	genParameterListForUsage(out,getExponents(),capture);
	genParameterListForUsage(out,getParameters(),capture);
	out << ")";
}

/*******************  FUNCTION  *********************/
void CMRProjectDefinition::runTransformation(CMRTransformation & transf)
{
	transf.run(ops);
}

/*******************  FUNCTION  *********************/
CMRProjectCodeEquation& CMRProjectDefinition::addEquation ( const string& eq )
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
CMRProjectCodeEquation& CMRProjectDefinition::addEquation ( const string& latexName, const string& compute, const string& op )
{
	return ops.addEquation(latexName,compute,op);
}

/*******************  FUNCTION  *********************/
CMRProjectCodeIteratorLoop& CMRProjectDefinition::addIteratorLoop ( const string& iterator )
{
	return ops.addIteratorLoop(iterator);
}

/*******************  FUNCTION  *********************/
CMRProjectLocalVariable& CMRProjectDefinition::addLocalVariable ( const string& latexName, const string& longName, const string& type, const string& defaultValue )
{
	return ops.addLocalVariable(latexName,longName,type,defaultValue);
}

/*******************  FUNCTION  *********************/
CMRProjectIterator& CMRProjectDefinition::addIterator ( const string& latexName, const string& longName, int start, int end )
{
	return ops.addIterator(latexName,longName,start,end);
}

/*******************  FUNCTION  *********************/
void CMRProjectDefinition::onUpdateCaptureType ( const string& name, CMRCaptureType captureType )
{
	if (captureType == CMR_CAPTURE_REQUIRED)
	{
		//TODO : create a parameter entity
		string longName = parametersContext.genTempName("param").longName;
		this->parametersContext.addEntry(new CMRProjectLocalVariable(name,longName,"int"));
	}
}

/*******************  FUNCTION  *********************/
CMRProjectContext& CMRProjectDefinition::getContext ( void )
{
	return ops.getContext();
}

/*******************  FUNCTION  *********************/
CMRProjectCodeEntry* CMRProjectDefinition::insert ( CMRProjectCodeEntry* entry, CMRProjectCodeTreeInsert location )
{
	return ops.insert(entry,location);
}

}
