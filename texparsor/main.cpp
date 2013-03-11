#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <map>
#include "CMRLatexEntity.h"
#include "CMRParsorBasics.h"
#include "CMRTexParsor.h"
#include "CMREqDepMatrix.h"
#include "CMREntityMatching.h"

/********************  NAMESPACE  *******************/
using namespace std;

/*********************  TYPES  **********************/
typedef std::vector<std::string> CMREntityIndiceVector;
typedef std::vector<bool> CMREntityIndiceCaptureVector;
typedef std::vector<int> CMRConstantDimensionsVector;
typedef std::vector<double> CMRConstantValueVector;
typedef std::map<std::string,std::string> CMRIndiceCaptureMap;

/*********************  CLASS  **********************/
class CMREntity
{
	public:
		CMREntity(const std::string & latexName,const std::string & longName);
		virtual ~CMREntity(void);
		void addIndice(const std::string & name,bool capture = false);
		void printDebug(void) const;
		bool match(CMRLatexEntity & entity,CMRIndiceCaptureMap & capture);
	protected:
		void applyLatexName(const std::string & latexName);
	public:
		std::string latexName;
		std::string shortName;
		std::string longName;
		std::string exponent;
		CMREntityIndiceVector indices;
		CMREntityIndiceCaptureVector indicesCapture;
};

/*********************  CLASS  **********************/
//<param short_name="R" long_name="relax_param" type="double" unit="??" estimated_min="0" estimated_max="1.3" value="1" comment=""/>
class CMREntityConstant : public CMREntity
{
	public:
		CMREntityConstant(const std::string & latexName,const std::string & longName);
		void addDimension(int size);
		void loadValues(const std::string & data);
	public:
		std::string unit;
		std::string estimatedMin;
		std::string estimatedMax;
		std::string comment;
		CMRConstantDimensionsVector dims;
		CMRConstantValueVector values;
};

/*********************  CLASS  **********************/
class CMRProject
{
	
};

/*******************  FUNCTION  *********************/
CMREntity::CMREntity ( const string& latexName, const string& longName )
{
	this->latexName = latexName;
	this->longName = longName;
	this->applyLatexName(latexName);
}

/*******************  FUNCTION  *********************/
CMREntity::~CMREntity ( void )
{
	//nothing to do
}

/*******************  FUNCTION  *********************/
void CMREntity::addIndice ( const string& name ,bool capture)
{
	indices.push_back(name);
	if (name == "i" || name == "j")
	{
		printf("Caution, automatic capture for i/j\n");
		capture = true;
	}
	indicesCapture.push_back(capture);
}

/*******************  FUNCTION  *********************/
void CMREntity::applyLatexName ( const string& latexName )
{
	//vars
	CMRLatexFormulas f;

	//errors
	assert(latexName.empty() == false);

	//parse
	cmrParseLatexFormula(f,latexName);
	
	//check only one element and extract it
	assert(f.childs.size() == 1);
	CMRLatexEntity * entity = f.childs[0];
	
	//get name and exponent
	this->shortName = entity->name;
	this->exponent = entity->subscriptTotalValue;
	
	//extract subscript info
	if (entity->subscript.childs.empty() == false)
	{
		if (entity->subscript.childs[0]->name == "\\COMMA_GROUP")
		{
			for (CMRLatexFormulasList::iterator it = entity->subscript.childs[0]->params.begin() ; it != entity->subscript.childs[0]->params.end() ; ++it)
				this->addIndice((*it)->string,false);
		} else {
			this->addIndice(entity->subscriptTotalValue,false);
		}
	}
}

/*******************  FUNCTION  *********************/
void CMREntity::printDebug ( void ) const
{
	printf("Entity : \n");
	printf("    - latexName : %s\n",latexName.c_str());
	printf("    - shortName : %s\n",shortName.c_str());
	printf("    - longName  : %s\n",longName.c_str());
	printf("    - exponent  : %s\n",exponent.c_str());
	printf("    - indices   : \n");
	for (int i = 0 ; i < indices.size() ; i++)
		printf("           * %s [%d]\n",indices[i].c_str(),indicesCapture[i]);
}

/*******************  FUNCTION  *********************/
bool CMREntity::match ( CMRLatexEntity& entity, CMRIndiceCaptureMap& capture )
{
	if (entity.name != shortName || entity.subscriptTotalValue == exponent)
		return false;
	
	CMRLatexFormulasList tmp = entity.getIndices();
	if (tmp.size() == indices.size())
	{
		//capture
		for (int i = 0 ; i < tmp.size() ; i++)
		{
			if (indicesCapture[i])
			{
				capture[indices[i]] = tmp[i]->string;
			} else {
				if (tmp[i]->string != indices[i])
				{
					fprintf(stderr,"Caution, not same indices on %s for matching %s\n",entity.getString().c_str(),latexName.c_str());
					return false;
				}
			}
		}
		return true;
	} else {
		fprintf(stderr,"Caution, not same indices on %s for matching %s\n",entity.getString().c_str(),latexName.c_str());
		return false;
	}
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
		CMREntity cst("A_{eq,i}^{2*4}","toto");
		cst.addIndice("k",true);
		cst.printDebug();
		
		CMRLatexFormulas f;
		cmrParseLatexFormula(f,"A_{eq,i+1,4}");
		
		CMRIndiceCaptureMap capture;
		printf("Matching : %d\n",cst.match(*f.childs[0],capture));
		for (CMRIndiceCaptureMap::iterator it = capture.begin() ; it != capture.end() ; ++it)
		{
			printf("Capture %s => %s\n",it->first.c_str(),it->second.c_str());
		}
	}

	return 0;
}
