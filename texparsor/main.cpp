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

/********************  ENUM  ************************/
enum CMRCaptureType
{
	CMR_CAPTURE_NONE,
	CMR_CAPTURE_REQUIRED,
	CMR_CAPTURE_OPTIONS
};

/*********************  TYPES  **********************/
typedef std::vector<std::string> CMREntityIndiceVector;
typedef std::vector<CMRCaptureType> CMREntityIndiceCaptureVector;
typedef std::vector<int> CMRConstantDimensionsVector;
typedef std::vector<double> CMRConstantValueVector;
typedef std::map<std::string,std::string> CMRIndiceCaptureMap;

/*********************  CLASS  **********************/
class CMREntity
{
	public:
		CMREntity(const std::string & latexName,const std::string & longName);
		virtual ~CMREntity(void);
		void addIndice(const std::string & name,CMRCaptureType capture = CMR_CAPTURE_NONE);
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
		int requiredIndices;
};

/*********************  CLASS  **********************/
//<param short_name="R" long_name="relax_param" type="double" unit="??" estimated_min="0" estimated_max="1.3" value="1" comment=""/>
class CMREntityConstant : public CMREntity
{
	public:
		CMREntityConstant( const string& latexName, const string& longName );
		void loadValues(const std::string & data,int dimensions);
		void printDebug(void) const;
		void printCPPCode(void) const;
	protected:
		void addDimension(int size);
		void loadValuesScalar(const std::string & data);
		void loadValuesVector(const std::string & data);
		void loadValuesMatrix(const std::string & data);
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
	this->requiredIndices = 0;
}

/*******************  FUNCTION  *********************/
CMREntity::~CMREntity ( void )
{
	//nothing to do
}

