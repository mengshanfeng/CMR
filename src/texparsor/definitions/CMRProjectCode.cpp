/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include "CMRProjectCode.h"
#include "CMRGenCode.h"
#include "CMRProjectIterator.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
CMRProjectLocalVariable::CMRProjectLocalVariable ( const std::string& latexName, const std::string& longName , const std::string & type,const std::string & defaultValue) 
	: CMRProjectEntity ( latexName, longName )
	, defaultValue(defaultValue)
{
	this->type = type;
}

/*******************  FUNCTION  *********************/
void CMRProjectLocalVariable::genDefinitionCCode ( std::ostream& out, const CMRProjectContext& context, int padding ) const
{
	doIndent(out,padding) << type << " " << getLongName() << " = ";
	cmrGenEqCCode(out,context,defaultValue);
	out << ";" << endl;
}

/*******************  FUNCTION  *********************/
void CMRProjectLocalVariable::genUsageCCode ( std::ostream& out, const CMRProjectContext& context, const CMRLatexEntity2& entity, bool write ) const
{
	out << getLongName();
}

/*******************  FUNCTION  *********************/
CMRProjectContext& CMRProjectCodeEntry::getContext ( void )
{
	return context;
}

/*******************  FUNCTION  *********************/
void CMRProjectCodeEntry::setParentContext ( CMRProjectContext* parentContext )
{
	this->context.setParent(parentContext);
}

/*******************  FUNCTION  *********************/
void CMRProjectCodeEntry::onParentChange ( CMRProjectCodeEntry* newParent )
{
	if (newParent == NULL)
		context.setParent(NULL);
	else
		context.setParent(&newParent->context);
}

/*******************  FUNCTION  *********************/
CMRProjectCodeType CMRProjectCodeNode::getType ( void ) const
{
	return CMR_PROJECT_CODE_NODE;
}

/*******************  FUNCTION  *********************/
CMRProjectCodeEquation::CMRProjectCodeEquation ( const string& latexName, const string& compute, const std::string &op )
	:output(latexName)
	,formula(compute)
	,op(op)
{
	assert(latexName.empty() == false);
	assert(compute.empty() == false);
}

/*******************  FUNCTION  *********************/
CMRProjectCodeType CMRProjectCodeEquation::getType ( void ) const
{
	return CMR_PROJECT_CODE_EQUATION;
}

/*******************  FUNCTION  *********************/
CMRProjectCodeIteratorLoop::CMRProjectCodeIteratorLoop ( const std::string & iterator )
{
	//TODO  search realted iterator and maybe create tmp var if not found
	//const CMRProjectEntity entity = context.find("iterator");
	
	//create local variable
// 	string name = context.genTempName("tmp_it");
// 	this->addLocalVariable(iterator,name,"int","0");
	
	this->iterator = iterator;
}

/*******************  FUNCTION  *********************/
void CMRProjectCodeIteratorLoop::genCCode ( ostream& out, int padding ) const
{
	//search iterator info
	const CMRProjectEntity * iteratorEntity = context.find(iterator);
	assert(iteratorEntity != NULL);
	
	//gen loop
	doIndent(out,padding) << "for(";
	iteratorEntity->genDefinitionCCode(out,context,padding);
	out << " )" << endl;
	//body part
	CMRProjectCodeNode::genCCode ( out,padding );
}

/*******************  FUNCTION  *********************/
const CMRProjectIterator& CMRProjectCodeIteratorLoop::getIterator(void ) const
{
	//build iterator loop
	const CMRProjectEntity * entity = context.findInParent(iterator);
	if (entity == NULL)
		throw CMRLatexException("Invalid iterator name : "+iterator);

	const CMRProjectIterator * it = dynamic_cast<const CMRProjectIterator*>(entity);
	if (it == NULL)
		throw CMRLatexException("Invalid iterator type : "+iterator);
	
	return *it;
}

/*******************  FUNCTION  *********************/
CMRProjectIterator& CMRProjectCodeEntry::addIterator ( const string& latexName, const string& longName, int start, int end )
{
	CMRProjectIterator * it = new CMRProjectIterator(latexName,longName,start,end);
	context.addEntry(it);
	return *it;
}

/*******************  FUNCTION  *********************/
CMRProjectCodeNode& CMRProjectCodeEntry::addSubBlock ( CMRProjectCodeTreeInsert location )
{
	CMRProjectCodeNode * res = new CMRProjectCodeNode();
	this->insert(res,location);
	return *res;
}

/*******************  FUNCTION  *********************/
CMRProjectCodeEquation& CMRProjectCodeEntry::addEquation ( const string& latexName, const string& compute, const string& op, CMRProjectCodeTreeInsert location )
{
	CMRProjectCodeEquation * eq = new CMRProjectCodeEquation(latexName,compute,op);
	this->insert(eq,location);
	return *eq;
}

/*******************  FUNCTION  *********************/
CMRProjectCodeIteratorLoop & CMRProjectCodeEntry::addIteratorLoop ( const string& iterator, CMRProjectCodeTreeInsert location )
{
	CMRProjectCodeIteratorLoop * loop = new CMRProjectCodeIteratorLoop(iterator);
	this->insert(loop,location);
	return * loop;
}

