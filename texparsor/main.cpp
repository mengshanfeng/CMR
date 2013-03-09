#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <cstring>

using namespace std;

/*********************  TYPES  **********************/
struct CMRLatexEntity;
typedef std::vector<CMRLatexEntity*> CMRLatexFormulas;
typedef std::vector<CMRLatexFormulas*> CMRLatexFormulasList;

/*********************  STRUCT  *********************/
struct CMRLatexEntity
{
	CMRLatexEntity(void);
	~CMRLatexEntity(void);
	string name;
	string parent;
	CMRLatexFormulas subscript;
	CMRLatexFormulas superscript;
	CMRLatexFormulasList params;
	int from;
	int to;
	string totalValue;
	string subscriptTotalValue;
	string superscriptTotalValue;
	string argsTotalValue;
};


/*******************  FUNCTION  *********************/
void cmrParseLatexFormula(CMRLatexFormulas & formula,const string & value);
void cmrPrintFormula(const CMRLatexFormulas & entity,int depth);

/*******************  FUNCTION  *********************/
void cmrParsorError(const string & value,int pos,const string & message="")
{
	if ( ! message.empty() )
		fprintf(stderr,"PARSING ERROR : %s\n",message.c_str());
	else
		fprintf(stderr,"PARGINS ERROR : \n");
	fprintf(stderr,"%s\n",value.c_str());
	for (int i = 0 ; i < value.size(); i++)
		fprintf(stderr,"%c",((i==pos)?'^':' '));
	fprintf(stderr,"\n");
	abort();
	
}

/*******************  FUNCTION  *********************/
void cmrPrintLatexEntity(const CMRLatexEntity & entity,int depth, int pos)
{
	printf(" - Entity [depth = %d, pos = %d]\n",depth,pos);
	printf("       * name        : %s\n",entity.name.c_str());
	printf("       * totalValue  : %s\n",entity.totalValue.c_str());
	printf("       * pos         : [%d - %d]\n",entity.from,entity.to);
	printf("       * subscript   : %s\n",entity.subscriptTotalValue.c_str());
	printf("       * superscript : %s\n",entity.superscriptTotalValue.c_str());
	printf("       * args        : %s\n",entity.argsTotalValue.c_str());
	printf("       * parent      : %s\n",entity.parent.c_str());
	cmrPrintFormula(entity.subscript,depth+1);
	cmrPrintFormula(entity.superscript,depth+1);
	for(CMRLatexFormulasList::const_iterator it = entity.params.begin();it!=entity.params.end();++it)
		cmrPrintFormula(**it,depth+1);
}

/*******************  FUNCTION  *********************/
void cmrPrintFormula(const CMRLatexFormulas & formula,int depth)
{
	int pos = 0;
	for(CMRLatexFormulas::const_iterator it = formula.begin();it!=formula.end();++it)
		cmrPrintLatexEntity(**it,depth,pos++);
}

/*******************  FUNCTION  *********************/
CMRLatexEntity::CMRLatexEntity ( void )
{
	this->from = 0;
	this->to = 0;
}

/*******************  FUNCTION  *********************/
CMRLatexEntity::~CMRLatexEntity ( void )
{
	for(CMRLatexFormulas::iterator it = subscript.begin();it!=subscript.end();++it)
		delete *it;
	for(CMRLatexFormulas::iterator it = superscript.begin();it!=superscript.end();++it)
		delete *it;
	for(CMRLatexFormulasList::iterator it = params.begin();it!=params.end();++it)
		delete *it;
}

/*******************  FUNCTION  *********************/
bool cmrIsAlphaNum(char value)
{
	return (value >= 'a' && value <= 'z')
		|| (value >= 'A' && value <= 'Z')
		|| (value >= '0' && value <= '9') || value == '.';
}

/*******************  FUNCTION  *********************/
bool cmrIsNum(char value)
{
	return (value >= '0' && value <= '9');
}

/*******************  FUNCTION  *********************/
bool cmrIsNumFloat(char value)
{
	return (value >= '0' && value <= '9') || value == '.';
}

/*******************  FUNCTION  *********************/
bool cmrIsOperator(char value)
{
	return (value == '=' || value == '*' || value == '+' || value == '-' || value == '/' || value == ',');
}

/*******************  FUNCTION  *********************/
bool cmrIsWhiteSpace(char value)
{
	return (value == ' ' || value == '\t');
}

