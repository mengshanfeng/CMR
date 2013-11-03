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
void CMRProjectAction::genDefinitionCCode ( ostream& out, const CMRProjectContext& context, int padding ) const
{
	doIndent(out,padding) << "//" << this->descr << endl;
	doIndent(out,padding) << "struct Action" << this->name << endl;
	doIndent(out,padding) << "{" << endl;
	doIndent(out,padding+1) << "static void cellAction(const VarSystem::CellAccessor & in,VarSystem::CellAccessor& out,const CMRCellPosition & pos,int x,int y)" << endl;
	doIndent(out,padding+1) << "{" << endl;
	ops.genCCode(out,padding+1);
	doIndent(out,padding+1) << "}" << endl;
	doIndent(out,padding) << "};" << endl;
	out << endl;
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