/*******************  FUNCTION  *********************/
CMRProjectCodeType CMRProjectCodeIteratorLoop::getType ( void ) const
{
	return CMR_PROJECT_CODE_ITERATOR;
}

/*******************  FUNCTION  *********************/
CMRProjectLocalVariable& CMRProjectCodeEntry::addLocalVariable ( const string& latexName, const string& longName, const string& type, const string& defaultValue, CMRProjectCodeTreeInsert location )
{
	CMRProjectLocalVariable * var = new CMRProjectLocalVariable(latexName,longName,type,defaultValue);
	#warning check location of context
	this->context.addEntry(var);
	this->insert(new CMRProjectCodeVarDecl(var),location);
	return *var;
}

/*******************  FUNCTION  *********************/
CMRProjectCodeNode::CMRProjectCodeNode(CMRProjectContext* context)
	:CMRProjectCodeEntry(context)
{
	
}

/*******************  FUNCTION  *********************/
CMRProjectCodeEntry::CMRProjectCodeEntry(CMRProjectContext* context)
	:context(context)
{

}
/*******************  FUNCTION  *********************/
CMRLatexFormulas2& CMRProjectCodeEquation::getFormulas(void )
{
	return formula;
}

/*******************  FUNCTION  *********************/
CMRLatexEntity2& CMRProjectCodeEquation::getOutput(void )
{
	return output;
}

/*******************  FUNCTION  *********************/
const string& CMRProjectCodeEquation::getOperator ( void ) const
{
	return op;
}

/*******************  FUNCTION  *********************/
void CMRProjectCodeEquation::setOperator ( const string& op )
{
	this->op = op;
}

/*******************  FUNCTION  *********************/
void CMRProjectCodeEntry::genChildCCode ( ostream& out ,int padding ) const
{
	const CMRProjectCodeEntry * cur = this->getFirstChild();
	const CMRProjectCodeEntry * end = this->getLastChild();
	while (cur != NULL)
	{
		cur->genCCode(out,padding);
		cur = cur->getNext();
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectCodeNode::genCCode ( ostream& out, int padding ) const
{
	doIndent(out,padding) << "{" << endl;
	genChildCCode(out,padding);
	doIndent(out,padding) << "}" << endl;
}

/*******************  FUNCTION  *********************/
void CMRProjectCodeEquation::genCCode ( ostream& out, int padding ) const
{
	const CMRProjectEntity * outputEntity = context.find(output);
	assert(outputEntity != NULL);
	
	doIndent(out,padding);
	outputEntity->genUsageCCode(out,context,output,true);
	out << " " << op << " ";
	cmrGenEqCCode(out,context,formula);
	out << ";" << endl;
	
// 	//errors
// 	assert(eq != NULL);
// 	//check if in context of if new def
// 	const CMRProjectEntity * entity = context.find(eq->latexEntity);
// 	
// 	//indent
// 	out << genCCodeIndent(depth);
// 	
// 	//if new def, need to insert declaration code
// 	if (entity == NULL)
// 	{
// 		out << "double ";
// // 		entity = & context.addEntry(new CMRProjectLocalVariable(eq->latexName,eq->longName));
// 	}
// 	
// 	//write left member
// 	entity->genUsageCCode(out,context,eq->latexEntity);
// 	out << " = ";
// 	
// 	//loop on body elements
// 	cmrGenEqCCode(out,context,eq->formula);
// 	
// 	out << ";" << endl;
}

/*******************  FUNCTION  *********************/
ostream& doIndent ( ostream& out, int depth )
{
	for (int i = 0 ; i < depth ; i++)
		out << "\t";
	return out;
}

/*******************  FUNCTION  *********************/
CMRProjectCodeRootNode::CMRProjectCodeRootNode ( CMRProjectContext* context ) : CMRProjectCodeNode ( context )
{
	
}

/*******************  FUNCTION  *********************/
void CMRProjectCodeRootNode::genCCode ( ostream& out, int padding ) const
{
	genChildCCode(out,padding);
}

/*******************  FUNCTION  *********************/
CMRProjectCodeVarDecl::CMRProjectCodeVarDecl ( CMRProjectLocalVariable* variable )
{
	assert(variable != NULL);
	this->variable = variable;
}

/*******************  FUNCTION  *********************/
CMRProjectCodeType CMRProjectCodeVarDecl::getType ( void ) const
{
	return CMR_PROJECT_CODE_VAR_DECL;
}

/*******************  FUNCTION  *********************/
void CMRProjectCodeVarDecl::genCCode ( ostream& out ,int padding ) const
{
	variable->genDefinitionCCode(out,context,getDepth()+padding);
}

/*******************  FUNCTION  *********************/
std::ostream & CMRProjectCodeEntry::doIndent ( ostream& out, int baseOffset ) const
{
	return ::doIndent(out,baseOffset + getDepth());
}