/*******************  FUNCTION  *********************/
string cmrExtractName(const string & value,int & start)
{
	//vars
	string res;

	//errors
	assert(start >= 0);
	assert(start < value.size());
	
	//long names
	if (value[start] == '\\')
	{
		res+=value[start++];
		while(cmrIsAlphaNum(value[start]))
			res+=value[start++];
	} else if (cmrIsNum(value[start])) {
		while(cmrIsNumFloat(value[start]))
			res+=value[start++];
	} else if (cmrIsAlphaNum(value[start])){
		res = value[start++];
		while(value[start] == '\'')
			res+=value[start++];
	} else if (cmrIsOperator(value[start])) {
		res = value[start++];
	} else {
		cmrParsorError(value,start,"Invalid character while searching name, must be [a-zA-Z0-9.] or [+-=*/\\,].");
	}
	
	return res;
}

/*******************  FUNCTION  *********************/
string cmrExtractSubZone(const string & value, int & start,char open = '{',char close = '}')
{
	//vars
	string res;
	int cnt = 1;
	int curStart = start;
	
	//errors
	assert(start >= 0);
	assert(start < value.size());

	if (value[start] == open)
	{
		start++;
		while(cnt > 0)
		{
			if (value[start] == open)
				cnt++;
			else if (value[start] == close)
				cnt --;
			if (cnt > 0 && !cmrIsWhiteSpace(value[start]))
				res += value[start++];
			else
				start++;
			if (start > value.size())
				cmrParsorError(value,curStart,"Missing close.");
		}
	} else {
		res = cmrExtractName(value,start);
	}
	
	return res;
}

/*******************  FUNCTION  *********************/
void cmrExtractSubAndSuperScript(const string & value,int & start,CMRLatexEntity * entity)
{
	//vars
	string res;
	char mode = value[start];
	
	//errors
	assert(start >= 0);
	assert(start < value.size());
	assert(entity != NULL);
	
	//check it
	if (mode == '_')
	{
		start++;
		res = cmrExtractSubZone(value,start);
		entity->subscriptTotalValue = res;
		cmrParseLatexFormula(entity->subscript,res);
	} else if (mode == '^') {
		start++;
		res = cmrExtractSubZone(value,start);
		entity->superscriptTotalValue = res;
		cmrParseLatexFormula(entity->superscript,res);
	} else {
		assert(false);
	}
}

int cmrRequireParameters(const string & name,const string & value,int pos)
{
	if (name[0] != '\\')
	{
		return 0;
	} else if (name == "\\frac") {
		return 2;
	} else if (name == "\\sum") {
		return 1;
	} else {
		cmrParsorError(value,pos,"Invalid latex command.");
		return -1;
	}
}

/*******************  FUNCTION  *********************/
void cmrReplaceByCommaGroup(CMRLatexFormulas & formula)
{
	//vars
	bool hasComma = false;
	CMRLatexEntity * entity = NULL;

	for (CMRLatexFormulas::const_iterator it = formula.begin() ; it != formula.end() ; ++it)
		if ((*it)->name == ",")
			hasComma = true;
		
	//nothing to do
	if (hasComma == false)
		return;
	
	//create entity
	entity = new CMRLatexEntity;
	entity->name = "\\COMMA_GROUP";
	entity->totalValue = (*formula.begin())->parent;
	
	//create formulas
	CMRLatexFormulas * f = new CMRLatexFormulas;
	//fill
	for (CMRLatexFormulas::const_iterator it = formula.begin() ; it != formula.end() ; ++it)
	{
		if ((*it)->name == ",")
		{
			entity->params.push_back(f);
			f = new CMRLatexFormulas;
		} else {
			f->push_back(*it);
		}
	}
	entity->params.push_back(f);
	
	//replace in formula
	formula.clear();
	formula.push_back(entity);
}

/*******************  FUNCTION  *********************/
CMRLatexEntity * cmrParseLatexEntitySimple(const string & value,int & start)
{
	//vars
	int cur = start;
	CMRLatexEntity * res = NULL;
	int params = 0;

	//errors
	assert(start >= 0);

	//trivial
	if (start >= value.size())
		return NULL;
	
	//setup entity
	res = new CMRLatexEntity;
	//extract name
	res->parent = value;
	res->from = start;
	res->name = cmrExtractName(value,cur);
	//extract sub and superscript
	if (value[cur] == '_' || value[cur] == '^')
		cmrExtractSubAndSuperScript(value,cur,res);
	if (value[cur] == '_' || value[cur] == '^')
		cmrExtractSubAndSuperScript(value,cur,res);
	//form subgroups
	cmrReplaceByCommaGroup(res->subscript);
	//check for parameters
	params = cmrRequireParameters(res->name,value,cur);
	for (int i = 0 ; i < params ; i++)
	{
		string tmp = cmrExtractSubZone(value,cur);
		CMRLatexFormulas * f = new CMRLatexFormulas;
		cmrParseLatexFormula(*f,tmp);
		res->params.push_back(f);
		res->argsTotalValue += '{';
		res->argsTotalValue += tmp;
		res->argsTotalValue += '}';
		
	}
	
	res->to = cur;
	res->totalValue = value.substr(start,cur-start);
	
	//ok done
	start = cur;
	return res;
}

