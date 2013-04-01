#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <map>
#include <sstream>
#include "parsor/CMRLatexEntity.h"
#include "parsor/CMRParsorBasics.h"
#include "parsor/CMRTexParsor.h"
#include "parsor/CMREqDepMatrix.h"
#include "parsor/CMREntityMatching.h"
#include "CMRProjectEntity.h"
#include "CMRProjectConstant.h"

/********************  NAMESPACE  *******************/
using namespace std;

/*********************  TYPES  **********************/
class CMRProjectVariable;
class CMRProjectIterator;
struct CMRProjectActionBlock;
class CMRProjectEquation;
class CMRProjectAction;

/*********************  TYPES  **********************/
typedef std::vector <CMREntityConstant*> CMRProjectConstantVector;
typedef std::vector <CMRProjectVariable*> CMRProjectVariableVector;
typedef std::vector <CMRProjectIterator*> CMRProjectIteratorVector;
typedef std::vector <CMRProjectActionBlock*> CMRProjectActionBlockVector;
typedef std::vector <CMREntity*> CMRProjectEntityList;
typedef std::vector <CMRProjectAction *> CMRProjectActionVector;
typedef std::vector <CMRProjectEquation*> CMRProjectEquationVector;

/*********************  CLASS  **********************/
struct CMRProjectContext
{
	CMRProjectContext(CMRProjectContext * parent) {this->parent = parent;};
	CMRProjectContext * parent;
	CMRProjectEntityList entities;
};

/*********************  CLASS  **********************/
class CMRProject
{
	public:
		CMRProject() :rootContext(NULL) {};
		CMREntityConstant & addConstant(const std::string& latexName, const std::string& longName);
		CMRProjectVariable & addvariable(const string& latexName, const string& longName, const string& type);
		CMRProjectAction & addAction(std::string name,std::string descr = "");
	private:
		CMRProjectConstantVector constants;
		CMRProjectVariableVector variables;
		CMRProjectActionVector actions;
		CMRProjectContext rootContext;
};

/*********************  CLASS  *********************/
struct CMRProjectActionBlock
{
	CMRProjectActionBlock(CMRProjectContext * parent) :context(parent) {};
	std::string loopDescr;
	CMRProjectEquation * eq;
	CMRProjectActionBlockVector subblocks;
	CMRProjectContext context;
};

/*********************  CLASS  *********************/
class CMRProjectAction
{
	public:
		CMRProjectAction(CMRProjectContext * parentContext,std::string name,std::string descr = "");
		CMRProjectActionBlock & addSubBlock(std::string loopDescr);
		CMRProjectEquation& addEquation( const string& latexName, const string& longName, const string& compute );
	private:
	CMRProjectActionBlockVector blocks;
	std::string name;
	std::string description;
	CMRProjectContext context;
};

/*********************  CLASS  **********************/
class CMRProjectEquation : public CMREntity
{
	public:
		CMRProjectEquation ( const string& latexName, const string& longName , const string & compute);
		void printDebug(void) const;
		CMRLatexEntity * extractNextInnerLoop(void);
	private:
		string compute;
		CMRLatexFormulas formula;
};

/*******************  FUNCTION  *********************/
CMRProjectEquation::CMRProjectEquation ( const string& latexName, const string& longName, const string& compute ) 
	: CMREntity ( latexName, longName )
{
	this->compute = compute;
	cmrParseLatexFormula(formula,compute);
}

/*******************  FUNCTION  *********************/
void CMRProjectEquation::printDebug ( void ) const
{
	CMREntity::printDebug();
	printf("    - value     : %s\n",compute.c_str());
}

/*********************  CLASS  **********************/
class CMRProjectIterator : public CMREntity
{
	public:
		CMRProjectIterator ( const string& latexName, const string& longName ,int start, int end);
		void printDebug(void) const;
		void printCPPCode(void) const;
	private:
		int start;
		int end;
};

/*******************  FUNCTION  *********************/
CMRProjectIterator::CMRProjectIterator ( const string& latexName, const string& longName, int start, int end ) 
	: CMREntity ( latexName, longName )
{
	this->start = start;
	this->end = end;
}

/*******************  FUNCTION  *********************/
void CMRProjectIterator::printDebug ( void ) const
{
	CMREntity::printDebug();
	printf("    - values    : %d .. %d\n",start,end);
}

/*******************  FUNCTION  *********************/
void CMRProjectIterator::printCPPCode ( void ) const
{
	cout << "int " << longName << " = " << start << " ; " << longName << " <= " << end << " ; " << longName << "++" << endl;
}

/*******************  FUNCTION  *********************/
CMRLatexEntity* extractFirstInnerLoop(CMRLatexFormulas & formula)
{
	for (CMRLatexEntityVector::iterator it = formula.childs.begin() ; it != formula.childs.end() ; ++it)
	{
		if ((*it)->name == "\\sum")
			return *it;
	}
	return NULL;
}

