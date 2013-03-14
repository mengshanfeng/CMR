#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <map>
#include <sstream>
#include "CMRLatexEntity.h"
#include "CMRParsorBasics.h"
#include "CMRTexParsor.h"
#include "CMREqDepMatrix.h"
#include "CMREntityMatching.h"
#include "CMRProjectEntity.h"
#include "CMRProjectConstant.h"

/********************  NAMESPACE  *******************/
using namespace std;

/*********************  TYPES  **********************/
typedef std::vector<CMREntityConstant*> CMRProjectConstantVector;

/*********************  CLASS  **********************/
class CMRProject
{
	public:
		~CMRProject(void);
	private:
		CMRProjectConstantVector constants;
};

/*********************  CLASS  **********************/
class CMRProjectEquation : public CMREntity
{
	public:
		CMRProjectEquation ( const string& latexName, const string& longName , const string & compute);
		void printDebug(void) const;
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
	}

	return 0;
}