/*******************  FUNCTION  *********************/
CMRLatexEntity * cmrExtractSubGroup(const string & value,int & start)
{
	//vars
	CMRLatexEntity * res = NULL;

	//errors
	assert(value[start] == '(');
	
	//trivial
	if (start >= value.size())
		return NULL;
	
	//setup entity
	res = new CMRLatexEntity;
	res->name = '(';
	res->from = start;
	res->parent = value;
	res->argsTotalValue = cmrExtractSubZone(value,start,'(',')');
	res->to = start;
	CMRLatexFormulas * f = new CMRLatexFormulas;
	cmrParseLatexFormula(*f,res->argsTotalValue);
	res->params.push_back(f);
	
	return res;
}

/*******************  FUNCTION  *********************/
CMRLatexEntity * cmrParseLatexEntity(const string & value,int & start)
{
	if (value[start] == '(')
		return cmrExtractSubGroup(value,start);
	else
		return cmrParseLatexEntitySimple(value,start);
}

/*******************  FUNCTION  *********************/
void cmrSkipWhiteSpace(const string & value,int & cur)
{
	while (cmrIsWhiteSpace(value[cur]))
		cur++;
}

/*******************  FUNCTION  *********************/
void cmrParseLatexFormula(CMRLatexFormulas & formula,const string & value)
{
	CMRLatexEntity * entity;
	int cur = 0;
	cmrSkipWhiteSpace(value,cur);
	while ((entity = cmrParseLatexEntity(value,cur)) != NULL)
	{
		formula.push_back(entity);
		cmrSkipWhiteSpace(value,cur);
	}
}

/*******************  FUNCTION  *********************/
bool cmrEntityExactMatch(const CMRLatexEntity & e1,const CMRLatexEntity & e2,bool subsup = true)
{
	if (subsup)
		return (e1.name == e2.name && e1.subscriptTotalValue == e2.subscriptTotalValue && e1.superscriptTotalValue == e2.superscriptTotalValue);
	else
		return (e1.name == e2.name);
}

struct CMREntityCellMatching
{
	CMREntityCellMatching(bool matching = false);
	void printDebug(void) const;
	bool match;
	bool depInfo;
	int dx;
	int dy;
	bool write;
	bool out;
};

/*******************  FUNCTION  *********************/
CMREntityCellMatching::CMREntityCellMatching ( bool matching )
{
	this->match = matching;
	this->depInfo = false;
	this->dx = 0;
	this->dy = 0;
	this->write = false;
	this->out = false;
}

/*******************  FUNCTION  *********************/
void CMREntityCellMatching::printDebug ( void ) const
{
	printf(" - CellMatching : %d\n",match);
	if (depInfo)
		printf("      * vect    : [ %d , %d ]\n",dx,dy);
	else
		printf("      * vect    : [ ?? , ?? ]\n");
	printf("      * write   : %d\n",write);
	printf("      * out     : %d\n",out);
}

/*******************  FUNCTION  *********************/
const CMRLatexFormulas * cmrEntityExtractSubscriptParam(const CMRLatexEntity & entity,int id)
{
	if (entity.subscript.empty() || entity.subscript.size() > 1)
		return NULL;
	
	const CMRLatexEntity * e = entity.subscript[0];
	if (e->name != "\\COMMA_GROUP")
		return NULL;
	else if (e->params.size() <= id)
		return NULL;
	else
		return e->params[id];
}

/*******************  FUNCTION  *********************/
bool cmrIsSimpleOperator(const string & value)
{
	return (value == "+" || value == "-");
}

/*******************  FUNCTION  *********************/
int cmrApplyOp(int orig,char op,int value)
{
	switch(op)
	{
		case '+':
			orig+=value;
			break;
		case '-':
			orig-=value;
			break;
		default:
			assert(false);
	}
	
	return orig;
}

/*******************  FUNCTION  *********************/
int cmrFormulaExtractDelta(const CMRLatexFormulas * formula,const string & varname)
{
	int res = 0;
	char op = '+';
	for (CMRLatexFormulas::const_iterator it = formula->begin() ; it != formula->end() ; ++it)
	{
		const string & name = (*it)->name;
		if (name == varname)
		{
			assert(op != ' ');
			op = ' ';
		} else if (cmrIsSimpleOperator(name)) {
			assert(op == ' ' || (op == '+' && name == "-" ));
			op = name[0];
		} else if (cmrIsNum(name[0])) {
			res = cmrApplyOp(res,op,atoi(name.c_str()));
		} else {
			assert(false);
		}
	}
	return res;
}

