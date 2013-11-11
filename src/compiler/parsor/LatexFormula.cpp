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
// #include "LatexEntity.h"
#include "ParsorBasics.h"
#include "LatexParsorContext.h"
#include "LatexFormula.h"

/**********************  USING  *********************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
LatexFormulas::LatexFormulas ( void )
{

}

/*******************  FUNCTION  *********************/
LatexFormulas::LatexFormulas ( const string& value )
{
	this->parse(value);
}

/*******************  FUNCTION  *********************/
LatexFormulas::LatexFormulas(const LatexFormulas& orig)
{
	this->copy(orig);
}

/*******************  FUNCTION  *********************/
void LatexFormulas::parse ( const string& value )
{
	//vars
	LatexParsorContext context(value);
	
	//do it
	parse(context);
}

/*******************  FUNCTION  *********************/
void LatexFormulas::parse ( LatexParsorContext& context )
{
	//reset content
	clear();

	//loop on all
	while (!context.isEnd())
	{
		LatexEntity * e = new LatexEntity;
		e->parse(context);
		this->push_back(e);
	}
}

/*******************  FUNCTION  *********************/
string LatexFormulas::getString ( void ) const
{
	stringstream tmp;
	tmp << *this;
	return tmp.str();
}

/*******************  FUNCTION  *********************/
ostream& operator<< ( ostream& out, const LatexFormulas& value )
{
	if (value.empty())
	{
		//in case of error
		out << "!!! EMPTY LATEX FORMULA !!!" << endl;
		//warning("Got undefined latex entity");
	} else {
		for (LatexFormulas::const_iterator it = value.begin() ; it != value.end() ; ++it)
		{
			assert(*it != NULL);
			out << **it;
		}
	}

	return out;
}

/*******************  FUNCTION  *********************/
void LatexFormulas::split ( LatexFormulasVector& formulas, const string& sep )
{
	int pos = 0;
	LatexFormulas * f = new LatexFormulas;
	
	//move until first one
// 	while (pos < size())
// 	{
// 		if ((*this)[pos]->getString() != sep)
// 			pos++;
// 		else
// 			break;
// 	}
	
	//cut
	for (int i = pos ; i < this->size() ; i++)
	{
		if ((*this)[i]->getString() == sep)
		{
			formulas.push_back(f);
			f = new LatexFormulas;
			delete (*this)[i];
		} else {
			f->push_back((*this)[i]);
		}
	}
	
	//finish
	formulas.push_back(f);
	
	LatexEntityVector::clear();
}

/*******************  FUNCTION  *********************/
void LatexFormulas::dumpAsXml ( ostream& out, int depth ) const
{
	out << cmrIndent(depth) << "<formula>" << endl;
	for (LatexFormulas::const_iterator it = begin() ; it != end() ; ++it)
		(*it)->dumpAsXml(out,depth+1);
	out << cmrIndent(depth) << "</formula>" << endl;
}

/*******************  FUNCTION  *********************/
void LatexFormulas::dumpAsTree ( ostream& out, int depth ) const
{
	out << cmrIndent(depth) << "formula:" << endl;
	for (LatexFormulas::const_iterator it = begin() ; it != end() ; ++it)
		(*it)->dumpAsTree(out,depth+1);
}

/*******************  FUNCTION  *********************/
LatexFormulas::~LatexFormulas ( void )
{
	this->clear();
}

/*******************  FUNCTION  *********************/
void LatexFormulas::clear ( void )
{
	for (LatexFormulas::iterator it = begin(); it != end() ; ++it)
		delete *it;
	LatexEntityVector::clear();
}

/*******************  FUNCTION  *********************/
bool LatexFormulas::isOnlyOneName ( void ) const
{
	if (size() == 1)
		return (*this)[0]->isOnlyOneName();
	else
		return false;
}

/*******************  FUNCTION  *********************/
bool LatexFormulas::isSimpleEntity ( void ) const
{
	if (size() != 1)
		return false;
	else if ((*this)[0]->getName() == "()")
		return false;
	else
		return true;
}

/*******************  FUNCTION  *********************/
void LatexFormulas::setExtraInfo(const string& key, void* value, bool allowOverride)
{
	//errors
	if (allowOverride == false && hasInfo(key))
		throw LatexException("Invalid override of information key on LatexEntity.");
	
	extraInfos[key] = value;
}

/*******************  FUNCTION  *********************/
void LatexFormulas::deleteInfo(const string& key, bool throwOnError)
{
	bool status = hasInfo(key);

	//errors
	if (throwOnError && status == false)
		throw LatexException("Invalid delete of information key on LatexEntity.");
	else if (status == false)
		return;
	
	extraInfos.erase(key);
}

/*******************  FUNCTION  *********************/
void* LatexFormulas::getExtraInfo(const string& key, bool throwOnError)
{
	bool status = hasInfo(key);

	//errors
	if (throwOnError && status == false)
		throw LatexException("Invalid read of information key on LatexEntity.");
	else if (status == false)
		return NULL;
	
	return extraInfos[key];
}

/*******************  FUNCTION  *********************/
bool LatexFormulas::hasInfo(const string& key) const
{
	return extraInfos.find(key) != extraInfos.end();
}

/*******************  FUNCTION  *********************/
void LatexFormulas::copy(const LatexFormulas& orig)
{
	//replace childs
	this->clear();
	for (int i = 0 ; i < orig.size() ; i++)
		this->push_back(new LatexEntity(*orig[i]));
	
	//setup exta info
	this->extraInfos.clear();
	this->extraInfos = orig.extraInfos;
}

/*******************  FUNCTION  *********************/
LatexFormulas& LatexFormulas::operator=(const LatexFormulas& orig)
{
	this->copy(orig);
	return *this;
}

};
