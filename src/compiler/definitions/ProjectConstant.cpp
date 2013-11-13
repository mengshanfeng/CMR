/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <cassert>
#include <sstream>
#include "GenCode.h"
#include "ProjectConstant.h"
#include "parsor/ParsorBasics.h"
#include "parsor/LatexFormula.h"
#include "transformations/CMRModelBasedReplacement.h"

/********************  NAMESPACE  *******************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
ProjectConstant::ProjectConstant ( const string& latexName, const string& longName, const string& type ) 
	: ProjectEntity ( latexName, longName )
{
	this->type = type;
}

/*******************  FUNCTION  *********************/
void ProjectConstant::loadValues ( const string& data, int dimensions )
{
	assert(formulas.empty());
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
			throw LatexException(err.str());
	}
	transform();
}

/*******************  FUNCTION  *********************/
void ProjectConstant::loadValuesScalar ( const string& data )
{
	//errors
	if(data.empty())
		throw LatexException("Invalid empty data to be loaded as scalar !");
	
	//error
	StringVector vs1 = cmrStringSplit(data,";");
	StringVector vs2 = cmrStringSplit(data,"\\\\");
	if (vs1.size() > 1 || vs2.size() > 1)
		throw LatexException("Caution, you say scalar but provide vector or matrix as data !");
	
	formulas.push_back(LatexFormulas(data));
}

/*******************  FUNCTION  *********************/
void ProjectConstant::loadValuesVector ( const string& data )
{
	//errors
	if(data.empty())
		throw LatexException("Invalid empty data to be loaded as scalar !");
	
	//error
	StringVector vs2 = cmrStringSplit(data,"\\\\");
	if (vs2.size() > 1)
		throw LatexException("Caution, you say vector but provide matrix as data !");
	
	//split
	StringVector vs = cmrStringSplit(data,"&");
	addDimension(vs.size());
	
	//push all
	for (StringVector::const_iterator it = vs.begin() ; it != vs.end() ; ++it)
		formulas.push_back(LatexFormulas(*it));
	
// 	if (values.size() == 1)
// 		fprintf(stderr,"Warning, you get a unique 0.0 value for a vector, maybe this is a mistake !\n");
}

/*******************  FUNCTION  *********************/
void ProjectConstant::loadValuesMatrix ( const string& data )
{
	//vars
	int dim1 = -1;
	int dim2 = -1;
	
	//errors
	if(data.empty())
		throw LatexException("Invalid empty data to be loaded as scalar !");
	
	StringVector ms = cmrStringSplit(data,"\\\\");
	dim1 = ms.size();
	
	for (StringVector::const_iterator it = ms.begin() ; it != ms.end() ; ++it)
	{
		StringVector vs = cmrStringSplit(*it,"&");
		if (dim2 == -1)
		{
			dim2 = vs.size();
		} else if(dim2 != vs.size()) {
			throw LatexException("Caution you prides lines which do not have the same size !");
		}
		for (StringVector::const_iterator it = vs.begin() ; it != vs.end() ; ++it)
			formulas.push_back(LatexFormulas(*it));
	}
	
	addDimension(dim2);
	addDimension(dim1);
	
// 	if (values.size() == 1 && values[0] == 0.0)
// 		fprintf(stderr,"Warning, you get a unique 0.0 value for a matrix, maybe this is a mistake !\n");
}

/*******************  FUNCTION  *********************/
void ProjectConstant::addDimension ( int size )
{
	dims.push_back(size);
	switch(dims.size())
	{
		case 1:
			addIndice("\\cmr{cstid}{i}",CAPTURE_REQUIRED);
			break;
		case 2:
			addIndice("\\cmr{cstid}{j}",CAPTURE_REQUIRED);
			break;
		default:
			stringstream err;
			err << "Unsupported constent dimension : " << size << endl;
			throw LatexException(err.str());
	}
}

/*******************  FUNCTION  *********************/
void ProjectConstant::printDebug ( std::ostream & out ) const
{
	//default
	ProjectEntity::printDebug(out);
	
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
	for (size_t i = 0 ; i < formulas.size() ; i++)
	{
		if ( i > 0 )
			out << ", ";
		out << formulas[i];
	}
	out << endl;
}