/*******************  FUNCTION  *********************/
CMREntityCellMatching cmrEntityCellMatch(const CMRLatexEntity & entity,const string & name)
{
	CMREntityCellMatching res;
	const CMRLatexFormulas * fdx;
	const CMRLatexFormulas * fdy;
	
	if (entity.name == name)
	{
		res.match = true;
	} else if (entity.name == name + "'") {
		res.match = true;
		res.write = true;
		res.out = true;
	} else {
		return res;
	}

	//check deps
	fdx = cmrEntityExtractSubscriptParam(entity,0);
	fdy = cmrEntityExtractSubscriptParam(entity,1);
	assert(fdx != NULL);
	assert(fdy != NULL);
	
	res.dx = cmrFormulaExtractDelta(fdx,"i");
	res.dy = cmrFormulaExtractDelta(fdy,"j");
	res.depInfo = true;

	return res;
}

/*******************  FUNCTION  *********************/
struct CMREqDepMatrix
{
	CMREqDepMatrix(void);
	void markAccess(int dx,int dy,bool write);
	void printDebug(void);
	unsigned char access[11][11];
	int maxDepth;
};

/*******************  FUNCTION  *********************/
CMREqDepMatrix::CMREqDepMatrix(void)
{
	maxDepth = 0;
	memset(access,0,sizeof(access));
}

/*******************  FUNCTION  *********************/
void CMREqDepMatrix::markAccess ( int dx, int dy, bool write )
{
	assert(dx >= -5 && dx <= 5);
	assert(dy >= -5 && dy <= 5);
	if (write)
		access[dy+5][dx+5] |= 2;
	else
		access[dy+5][dx+5] |= 1;
	if (abs(dx) > maxDepth) maxDepth = abs(dx);
	if (abs(dy) > maxDepth) maxDepth = abs(dy);
}

/*******************  FUNCTION  *********************/
void CMREqDepMatrix::printDebug ( void )
{
	for (int j = 5-maxDepth ; j <= 5+maxDepth ; j++)
	{
		for (int i = 5-maxDepth ; i <= 5+maxDepth ; i++)
				printf("%d  ",access[j][i]);
		printf("\n");
	}		
}

/*******************  FUNCTION  *********************/
void cmrExtractDeps(CMREqDepMatrix & matrix,const CMRLatexFormulas & f,const string & varname);

/*******************  FUNCTION  *********************/
void cmrExtractDeps(CMREqDepMatrix & matrix,const CMRLatexEntity & f,const string & varname)
{
	CMREntityCellMatching match = cmrEntityCellMatch(f,varname);
	if (match.match)
	{
		printf("=> Check %s : %s\n",f.name.c_str(),f.totalValue.c_str());
		match.printDebug();
		matrix.markAccess(match.dx,match.dy,match.write);
	}
	//loop on sublists
	cmrExtractDeps(matrix,f.subscript,varname);
	cmrExtractDeps(matrix,f.superscript,varname);
	for (CMRLatexFormulasList::const_iterator it = f.params.begin() ; it != f.params.end() ; ++it)
		cmrExtractDeps(matrix,**it,varname);
}

/*******************  FUNCTION  *********************/
void cmrExtractDeps(CMREqDepMatrix & matrix,const CMRLatexFormulas & f,const string & varname)
{
	for (CMRLatexFormulas::const_iterator it = f.begin() ; it != f.end() ; ++it)
		cmrExtractDeps(matrix,**it,varname);
}

/*******************  FUNCTION  *********************/
int main(int argc,char ** argv)
{
	//string value = "v'_{l} = \\frac{\\sum_{k}{v_{i,j,k} * D_{l,k}}}{d}";
	//string value = "d = \\sum_{l=1}^{9}{D_{i,j,k}}";
	//string value = "f_{ eq } = (1.0 + (3 * p) + ((9/2) * p^2 - (3/2) * v^2)) * W_{k} * d";
// 	string value = "D'_{i,j} = D_{i+1,j+2}+\\sum_{l=1}^{9}{D_{i,j,k}}";
	assert(argc == 3);
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
	cmrPrintLatexEntity(entity,0,0);
	printf(" => matching : %d\n",cmrEntityExactMatch(*f[0],entity));
	
	printf("================================================\n");
	
	CMREqDepMatrix depMatrix;
	//CMREntityCellMatching match = cmrEntityCellMatch(*f[0],"D");
	//match.printDebug();
	//depMatrix.markAccess(match.dx,match.dy,match.write);
	cmrExtractDeps(depMatrix,f,dep);
	printf("=============== DEP MATRIX ON %s ===============\n",dep.c_str());
	depMatrix.printDebug();
	printf("================================================\n");

	return 0;
}
