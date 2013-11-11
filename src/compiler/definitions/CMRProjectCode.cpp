/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <sstream>
#include <cstdio>
#include "../common/Debug.h"
#include "CMRProjectCode.h"
#include "CMRGenCode.h"
#include "common/Common.h"
#include "CMRProjectIterator.h"

/**********************  USING  *********************/
using namespace std;
using namespace CMRCompiler;

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
void CMRProjectLocalVariable::genUsageCCode ( std::ostream& out, const CMRProjectContext& context, const LatexEntity& entity, bool write ) const
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
CMRProjectCodeEquation& CMRProjectCodeEntry::addEquation ( const string& eq )
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
		throw LatexException("Invalid iterator name : "+iterator);

	const CMRProjectIterator * it = dynamic_cast<const CMRProjectIterator*>(entity);
	if (it == NULL)
		throw LatexException("Invalid iterator type : "+iterator);
	
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
LatexFormulas& CMRProjectCodeEquation::getFormulas(void )
{
	return formula;
}

/*******************  FUNCTION  *********************/
LatexEntity& CMRProjectCodeEquation::getOutput(void )
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

/*******************  FUNCTION  *********************/
CMRProjectCConstruct::CMRProjectCConstruct ( const string& code )
{
	this->loadCode(code);
}

/*******************  FUNCTION  *********************/
CMRProjectCConstruct& CMRProjectCConstruct::arg ( const string& value )
{
	this->args.push_back(new LatexFormulas(value));
	return *this;
}

/*******************  FUNCTION  *********************/
void CMRProjectCConstruct::loadCode ( const string& code )
{
	//errors
	assert(this->code.empty());
	assert(this->autoArgs.empty());
	assert(this->args.empty());

	//some vars
	string buffer;
	bool capture = false;
	char prev = '\0';
	int cnt = 1;
	stringstream codePattern;
	
	//fill code while searching $....$ values, extract them and replace by %a1, %a2.
	for (int i = 0 ; i < code.size() ; i++)
	{
		//escape sequence
		if (code[i] == '$' && prev != '\\') {
			if (capture)
			{
				this->autoArgs.push_back(new LatexFormulas(buffer));
				codePattern << "%a" << cnt++;
			} else {
				buffer.clear();
			}
			capture = !capture;
		} else if (capture) {
			buffer += code[i];
		} else {
			codePattern << code[i];
		}

		prev = code[i];
	}
	
	//setup pattern
	this->code = codePattern.str();
	//cmrDebug("Extracted code template is : %s",this->code.c_str());
}

/*******************  FUNCTION  *********************/
void CMRProjectCConstruct::extractReplacementLocus ( ExtractionLocusList& locusList ) const
{
	//var
	ExtractionLocus locus;
	char prev = '\0';
	
	//errors
	assert(locusList.empty());
	
	//loop to search %[0-9]+ or %a[0-9]+
	for (int i = 0 ; i < code.size() ; i++)
	{
		if (code[i] == '%' && prev != '\\')
		{
			locus.isAutoEntry = (code[i+1] == 'a');
			locus.id = atoi(code.c_str()+i+(locus.isAutoEntry?2:1));
			locus.position = i;
			cmrAssume("Invalid locus definition '%s...'",code.substr(i,4).c_str());
			locusList.push_back(locus);
		}
		prev = code[i];
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectCConstruct::genCCode ( ostream& out, const CMRProjectContext& context, int padding ) const
{
	//vars
	ExtractionLocusList locusList;
	string res = code;
	char tmp[16];
	int size;
	
	//Errors
	assert(padding >= 0);
	
	//extract locus list
	extractReplacementLocus(locusList);
	
	//loop on all locus and replace, replace by end, so position are still known after first replace
	for (ExtractionLocusList::const_reverse_iterator it = locusList.rbegin() ; it != locusList.rend() ; ++it)
	{
		const LatexFormulas * formula = getLocusValue(*it);
		assert(formula != NULL);
		stringstream buffer;
		cmrGenEqCCode(buffer,context,*formula);
		size = sprintf(tmp,"%%%s%d",(it->isAutoEntry?"a":""),it->id);
		res = res.replace(it->position, size, buffer.str());
	}
	
	doIndent(out,padding) << res << endl;
}

/*******************  FUNCTION  *********************/
const LatexFormulas* CMRProjectCConstruct::getLocusValue ( const ExtractionLocus& locus ) const
{
	cmrAssume(locus.id > 0,"Locus ID must be greater than 0 (%d)",locus.id);
	if (locus.isAutoEntry)
	{
		cmrAssume(autoArgs.size() >= locus.id,"Invalid locus ID in automatic entry of CConstruct : %d",locus.id);
		return autoArgs[locus.id-1];
	} else {
		cmrAssume(args.size() >= locus.id,"Invalid locus ID in automatic entry of CConstruct : %d",locus.id);
		return args[locus.id-1];
	}
}

/*******************  FUNCTION  *********************/
CMRProjectCSimpleConstruct::CMRProjectCSimpleConstruct ( CMRProjectContext * parentContext, const string& code ) 
	: CMRProjectCodeNode(parentContext), construct(code)
{
	
}

/*******************  FUNCTION  *********************/
CMRProjectCSimpleConstruct& CMRProjectCSimpleConstruct::arg ( const string& value )
{
	construct.arg(value);
	return *this;
}

/*******************  FUNCTION  *********************/
CMRProjectCodeType CMRProjectCSimpleConstruct::getType ( void ) const
{
	return CMR_PROJECT_CODE_CSIMPLE;
}

/*******************  FUNCTION  *********************/
void CMRProjectCSimpleConstruct::genCCode ( ostream& out, int padding ) const
{
	construct.genCCode(out,context,padding);
}
