/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "../transformations/CMRTransformation.h"
#include "../common/CMRCommon.h"
#include "../common/CMRCodeTemplate.h"
#include "../common/CMRLangDef.h"
#include "CMRProjectAction.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
CMRProjectAction::CMRProjectAction ( const string& name, const string& descr, CMRProjectContext* parentContext )
	:ops(parentContext)
{
	this->name = name;
	this->descr = descr;
}

/*******************  FUNCTION  *********************/
CMRProjectCodeEquation& CMRProjectAction::addEquation ( const string& latexName, const string& compute, const string& op )
{
	return ops.addEquation(latexName,compute,op);
}

/*******************  FUNCTION  *********************/
CMRProjectIterator& CMRProjectAction::addIterator ( const string& latexName, const string& longName, int start, int end )
{
	return ops.addIterator(latexName,longName,start,end);
}

/*******************  FUNCTION  *********************/
CMRProjectCodeIteratorLoop& CMRProjectAction::addIteratorLoop ( const string& iterator )
{
	return ops.addIteratorLoop(iterator);
}

/*******************  FUNCTION  *********************/
CMRProjectLocalVariable& CMRProjectAction::addLocalVariable ( const string& latexName, const string& longName, const string& type, const string& defaultValue )
{
	return ops.addLocalVariable(latexName,longName,type,defaultValue);
}

/*******************  FUNCTION  *********************/
void CMRProjectAction::runTransformation ( CMRTransformation& transf )
{
	transf.run(ops);
}

/*******************  FUNCTION  *********************/
CMRProjectCodeEquation& CMRProjectAction::addEquation ( const string& eq )
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
void CMRProjectAction::genDefinitionCCode ( ostream& out, const CMRLangDef & lang,const CMRProjectContext& context, int padding ) const
{
	CMRCodeTemplateValueDic dic;
	dic.set("descr",this->descr);
	dic.set("name",this->name);
	dic.set("code",new CMRCodeValueForCodeEntry(&this->ops,-1));
	
	lang.applyOn(out,"cellActionBodyCode", dic,padding);
}

/*******************  FUNCTION  *********************/
void CMRProjectAction::genUsageCCode ( ostream& out, const CMRProjectContext& context, const CMRLatexEntity2& entity, bool write ) const
{
	assert(false);
}

/*******************  FUNCTION  *********************/
void CMRProjectAction::printDebug ( ostream& out ) const
{
	out << "TODO" << endl;
}

/*******************  FUNCTION  *********************/
CMRProjectContext& CMRProjectAction::getContext ( void )
{
	return ops.getContext();
}

/*******************  FUNCTION  *********************/
CMRProjectCodeEntry* CMRProjectAction::insert ( CMRProjectCodeEntry* entry, CMRProjectCodeTreeInsert location )
{
	return ops.insert(entry,location);
}
