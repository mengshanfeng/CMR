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
#include "../transformations/CMRTransformation.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
CMRProjectDefinition::CMRProjectDefinition ( const string& latexName, const string& longName, CMRProjectContext* parentContext ) 
	: CMRProjectEntity ( latexName, longName )
	, ops(parentContext)
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
void CMRProjectDefinition::genParameterListForDef ( ostream& out, const CMRProjectCaptureDefMap& map )
{
	for (CMRProjectCaptureDefMap::const_iterator it = map.begin() ; it != map.end() ; ++it)
	{
		assert(it->captureType != CMR_CAPTURE_OPTIONS);
		if (it->captureType == CMR_CAPTURE_REQUIRED)
			out << ", int " << it->name;
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectDefinition::genParameterListForUsage ( ostream& out, const CMRProjectCaptureDefMap& map,CMRProjectCaptureMap & capture )
{
	for (CMRProjectCaptureDefMap::const_iterator it = map.begin() ; it != map.end() ; ++it)
	{
		assert(it->captureType != CMR_CAPTURE_OPTIONS);
		if (it->captureType == CMR_CAPTURE_REQUIRED)
			out << ", (" << *capture[it->name] << ")";
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectDefinition::genUsageCCode ( ostream& out, const CMRProjectContext& context, const CMRLatexEntity2& entity, bool write ) const
{
	//extract matching
	CMRProjectCaptureMap capture;
	
	//extract matching
	this->capture(entity,capture);
	
	out << "compute_" << this->getLongName() << "(in,out,x,y";
	genParameterListForUsage(out,getIndices(),capture);
	genParameterListForUsage(out,getExponents(),capture);
	genParameterListForUsage(out,getParameters(),capture);
	out << ");";
}

/*******************  FUNCTION  *********************/
void CMRProjectDefinition::runTransformation(CMRTransformation & transf)
{
	transf.run(ops);
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