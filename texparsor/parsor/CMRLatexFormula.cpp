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
#include "CMRLatexEntity2.h"
#include "CMRLatexFormula.h"
#include "CMRParsorBasics.h"
#include "CMRLatexParsorContext.h"

/**********************  USING  *********************/
using namespace std;

/*******************  FUNCTION  *********************/
CMRLatexFormulas2::CMRLatexFormulas2 ( void )
{

}

/*******************  FUNCTION  *********************/
CMRLatexFormulas2::CMRLatexFormulas2 ( const string& value )
{
	this->parse(value);
}

/*******************  FUNCTION  *********************/
void CMRLatexFormulas2::parse ( const string& value )
{
	//vars
	CMRLatexParsorContext context(value);
	
	//do it
	parse(context);
}

/*******************  FUNCTION  *********************/
void CMRLatexFormulas2::parse ( CMRLatexParsorContext& context )
{
	//reset content
	clear();

	//loop on all
	while (!context.isEnd())
	{
		CMRLatexEntity2 * e = new CMRLatexEntity2;
		e->parse(context);
		this->push_back(e);
	}
}

/*******************  FUNCTION  *********************/
string CMRLatexFormulas2::getString ( void ) const
{
	stringstream tmp;
	tmp << *this;
	return tmp.str();
}

/*******************  FUNCTION  *********************/
ostream& operator<< ( ostream& out, const CMRLatexFormulas2& value )
{
	if (value.empty())
	{
		//in case of error
		out << "!!! EMPTY LATEX FORMULA !!!" << endl;
		//warning("Got undefined latex entity");
	} else {
		for (CMRLatexFormulas2::const_iterator it = value.begin() ; it != value.end() ; ++it)
		{
			assert(*it != NULL);
			out << **it;
		}
	}

	return out;
}

/*******************  FUNCTION  *********************/
void CMRLatexFormulas2::split ( CMRLatexFormulasVector2& formulas, const string& sep )
{
	int pos = 0;
	CMRLatexFormulas2 * f = new CMRLatexFormulas2;
	
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
			f = new CMRLatexFormulas2;
			delete (*this)[i];
		} else {
			f->push_back((*this)[i]);
		}
	}
	
	//finish
	formulas.push_back(f);
	
	CMRLatexEntityVector2::clear();
}

/*******************  FUNCTION  *********************/
void CMRLatexFormulas2::dumpAsXml ( ostream& out, int depth ) const
{
	out << cmrIndent(depth) << "<formula>" << endl;
	for (CMRLatexFormulas2::const_iterator it = begin() ; it != end() ; ++it)
		(*it)->dumpAsXml(out,depth+1);
	out << cmrIndent(depth) << "</formula>" << endl;
}

/*******************  FUNCTION  *********************/
void CMRLatexFormulas2::dumpAsTree ( ostream& out, int depth ) const
{
	out << cmrIndent(depth) << "formula:" << endl;
	for (CMRLatexFormulas2::const_iterator it = begin() ; it != end() ; ++it)
		(*it)->dumpAsTree(out,depth+1);
}

/*******************  FUNCTION  *********************/
CMRLatexFormulas2::~CMRLatexFormulas2 ( void )
{
	this->clear();
}

/*******************  FUNCTION  *********************/
void CMRLatexFormulas2::clear ( void )
{
	for (CMRLatexFormulas2::iterator it = begin(); it != end() ; ++it)
		delete *it;
	CMRLatexEntityVector2::clear();
}

/*******************  FUNCTION  *********************/
bool CMRLatexFormulas2::isOnlyOneName ( void ) const
{
	if (size() == 1)
		return (*this)[0]->isOnlyOneName();
	else
		return false;
}
