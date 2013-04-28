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
#include <sstream>
#include "CMRProjectConstant.h"
#include "../parsor/CMRParsorBasics.h"
#include "CMRGenCode.h"

/********************  NAMESPACE  *******************/
using namespace std;

/*******************  FUNCTION  *********************/
CMRProjectConstant::CMRProjectConstant ( const string& latexName, const string& longName) 
	: CMRProjectEntity ( latexName, longName )
{
}

/*******************  FUNCTION  *********************/
void CMRProjectConstant::loadValues ( const string& data, int dimensions )
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
			stringstream err;
			err << "Unsupported constent dimension : " << dimensions << endl;
			throw CMRLatexException(err.str());
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectConstant::loadValuesScalar ( const string& data )
{
	//errors
	if(data.empty())
		throw CMRLatexException("Invalid empty data to be loaded as scalar !");
	
	//error
	CMRStringVector vs1 = cmrStringSplit(data,";");
	CMRStringVector vs2 = cmrStringSplit(data,"\\\\");
	if (vs1.size() > 1 || vs2.size() > 1)
		throw CMRLatexException("Caution, you say scalar but provide vector or matrix as data !");
	
	values.push_back(data);
}

/*******************  FUNCTION  *********************/
void CMRProjectConstant::loadValuesVector ( const string& data )
{
	//errors
	if(data.empty())
		throw CMRLatexException("Invalid empty data to be loaded as scalar !");
	
	//error
	CMRStringVector vs2 = cmrStringSplit(data,"\\\\");
	if (vs2.size() > 1)
		throw CMRLatexException("Caution, you say vector but provide matrix as data !");
	
	//split
	CMRStringVector vs = cmrStringSplit(data,";");
	addDimension(vs.size());
	
	//push all
	for (CMRStringVector::const_iterator it = vs.begin() ; it != vs.end() ; ++it)
		values.push_back(*it);
	
// 	if (values.size() == 1)
// 		fprintf(stderr,"Warning, you get a unique 0.0 value for a vector, maybe this is a mistake !\n");
}

/*******************  FUNCTION  *********************/
void CMRProjectConstant::loadValuesMatrix ( const string& data )
{
	//vars
	int dim1 = -1;
	int dim2 = -1;
	
	//errors
	if(data.empty())
		throw CMRLatexException("Invalid empty data to be loaded as scalar !");
	
	CMRStringVector ms = cmrStringSplit(data,"\\\\");
	dim1 = ms.size();
	
	for (CMRStringVector::const_iterator it = ms.begin() ; it != ms.end() ; ++it)
	{
		CMRStringVector vs = cmrStringSplit(*it,";");
		if (dim2 == -1)
		{
			dim2 = vs.size();
		} else if(dim2 != vs.size()) {
			throw CMRLatexException("Caution you prides lines which do not have the same size !");
		}
		for (CMRStringVector::const_iterator it = vs.begin() ; it != vs.end() ; ++it)
			values.push_back(*it);
	}
	
	addDimension(dim2);
	addDimension(dim1);
	
// 	if (values.size() == 1 && values[0] == 0.0)
// 		fprintf(stderr,"Warning, you get a unique 0.0 value for a matrix, maybe this is a mistake !\n");
}

/*******************  FUNCTION  *********************/
void CMRProjectConstant::addDimension ( int size )
{
	dims.push_back(size);
	switch(dims.size())
	{
		case 1:
			addIndice("\\cmr{cstid}{i}",CMR_CAPTURE_REQUIRED);
			break;
		case 2:
			addIndice("\\cmr{cstid}{j}",CMR_CAPTURE_REQUIRED);
			break;
		default:
			stringstream err;
			err << "Unsupported constent dimension : " << size << endl;
			throw CMRLatexException(err.str());
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectConstant::printDebug ( std::ostream & out ) const
{
	//default
	CMRProjectEntity::printDebug(out);
	
	//dims
	out << "    - dims       : " << dims.size() << " : [ ";
	for (size_t i = 0 ; i < dims.size() ; i++)
	{
		if ( i > 0 )
			out << ", ";
		out << dims[i];
	};
	out << " ]" << endl;
	
	//values
	out << "    - values     :";
	for (size_t i = 0 ; i < values.size() ; i++)
	{
		if ( i > 0 )
			out << ", ";
		out << values[i];
	}
	out << endl;
}

/*******************  FUNCTION  *********************/
void CMRProjectConstant::genDefinitionCCode ( ostream& out, const CMRProjectContext& context ) const
{
	//check errors
	assert(dims.size() <= 2);
	
	//do it depend on vectors or scalar form
	if (dims.size() > 0)
	{
		out << "//Definition of constant " << getShortName() << endl;
		out << "static const float TMP_VALUE_" << getLongName();
		for (size_t i = 0 ; i < dims.size() ; i++)
			out << "[" << dims[i] << "]";
		out << "=";
		out << "{";
		for (size_t i = 0 ; i < values.size() ; i++)
		{
			if (dims.size()==2 && i % dims[0] == 0 && i > 0)
				out << "},";
			if (dims.size()==2 && i % dims[0] == 0 && i < values.size() - 1)
				out << "{";
			out << values[i] << ",";
		}
		if (dims.size() == 2)
			out << "}";
		out << "};";
		out << endl;
		
		if (dims.size() == 1)
			out << "const CMRMathVector " << getLongName() << "(TMP_VALUE_" << getLongName() << ","<< dims[0] <<");" << endl;
		else if (dims.size() == 2)
			out << "const CMRMathMatrix " << getLongName() << "(TMP_VALUE_" << getLongName() << ","<< dims[0] << ","<< dims[1] << ");" << endl;
	} else {
		out << "const float " << getLongName() << " = " << values[0] << ";" << endl;
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectConstant::genUsageCCode ( ostream& out, const CMRProjectContext& context, const CMRLatexEntity2& entity, bool write ) const
{
	//extract matching
	CMRProjectCaptureMap capture;
	
	//error
	if (write)
		throw CMRLatexException("Constant cannot be used to write in !");
	
	//extract matching
	this->capture(entity,capture);
	
	//gen usage
	out << getLongName();
	
// 	for (CMRIndiceCaptureMap::iterator it = capture.begin() ; it != capture.end() ; ++it)
// 		cerr << "==> " << it->first << " => " << it->second << endl;
	
	//matrix
	if (dims.size() >= 2)
	{
		assert(capture.find("\\cmr{cstid}{j}") != capture.end());
		out << "[ ";
		cmrGenEqCCode(out,context, *capture["\\cmr{cstid}{j}"]) << "]";
	}
	
	//vector
	if (dims.size() >= 1)
	{
		assert(capture.find("\\cmr{cstid}{i}") != capture.end());
		out << "[ ";
		cmrGenEqCCode(out,context, *capture["\\cmr{cstid}{i}"]) << "]";
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectConstant::printValues ( ostream& out ) const
{
	switch(dims.size())
	{
		case 0:
			out << values[0];
			break;
		case 1:
			for (int i = 0 ; i < dims[0] ; i++)
				out << values[i] << " ";
			break;
		case 2:
			for (int j = 0 ; j < dims[1] ; j++)
			{
				for (int i = 0 ; i < dims[0] ; i++)
					out << values[dims[0]*j+i] << " ";
				out << endl;
			}
			break;
		default:
			throw CMRLatexException("Dims > 2 not suppported !");
	}
}