/*******************  FUNCTION  *********************/
void ProjectConstant::genDefinitionCCode ( ostream& out, const ProjectContext& context ,int indent) const
{
	//check errors
	assert(dims.size() <= 2);
	
	//do it depend on vectors or scalar form
	if (dims.size() > 0)
	{
		out << "//Definition of constant " << getShortName() << endl;
		out << "static const " << type << " " << getLongName();
		for (size_t i = 0 ; i < dims.size() ; i++)
			out << "[" << dims[dims.size() - i - 1] << "]";
		out << "=";
		out << "{";
		for (size_t i = 0 ; i < formulas.size() ; i++)
		{
			if (dims.size()==2 && i % dims[0] == 0 && i > 0)
				out << "},";
			if (dims.size()==2 && i % dims[0] == 0 && i < formulas.size() - 1)
				out << "{";
			cmrGenEqCCode(out,context,formulas[i]);
			out << ",";
		}
		if (dims.size() == 2)
			out << "}";
		out << "};";
		out << endl;
		
// 		if (dims.size() == 1)
// 			out << "const CMRMathVector " << getLongName() << "(TMP_VALUE_" << getLongName() << ","<< dims[0] <<");" << endl;
// 		else if (dims.size() == 2)
// 			out << "const CMRMathMatrix " << getLongName() << "(TMP_VALUE_" << getLongName() << ","<< dims[0] << ","<< dims[1] << ");" << endl;
	} else {
		out << "const float " << getLongName() << " = ";
		cmrGenEqCCode(out,context,formulas[0]);
		out << ";" << endl;
	}
}

/*******************  FUNCTION  *********************/
void ProjectConstant::genUsageCCode ( ostream& out, const ProjectContext& context, const LatexEntity& entity, bool write ) const
{
	//extract matching
	ProjectCaptureMap capture;
	
	//error
	if (write)
		throw LatexException("Constant cannot be used to write in !");
	
	//extract matching
	this->capture(entity,capture);
	
	//gen usage
	out << getLongName();
	
// 	for (CMRIndiceCaptureMap::iterator it = capture.begin() ; it != capture.end() ; ++it)
// 		cerr << "==> " << it->first << " => " << it->second << endl;
	
	//not managed
	assert(dims.size() <= 2);
	
	//vector
	if (dims.size() >= 1)
	{
		assert(capture.find("\\cmr{cstid}{i}") != capture.end());
		out << "[ ";
		cmrGenEqCCode(out,context, *capture["\\cmr{cstid}{i}"]) << "]";
	}
	
	//matrix
	if (dims.size() >= 2)
	{
		assert(capture.find("\\cmr{cstid}{j}") != capture.end());
		out << "[ ";
		cmrGenEqCCode(out,context, *capture["\\cmr{cstid}{j}"]) << "]";
	}
}

/*******************  FUNCTION  *********************/
void ProjectConstant::printValues ( ostream& out ) const
{
	switch(dims.size())
	{
		case 0:
			out << formulas[0];
			break;
		case 1:
			for (int i = 0 ; i < dims[0] ; i++)
				out << formulas[i] << " ";
			break;
		case 2:
			for (int j = 0 ; j < dims[1] ; j++)
			{
				for (int i = 0 ; i < dims[0] ; i++)
					out << formulas[dims[0]*j+i] << " ";
				out << endl;
			}
			break;
		default:
			throw LatexException("Dims > 2 not suppported !");
	}
}

/*******************  FUNCTION  *********************/
void ProjectConstant::transform ( void )
{
	CMRModelBasedReplacement frac("\\frac{a}{b}","a/b");
	for (ConstantFormulaVector::iterator it = formulas.begin() ; it != formulas.end() ; ++it)
		transform(*it,frac);
}

/*******************  FUNCTION  *********************/
void ProjectConstant::transform ( LatexFormulas& formula, CMRModelBasedReplacement& action )
{
	for (LatexEntityVector::iterator it = formula.begin() ; it != formula.end() ; ++it)
		if (formula.hasInfo("cmrNoTranform") == false)
			this->transform(**it,action);
}

/*******************  FUNCTION  *********************/
void ProjectConstant::transform ( LatexEntity& entity, CMRModelBasedReplacement& action )
{
	action.apply(entity);
	
	LatexFormulasVector & indices = entity.getIndices();
	for (LatexFormulasVector::iterator it = indices.begin() ; it != indices.end() ; ++it)
		if ((*it)->hasInfo("cmrNoTranform") == false)
			this->transform(**it,action);
	
	LatexFormulasVector & exponents = entity.getExponents();
	for (LatexFormulasVector::iterator it = exponents.begin() ; it != exponents.end() ; ++it)
		if ((*it)->hasInfo("cmrNoTranform") == false)
			this->transform(**it,action);
	
	LatexFormulasVector & params = entity.getParameters();
	for (LatexFormulasVector::iterator it = params.begin() ; it != params.end() ; ++it)
		if ((*it)->hasInfo("cmrNoTranform") == false)
			this->transform(**it,action);
}

}