/*******************  FUNCTION  *********************/
void CMREntity::addIndice ( const string& name ,CMRCaptureType capture)
{
	indices.push_back(name);
	if (name == "i" || name == "j")
	{
		printf("Caution, automatic capture for i/j\n");
		capture = CMR_CAPTURE_REQUIRED;
	}
	indicesCapture.push_back(capture);
	if (capture != CMR_CAPTURE_OPTIONS)
		this->requiredIndices++;
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
				this->addIndice((*it)->string,CMR_CAPTURE_NONE);
		} else {
			this->addIndice(entity->subscriptTotalValue,CMR_CAPTURE_NONE);
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
	if (tmp.size() <= indices.size() & tmp.size() >= requiredIndices)
	{
		//capture
		for (int i = 0 ; i < tmp.size() ; i++)
		{
			if (indicesCapture[i] != CMR_CAPTURE_NONE)
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
CMREntityConstant::CMREntityConstant ( const string& latexName, const string& longName) 
	: CMREntity ( latexName, longName )
{
}

/*******************  FUNCTION  *********************/
void CMREntityConstant::loadValues ( const string& data, int dimensions )
{
	assert(values.empty());
	switch(dimensions)
	{
		case 0:
			loadValuesScalar(data);
			break;
		case 1:
			loadValuesVector(data);
			break;
		case 2:
			loadValuesMatrix(data);
			break;
		default:
			fprintf(stderr,"Unsupported constent dimension : %d\n",dimensions);
			abort();
	}
}

/*******************  FUNCTION  *********************/
void CMREntityConstant::loadValuesScalar ( const string& data )
{
	//errors
	assert(data.empty() == false);
	
	values.push_back(atof(data.c_str()));
}

/*******************  FUNCTION  *********************/
void CMREntityConstant::loadValuesVector ( const string& data )
{
	//errors
	assert(data.empty() == false);
	
	CMRStringVector vs = cmrStringSplit(data,";");
	addDimension(vs.size());
	
	for (CMRStringVector::const_iterator it = vs.begin() ; it != vs.end() ; ++it)
		values.push_back(atof(it->c_str()));
	
	if (values.size() == 1 && values[0] == 0.0)
		fprintf(stderr,"Warning, you get a unique 0.0 value for a vector, maybe this is a mistake !\n");
}

/*******************  FUNCTION  *********************/
void CMREntityConstant::loadValuesMatrix ( const string& data )
{
	//vars
	int dim1 = -1;
	int dim2 = -1;
	
	//errors
	assert(data.empty() == false);
	
	CMRStringVector ms = cmrStringSplit(data,"\\\\");
	dim1 = ms.size();
	
	for (CMRStringVector::const_iterator it = ms.begin() ; it != ms.end() ; ++it)
	{
		CMRStringVector vs = cmrStringSplit(*it,";");
		if (dim2 == -1)
		{
			dim2 = vs.size();
		} else {
			assert(dim2 == vs.size());
		}
		for (CMRStringVector::const_iterator it = vs.begin() ; it != vs.end() ; ++it)
			values.push_back(atof(it->c_str()));
	}
	
	addDimension(dim2);
	addDimension(dim1);
	
	if (values.size() == 1 && values[0] == 0.0)
		fprintf(stderr,"Warning, you get a unique 0.0 value for a matrix, maybe this is a mistake !\n");
}

/*******************  FUNCTION  *********************/
void CMREntityConstant::addDimension ( int size )
{
	dims.push_back(size);
	switch(dims.size())
	{
		case 1:
			addIndice("\\const_id_i",CMR_CAPTURE_OPTIONS);
			break;
		case 2:
			addIndice("\\const_id_j",CMR_CAPTURE_OPTIONS);
			break;
		default:
			abort();
			fprintf(stderr,"Not supported !");
	}
}

/*******************  FUNCTION  *********************/
void CMREntityConstant::printDebug ( void ) const
{
	CMREntity::printDebug();
	printf("    - dims      : %lu : [",dims.size());
	for (size_t i = 0 ; i < dims.size() ; i++)
		printf(" %d ,",dims[i]);
	printf("]\n");
	printf("    - values    :");
	for (size_t i = 0 ; i < values.size() ; i++)
		printf(" %f ,",values[i]);
	printf("\n");
}

/*******************  FUNCTION  *********************/
void CMREntityConstant::printCPPCode ( void ) const
{
	assert(dims.size() <= 2);
	if (dims.size() > 0)
	{
		cout << "//Definition of constant " << shortName << endl;
		cout << "static const float TMP_VALUE_" << longName;
		for (size_t i = 0 ; i < dims.size() ; i++)
			cout << "[" << dims[i] << "]";
		cout << "=";
		cout << "{";
		for (size_t i = 0 ; i < values.size() ; i++)
		{
			if (dims.size()==2 && i % dims[0] == 0 && i > 0)
				cout << "},";
			if (dims.size()==2 && i % dims[0] == 0 && i < values.size() - 1)
				cout << "{";
			cout << values[i] << ",";
		}
		if (dims.size() == 2)
			cout << "}";
		cout << "}";
		cout << endl;
		
		if (dims.size() == 1)
			cout << "const CMRMathVector " << longName << "(TMP_VALUE_" << longName << ","<< dims[0] <<");" << endl;
		else if (dims.size() == 2)
			cout << "const CMRMathMatrix " << longName << "(TMP_VALUE_" << longName << ","<< dims[0] << ","<< dims[1] << ");" << endl;
	} else {
		cout << "const float " << longName << "=" << values[0] << ";" << endl;
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
		CMREntityConstant cst("A_{eq,i}^{2*4}","toto");
		cst.addIndice("k",CMR_CAPTURE_REQUIRED);
		//cst.loadValues("1.1",0);
		//cst.loadValues("1.1 ; 2.2 ; 3.3",1);
		cst.loadValues("1.1 ; 1.2 ; 1.3 ; 1.4 \\\\ 2.1 ; 2.2 ; 2.3 ; 2.4 \\\\ 3.1 ; 3.2 ; 3.3 ; 3.4",2);
		cst.printDebug();
		cst.printCPPCode();
		
		CMRLatexFormulas f;
		cmrParseLatexFormula(f,"A_{eq,i+1,4,j,k}");
		
		CMRIndiceCaptureMap capture;
		printf("Matching : %d\n",cst.match(*f.childs[0],capture));
		for (CMRIndiceCaptureMap::iterator it = capture.begin() ; it != capture.end() ; ++it)
		{
			printf("Capture %s => %s\n",it->first.c_str(),it->second.c_str());
		}
	}

	return 0;
}