/*******************  FUNCTION  *********************/
CMRLatexEntity* CMRProjectEquation::extractNextInnerLoop ( void )
{
	return extractFirstInnerLoop(formula);
}

/*********************  CLASS  **********************/
class CMRProjectVariable : public CMREntity
{
	public:
		CMRProjectVariable ( const string& latexName, const string& longName, const string& type );
		void addDim( int size, const string& name, int start = 0);
		void printCPPCode(void) const;
		std::string getTypeWithDims(void) const;
	private:
		int ghostDepths;
		std::string type;
		std::string memoryModel;
		CMRConstantDimensionsVector dims;
		CMRConstantDimensionsVector dimStart;
		CMRStringVector dimNames;
};

/*******************  FUNCTION  *********************/
CMRProjectVariable::CMRProjectVariable ( const string& latexName, const string& longName , const std::string & type) 
	: CMREntity ( latexName, longName )
{
	//check that we have i/j for capture
	assert(haveCaptureFor("i"));
	assert(haveCaptureFor("j"));
	assert(type.empty() == false);
	ghostDepths = 1;
	memoryModel = "CMRMemoryModelColMajor";
	this->type = type;
}

/*******************  FUNCTION  *********************/
void CMRProjectVariable::addDim ( int size, const string& name,int start )
{
	assert(size > 0);
	assert(name.empty() == false);
	dims.push_back(size);
	dimNames.push_back(name);
	dimStart.push_back(start);
	madeCaptureIndice(name,CMR_CAPTURE_OPTIONS);
}

/*******************  FUNCTION  *********************/
void CMRProjectVariable::printCPPCode ( void ) const
{
	//definition
	cout << "//define variable " << latexName << endl;
	cout << "this->addVariable(\"" << longName << "\",sizeof(" << getTypeWithDims() << ")," << ghostDepths << ");" << endl;
	
	//accessor member definition
	cout << "//define variable " << latexName << endl;
	cout << "CMRCellAccessor<" << getTypeWithDims() << "," << memoryModel << "> " << longName << endl;
	
	//build local accessor from domain
	cout << longName << "*(sys.getDomain(0,tstep)),x,y,absolute)" << endl;
	
	//build local accessor from parent accessor
	cout << longName << "(acc.directions,x,y,absolute)" << endl;
}

/*******************  FUNCTION  *********************/
string CMRProjectVariable::getTypeWithDims ( void ) const
{
	stringstream res;
	res << type;
	for (int i = 0 ; i < dims.size() ; i++)
		res << "[" << dims[i] << "]";
	return res.str();
}

/*******************  FUNCTION  *********************/
CMRProjectEquation * convertLoopToEq(CMRLatexEntity * entity)
{
	//nothing to do
	if (entity == NULL)
		return NULL;
	//extract the loop id
	string id = entity->subscriptTotalValue;
	cout << "===> loop id = " << id << endl;
	//extract inner term
	return new CMRProjectEquation("Z_1","CMR_inner_loop_1",entity->params[0]->string);
}

/*******************  FUNCTION  *********************/
CMREntityConstant& CMRProject::addConstant ( const string& latexName, const string& longName )
{
	CMREntityConstant * tmp = new CMREntityConstant(latexName,longName);
	constants.push_back(tmp);
	rootContext.entities.push_back(tmp);
	return *tmp;
}

/*******************  FUNCTION  *********************/
CMRProjectVariable& CMRProject::addvariable ( const string& latexName, const string& longName, const string& type )
{
	CMRProjectVariable * tmp = new CMRProjectVariable(latexName,longName,type);
	variables.push_back(tmp);
	rootContext.entities.push_back(tmp);
	return *tmp;
}

/*******************  FUNCTION  *********************/
CMRProjectAction::CMRProjectAction ( CMRProjectContext* parentContext, string name, string descr )
	:context(parentContext)
{
	assert(parentContext != NULL);
	this->name = name;
	this->description = descr;
}

/*******************  FUNCTION  *********************/
CMRProjectEquation& CMRProjectAction::addEquation ( const string& latexName, const string& longName, const string& compute )
{
	CMRProjectActionBlock * tmpBlock = new CMRProjectActionBlock(&context);
	CMRProjectEquation * tmp = tmpBlock->eq = new CMRProjectEquation(latexName,longName,compute);
	context.entities.push_back(tmp);
	return *tmp;
}

/*******************  FUNCTION  *********************/
CMRProjectActionBlock& CMRProjectAction::addSubBlock ( string loopDescr )
{
	CMRProjectActionBlock * tmpBlock = new CMRProjectActionBlock(&context);
	tmpBlock->loopDescr = loopDescr;
	return *tmpBlock;
}

/*******************  FUNCTION  *********************/
CMRProjectAction& CMRProject::addAction ( string name, string descr )
{
	CMRProjectAction * tmp = new CMRProjectAction(&rootContext,name,descr);
	actions.push_back(tmp);
	return *tmp;
}

