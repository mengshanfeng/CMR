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
#include "common/LangDef.h"
#include "common/Common.h"
#include "ProjectAction.h"

/**********************  USING  *********************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
ProjectAction::ProjectAction ( const string& name, const string& descr, const string& loopType, ProjectContext* parentContext )
	:ops(parentContext)
{
	this->name = name;
	this->descr = descr;
	this->loopType = loopType;
	this->ops.getContext().setKey("CMRActionLoopType",loopType);
}

/*******************  FUNCTION  *********************/
CMRProjectCodeEquation& ProjectAction::addEquation ( const string& latexName, const string& compute, const string& op )
{
	return ops.addEquation(latexName,compute,op);
}

/*******************  FUNCTION  *********************/
ProjectIterator& ProjectAction::addIterator ( const string& latexName, const string& longName, int start, int end )
{
	return ops.addIterator(latexName,longName,start,end);
}

/*******************  FUNCTION  *********************/
CMRProjectCodeIteratorLoop& ProjectAction::addIteratorLoop ( const string& iterator )
{
	return ops.addIteratorLoop(iterator);
}

/*******************  FUNCTION  *********************/
CMRProjectLocalVariable& ProjectAction::addLocalVariable ( const string& latexName, const string& longName, const string& type, const string& defaultValue )
{
	return ops.addLocalVariable(latexName,longName,type,defaultValue);
}

/*******************  FUNCTION  *********************/
void ProjectAction::runTransformation ( CMRTransformation& transf )
{
	transf.run(ops);
}

/*******************  FUNCTION  *********************/
CMRProjectCodeEquation& ProjectAction::addEquation ( const string& eq )
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
void ProjectAction::genDefinitionCCode ( ostream& out, const LangDef & lang,const ProjectContext& context, int padding ) const
{
	CodeTemplateValueDic dic;
	dic.set("descr",this->descr);
	dic.set("name",this->name);
	dic.set("looptype",this->loopType);
	dic.set("code",new CMRCodeValueForCodeEntry(&this->ops,-1));
	//dic.set("params",new CodeTemplateValueActionParameters(&parameters,",",true));
	dic.set("storage",new CodeTemplateValueActionParameters(&parameters,";",false,true,true));

	lang.applyOn(out,string("cellActionBodyCode") + loopType, dic,padding);
}

/*******************  FUNCTION  *********************/
void ProjectAction::genUsageCCode ( ostream& out, const ProjectContext& context, const LatexEntity& entity, bool write ) const
{
	assert(false);
}

/*******************  FUNCTION  *********************/
void ProjectAction::printDebug ( ostream& out ) const
{
	out << "TODO" << endl;
}

/*******************  FUNCTION  *********************/
ProjectContext& ProjectAction::getContext ( void )
{
	return ops.getContext();
}

/*******************  FUNCTION  *********************/
CMRProjectCodeEntry* ProjectAction::insert ( CMRProjectCodeEntry* entry, ProjectCodeTreeInsert location )
{
	return ops.insert(entry,location);
}

/*******************  FUNCTION  *********************/
ProjectActionParameter& ProjectAction::addParameter ( const string& latexName, const string& longName, const string& type )
{
	ProjectActionParameter * res = new ProjectActionParameter(latexName,longName,type);
	parameters.push_back(res);
	ops.getContext().addEntry(res);
	return *res;
}

/*******************  FUNCTION  *********************/
ProjectActionParameter::ProjectActionParameter ( const string& latexName, const string& longName, const string& type ) 
	: ProjectEntityOld ( latexName, longName )
{
	this->type = type;
}

/*******************  FUNCTION  *********************/
void ProjectActionParameter::genDefinitionCCode ( ostream& out, const ProjectContext& context, int padding ) const
{
	return;
}

/*******************  FUNCTION  *********************/
void ProjectActionParameter::genUsageCCode ( ostream& out, const ProjectContext& context, const LatexEntity& entity, bool write ) const
{
	out << this->getLongName();
}

/*******************  FUNCTION  *********************/
const string& ProjectActionParameter::getType ( void ) const
{
	return this->type;
}

/*******************  FUNCTION  *********************/
CodeTemplateValueActionParameters::CodeTemplateValueActionParameters ( const ProjectActionParameterVector* list, const string& separator, bool firstSeparator, bool lastSeparator, bool indentEach )
{
	assert(list != NULL);
	this->list = list;
	this->separator = separator;
	this->firstSeparator = firstSeparator;
	this->lastSeparator = lastSeparator;
	this->indentEach = indentEach;
}

/*******************  FUNCTION  *********************/
void CodeTemplateValueActionParameters::genCode ( ostream& out, int indent ) const
{
	if (firstSeparator && list->empty() == false)
		out << separator;
	
	for (ProjectActionParameterVector::const_iterator it = list->begin() ; it != list->end() ; ++it)
	{
		if (it != list->begin())
			out << separator;
		if (indentEach)
		{
			if (it != list->begin())
				out << endl;
			doIndent(out,indent);
		}
		out << (*it)->getType() << " " << (*it)->getLongName();
	}
	
	if (lastSeparator && list->empty() == false)
		out << separator;
}

}
