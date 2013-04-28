/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_PROJECT_CODE_H
#define CMR_PROJECT_CODE_H

/********************  HEADERS  *********************/
#include <ostream>
#include "CMRProjectCodeTree.h"
#include "../parsor/CMRLatexFormula.h"

/*********************  TYPES  **********************/
class CMRProjectIterator;
class CMRProjectCodeEquation;
class CMRProjectCodeBlockNode;
class CMRProjectCodeIteratorLoop;

/********************  ENUM  ************************/
enum CMRProjectCodeType
{
	//usefull to build the root node
	CMR_PROJECT_CODE_NODE,
	//for eqs
	CMR_PROJECT_CODE_EQUATION,
	//loop on iterators
	CMR_PROJECT_CODE_ITERATOR,
};

/*********************  CLASS  **********************/
class CMRProjectLocalVariable : public CMRProjectEntity
{
	public:
		CMRProjectLocalVariable( const std::string& latexName, const std::string& longName, const std::string& type, const std::string& defaultValue = "0");
		virtual void genDefinitionCCode ( std::ostream& out, const CMRProjectContext& context ) const;
		virtual void genUsageCCode ( std::ostream& out, const CMRProjectContext& context, const CMRLatexEntity2& entity, bool write = false ) const;
	private:
		std::string type;
		CMRLatexFormulas2 defaultValue;
};

/*********************  TYPES  **********************/
class CMRProjectCodeEntry : public CMRProjectCodeTree<CMRProjectCodeEntry>
{
	public:
		virtual CMRProjectCodeType getType(void) const = 0;
		CMRProjectContext & getContext(void);
	protected:
		virtual void onParentChange ( CMRProjectCodeEntry * newParent );
	protected:
		CMRProjectContext context;
};

/*********************  CLASS  **********************/
class CMRProjectCodeNode : public CMRProjectCodeEntry
{
	public:
		CMRProjectCodeBlockNode& addSubBlock( CMRProjectCodeTreeInsert location = CMR_INSERT_LAST_CHILD );
		CMRProjectCodeEquation & addEquation(const std::string& latexName, const std::string& longName, const std::string& compute,CMRProjectCodeTreeInsert location = CMR_INSERT_LAST_CHILD);
		CMRProjectCodeIteratorLoop& addIteratorLoop( const std::string& iterator, CMRProjectCodeTreeInsert location = CMR_INSERT_LAST_CHILD );
		CMRProjectLocalVariable & addLocalVariable(const std::string & latexName, const std::string & longName,const std::string &type, const std::string & defaultValue, CMRProjectCodeTreeInsert location = CMR_INSERT_LAST_CHILD);
};

/*********************  CLASS  **********************/
class CMRProjectCodeBlockNode : public CMRProjectCodeNode
{
	public:
		virtual CMRProjectCodeType getType ( void ) const;
};

/*********************  CLASS  **********************/
class CMRProjectCodeEquation : public CMRProjectCodeEntry
{
	public:
		CMRProjectCodeEquation( const std::string& latexName, const std::string& compute );
		virtual CMRProjectCodeType getType ( void ) const;
	private:
		CMRLatexEntity2 output;
		CMRLatexFormulas2 formula;
};

/*********************  CLASS  **********************/
class CMRProjectCodeIteratorLoop : public CMRProjectCodeNode
{
	public:
		CMRProjectCodeIteratorLoop( const std::string& iterator );
		virtual CMRProjectCodeType getType ( void ) const;
	private:
		const CMRProjectIterator * it;
};

#endif //CMR_PROJECT_CODE_H