/*******************  FUNCTION  *********************/
int main(int argc,char ** argv)
{
	//string value = "v'_{l} = \\frac{\\sum_{k}{v_{i,j,k} * D_{l,k}}}{d}";
	//string value = "d = \\sum_{l=1}^{9}{D_{i,j,k}}";
	//string value = "f_{ eq } = (1.0 + (3 * p) + ((9/2) * p^2 - (3/2) * v^2)) * W_{k} * d";
// 	string value = "D'_{i,j} = D_{i+1,j+2}+\\sum_{l=1}^{9}{D_{i,j,k}}";
	if (argc == 3)
	{
		string dep= argv[1];
		string value = argv[2];
		CMRLatexFormulas f;
		/*CMRLatexEntity * entity = cmrParseLatexEntity(value,start);
		if (entity != NULL)
			cmrPrintLatexEntity(*entity,0,0);*/
		cmrParseLatexFormula(f,value);
		cmrPrintFormula(f,0);
		
		printf("================================================\n");

		CMRLatexEntity entity;
		entity.name = "f";
		entity.subscriptTotalValue = "eq";
		entity.print(0,0);
		printf(" => matching : %d\n",cmrEntityExactMatch(*f.childs[0],entity));
		
		printf("================================================\n");
		
		CMREqDepMatrix depMatrix;
		//CMREntityCellMatching match = cmrEntityCellMatch(*f[0],"D");
		//match.printDebug();
		//depMatrix.markAccess(match.dx,match.dy,match.write);
		cmrExtractDeps(depMatrix,f,dep);
		printf("=============== DEP MATRIX ON %s ===============\n",dep.c_str());
		depMatrix.printDebug();
		printf("================================================\n");
	} else {
		///////////////////////////////////////////////////////////////////////////////////////////////////
		CMREntityConstant cst("A_{eq,i}^{2*4}","toto");
		cst.addIndice("k",CMR_CAPTURE_REQUIRED);
		//cst.loadValues("1.1",0);
		//cst.loadValues("1.1 ; 2.2 ; 3.3",1);
		cst.loadValues("1.1 ; 1.2 ; 1.3 ; 1.4 \\\\ 2.1 ; 2.2 ; 2.3 ; 2.4 \\\\ 3.1 ; 3.2 ; 3.3 ; 3.4",2);
		cst.printDebug();
		cst.printCPPCode();
		
		CMRLatexFormulas f;
		cmrParseLatexFormula(f,"A^2_{eq,i+1,4,j,k}");
		
		CMRIndiceCaptureMap capture;
		printf("Matching : %d\n",cst.match(*f.childs[0],capture));
		for (CMRIndiceCaptureMap::iterator it = capture.begin() ; it != capture.end() ; ++it)
		{
			printf("Capture %s => %s\n",it->first.c_str(),it->second.c_str());
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////
		CMRProjectVariable var("D_{i,j,k}","Directions","int");
		var.addDim(9,"k",1);
		var.printDebug();
		var.printCPPCode();

		///////////////////////////////////////////////////////////////////////////////////////////////////
		CMRProjectIterator iter("k","k",1,9);
		iter.printDebug();
		iter.printCPPCode();
		
		///////////////////////////////////////////////////////////////////////////////////////////////////
		CMRProjectEquation eq("d_{i,j}","density","\\sum_k{D_{i,j,k}}");
		eq.printDebug();
		CMRLatexEntity * term = eq.extractNextInnerLoop();
		if (term == NULL)
		{
			cout << "None" << endl;
		} else {
			cout << "Capture inner loop : " << term->totalValue << endl;
		}
		convertLoopToEq(term)->printDebug();
		
		///////////////////////////////////////////////////////////////////////////////////////////////////
		//assemble into project
		cout << "===================================================" << endl;
		CMRProject project;
		CMREntityConstant & cst2 = project.addConstant("A_{eq}","toto");
		cst2.addIndice("k",CMR_CAPTURE_REQUIRED);
		//cst2.loadValues("1.1",0);
		cst2.loadValues("1.1 ; 2.2 ; 3.3 ; 4.4 ; 5.5 ; 6.6 ; 7.7 ; 8.8 ; 9.9",1);
		//cst2.loadValues("1.1 ; 1.2 ; 1.3 ; 1.4 \\\\ 2.1 ; 2.2 ; 2.3 ; 2.4 \\\\ 3.1 ; 3.2 ; 3.3 ; 3.4",2);
		
		CMRProjectVariable & var2 = project.addvariable("D_{i,j,k}","Directions","int");
		var2.addDim(9,"k",1);
		
		CMRProjectAction & ac = project.addAction("test1");
		ac.addEquation("d_{i,j}","density","4+\\sum_k{D_{i,j,k}*A_{eq,k}}");
	}

	return 0;
}
