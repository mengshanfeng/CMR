/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include "CMRProjectConstant.h"
#include "CMRParsorBasics.h"

/********************  NAMESPACE  *******************/
using namespace std;

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
