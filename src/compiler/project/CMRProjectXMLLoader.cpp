/********************  HEADERS  *********************/
#include <cassert>
#include "CMRProject.h"
#include "CMRXmlDoc.h"
#include "CMRProjectXMLLoader.h"
#include "../common/Debug.h"
#include "../definitions/CMRProjectAlias.h"

/**********************  USING  *********************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  CONSTS  *********************/
const char * CMR_NODE_ROOT = "cmr";
const char * CMR_NODE_ELEMNTS = "elements";
const char * CMR_NODE_CONSTS = "consts";
const char * CMR_NODE_CONST = "const";
const char * CMR_NODE_MESH = "mesh";
const char * CMR_NODE_MESH_VAR = "var";
const char * CMR_NODE_EXTRA_DIMS = "extradims";
const char * CMR_NODE_EXTRA_DIM = "extradim";
const char * CMR_NODE_ITERATORS = "iterators";
const char * CMR_NODE_ITERATOR = "iterator";
const char * CMR_NODE_DEFINITIONS = "defs";
const char * CMR_NODE_DEFINITION = "def";
const char * CMR_NODE_MATHSTEP = "mathstep";
const char * CMR_NODE_DEF_PARAM = "defparameter";
const char * CMR_NODE_DECL_VAR = "declvar";
const char * CMR_NODE_CELL_ACTIONS = "cellactions";
const char * CMR_NODE_CELL_ACTION = "cellaction";
const char * CMR_NODE_FOREACH = "foreach";
const char * CMR_NODE_ALIAS = "alias";
const char * CMR_NODE_CCODE = "ccode";
//properties
const char * CMR_PROP_MATHNAME = "mathname";
const char * CMR_PROP_LONGNAME = "longname";
const char * CMR_PROP_DIMS = "dims";

/*******************  FUNCTION  *********************/
void CMRProjectXMLLoader::load ( CMRProject2& project, std::string& filename )
{
	//Errors
	assert(filename.empty() == false);

	//Open XML
	CMRXmlDoc doc;
	doc.loadFromFile(filename);
	//doc.validateWithDTD();
	
	//extract root element
	CMRXmlNode rootNode = doc.getRootNode();
	assert(rootNode.isNamed(CMR_NODE_ROOT));
	
	//load content
	load(project,rootNode);
}

/*******************  FUNCTION  *********************/
void CMRProjectXMLLoader::load ( CMRProject2& project, CMRXmlNode& rootNode )
{
	//errors
	assert(rootNode.isValid() && rootNode.isNamed(CMR_NODE_ROOT));
	
	CMRXmlNode projectNode = rootNode.getUniqChild("project");
	if (projectNode.isValid())
		loadProjectInfo(project,projectNode);
	
	CMRXmlNode elementsNode = rootNode.getUniqChild(CMR_NODE_ELEMNTS);
	if (elementsNode.isValid())
		loadElements(project,elementsNode);
}

/*******************  FUNCTION  *********************/
void CMRProjectXMLLoader::loadProjectInfo ( CMRProject2& project, CMRXmlNode& node )
{
	//errors
	assert(node.isValid());
	assert(node.isNamed("project"));

	CMRXmlNode headers = node.getUniqChild("headers");
	if (headers.isValid())
		loadExtraHeaders(project,headers);
}

/*******************  FUNCTION  *********************/
void CMRProjectXMLLoader::loadExtraHeaders ( CMRProject2& project, CMRXmlNode& node )
{
	//errors
	assert(node.isValid());
	assert(node.isNamed("headers"));
	
	//loop on all
	CMRXmlNode cur = node.getFirstChild();
	while (cur.isValid())
	{
		if (cur.isNamed("include"))
			project.addUserHeader(cur.getContent());
		else
			throw LatexException("Invalid tag name while loading project user headers in XML file.");
		cur = cur.getNext();
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectXMLLoader::loadOutput ( CMRProject2& project, CMRXmlNode& node )
{
	//errors
	assert(node.isValid());
	assert(node.isNamed("output"));
	
	//loop on all
	CMRXmlNode cur = node.getFirstChild();
	while (cur.isValid())
	{
		if (cur.isNamed("entry"))
		{
			string name = cur.getNonEmptyProperty("name");
			string type = cur.getNonEmptyProperty("type");
			project.addOutputEntry(name,type,cur.getContent());
		} else {
			throw LatexException("Invalid tag name while loading project user headers in XML file.");
		}
		cur = cur.getNext();
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectXMLLoader::loadElements ( CMRProject2& project, CMRXmlNode& node )
{
	//check
	assert(node.isValid());
	assert(node.isNamed(CMR_NODE_ELEMNTS));
	
	//load consts
	CMRXmlNode constsNode = node.getUniqChild(CMR_NODE_CONSTS);
	if (constsNode.isValid())
		loadConsts(project,constsNode);
	
	//load mesh
	CMRXmlNode meshNode = node.getUniqChild(CMR_NODE_MESH);
	if (meshNode.isValid())
		loadMesh(project,meshNode);
	
	//load iterators
	CMRXmlNode iteratorsNode = node.getUniqChild(CMR_NODE_ITERATORS);
	if (iteratorsNode.isValid())
		loadGlobIterators(project,iteratorsNode);
	
	//load iterators
	CMRXmlNode defsNode = node.getUniqChild(CMR_NODE_DEFINITIONS);
	if (defsNode.isValid())
		loadDefinitions(project,defsNode);
	
	//load iterators
	CMRXmlNode actionsNode = node.getUniqChild(CMR_NODE_CELL_ACTIONS);
	if (actionsNode.isValid())
		loadCellActions(project,actionsNode);
	
	//load iterators
	CMRXmlNode initNode = node.getUniqChild("init");
	if (initNode.isValid())
		loadInitCallActions(project,initNode);
	
	//load iterators
	CMRXmlNode mainNode = node.getUniqChild("mainloop");
	if (mainNode.isValid())
		loadMainCallActions(project,mainNode);
	
	//load iterators
	CMRXmlNode outputNode = node.getUniqChild("output");
	if (outputNode.isValid())
		loadOutput(project,outputNode);
}

/*******************  FUNCTION  *********************/
void CMRProjectXMLLoader::loadConsts ( CMRProject2& project, CMRXmlNode& node )
{
	//check
	assert(node.isValid());
	assert(node.isNamed(CMR_NODE_CONSTS));	
	
	//loop on childs
	CMRXmlNode cur = node.getFirstChild();
	while (cur.isValid())
	{
		if (cur.isNamed(CMR_NODE_CONST))
			loadConst(project,cur);
		else
			throw LatexException("Invalid tag name while loading project constants in XML file.");
		cur = cur.getNext();
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectXMLLoader::loadConst ( CMRProject2& project, CMRXmlNode& node )
{
	//check
	assert(node.isValid());
	assert(node.isNamed(CMR_NODE_CONST));
	
	//extract infos
	int dims = atoi(node.getProperty(CMR_PROP_DIMS).c_str());
	string mathName = node.getNonEmptyProperty(CMR_PROP_MATHNAME);
	string longName = node.getNonEmptyProperty(CMR_PROP_LONGNAME);
	string value = node.getContent();
	
	//errors
	assert(dims >=0 && dims <= 2);
	cmrDebug("Load constant : %s",mathName.c_str());
	
	//insert in project
	CMRProjectConstant & entity = project.addConstant(mathName,longName);
	entity.loadValues(value,dims);
}

/*******************  FUNCTION  *********************/
void CMRProjectXMLLoader::loadMesh ( CMRProject2& project, CMRXmlNode& node )
{
	//check
	assert(node.isValid());
	assert(node.isNamed(CMR_NODE_MESH));
	
	//loop on vars
	CMRXmlNode cur = node.getFirstChild();
	while (cur.isValid())
	{
		if (cur.isNamed(CMR_NODE_MESH_VAR))
			loadMeshVar(project,cur);
		else
			throw LatexException("Invalid tag name while loading project mesh in XML file.");
		cur = cur.getNext();
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectXMLLoader::loadMeshVar ( CMRProject2& project, CMRXmlNode& node )
{
	//check
	assert(node.isValid());
	assert(node.isNamed(CMR_NODE_MESH_VAR));

	//extract info
	string mathName = node.getNonEmptyProperty(CMR_PROP_MATHNAME);
	string longName = node.getNonEmptyProperty(CMR_PROP_LONGNAME);
	string type = node.getNonEmptyProperty("type");
	cmrDebug("Load mesh variable %s -> %s",mathName.c_str(),longName.c_str());

	//create
	ProjectMeshVar & var = project.addvariable(mathName,longName,type);

	//loop on extra dims
	CMRXmlNode extraDimsNode = node.getUniqChild(CMR_NODE_EXTRA_DIMS);
	if (extraDimsNode.isValid())
	{
		CMRXmlNode cur = extraDimsNode.getFirstChild();
		while (cur.isValid())
		{
			if (cur.isNamed(CMR_NODE_EXTRA_DIM))
			{
				string dimMathName = cur.getNonEmptyProperty(CMR_PROP_MATHNAME);
				string dimLongName = cur.getNonEmptyProperty(CMR_PROP_LONGNAME);
				int dimSize = atoi(cur.getNonEmptyProperty("size").c_str());
				int dimStart = atoi(cur.getProperty("start").c_str());
				assert(dimSize > 1);
				var.addDim(dimMathName,dimSize,dimStart);
			} else {
				throw LatexException("Invalid tag name while loading project mesh var in XML file.");
			}
			cur = cur.getNext();
		}
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectXMLLoader::loadGlobIterators ( CMRProject2& project, CMRXmlNode& node )
{
	//check
	assert(node.isValid());
	assert(node.isNamed(CMR_NODE_ITERATORS));

	//loop on vars
	CMRXmlNode cur = node.getFirstChild();
	while (cur.isValid())
	{
		if (cur.isNamed(CMR_NODE_ITERATOR))
			loadGlobIterator(project,cur);
		else
			throw LatexException("Invalid tag name while loading project iterators in XML file.");
		cur = cur.getNext();
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectXMLLoader::loadGlobIterator ( CMRProject2& project, CMRXmlNode& node )
{
	//check
	assert(node.isValid());
	assert(node.isNamed(CMR_NODE_ITERATOR));

	//load info
	string mathName = node.getNonEmptyProperty(CMR_PROP_MATHNAME);
	string longName = node.getNonEmptyProperty(CMR_PROP_LONGNAME);
	int start = atoi(node.getNonEmptyProperty("start").c_str());
	int end = atoi(node.getNonEmptyProperty("end").c_str());
	cmrDebug("Load global iterator : %s",mathName.c_str());

	//create
	project.addIterator(mathName,longName,start,end);
}

/*******************  FUNCTION  *********************/
void CMRProjectXMLLoader::loadDefinitions ( CMRProject2& project, CMRXmlNode& node )
{
	//check
	assert(node.isValid());
	assert(node.isNamed(CMR_NODE_DEFINITIONS));

	//loop on vars
	CMRXmlNode cur = node.getFirstChild();
	while (cur.isValid())
	{
		if (cur.isNamed(CMR_NODE_DEFINITION))
			loadDefinition(project,cur);
		else
			throw LatexException("Invalid tag name while loading project definitions in XML file.");
		cur = cur.getNext();
	}
}

/*******************  FUNCTION  *********************/
template <class T>
bool CMRProjectXMLLoader::loadCodeNode ( CMRProject2& project, T& parent, CMRXmlNode& node )
{
	bool res = true;

	if (node.isNamed(CMR_NODE_MATHSTEP))
	{
		string buffer = node.getContent();
		cmrDebug("   -> mathstep : %s",buffer.c_str());
		parent.addEquation(buffer);
	} else if (node.isNamed(CMR_NODE_DEF_PARAM)) {
		string paramMathName = node.getNonEmptyProperty(CMR_PROP_MATHNAME);
		string paramLongName = node.getNonEmptyProperty(CMR_PROP_LONGNAME);
		string type = node.getNonEmptyProperty("type");
		cmrDebug("   -> param : %s",paramMathName.c_str());
		parent.changeCaptureType(paramMathName,CAPTURE_REQUIRED);
	} else if (node.isNamed(CMR_NODE_DECL_VAR)) {
		string declVarMathName = node.getNonEmptyProperty(CMR_PROP_MATHNAME);
		string declVarLongName = node.getNonEmptyProperty(CMR_PROP_LONGNAME);
		string declVarDefault = node.getProperty("default");
		string declVarType = node.getNonEmptyProperty("type");
		cmrDebug("   -> declarvar : %s",declVarMathName.c_str());
		parent.addLocalVariable(declVarMathName,declVarLongName,declVarType,declVarDefault);
	} else if (node.isNamed(CMR_NODE_FOREACH)) {
		string loopIterator = node.getNonEmptyProperty("iterator");
		CMRProjectCodeIteratorLoop & loop = parent.addIteratorLoop(loopIterator);
		loadCode(project,loop,node);
	} else if (node.isNamed(CMR_NODE_ALIAS)) {
		string aliasMathName = node.getNonEmptyProperty(CMR_PROP_MATHNAME);
		string aliasBody = node.getContent();
		bool aliasWildcardName = node.getProperty("capturename") == "true";
		string aliasCaptureAllStr = node.getProperty("captureall");
		bool aliasCaptureAll = (aliasCaptureAllStr.empty() || aliasCaptureAllStr == "true");
		cmrDebug("    -> Alias : %s",aliasMathName.c_str());
		if (aliasWildcardName)
			parent.getContext().addEntry(new CMRProjectAlias(aliasMathName,aliasBody,aliasCaptureAll)).captureName();
		else
			parent.getContext().addEntry(new CMRProjectAlias(aliasMathName,aliasBody,aliasCaptureAll));
	} else if (node.isNamed(CMR_NODE_CCODE)) {
		string codeContent = node.getContent();
		cmrDebug("    -> CCode : %s",codeContent.c_str());
		parent.insert(new CMRProjectCSimpleConstruct(&parent.getContext(),codeContent));
	} else {
		res = false;
	}
	
	return res;
}

/*******************  FUNCTION  *********************/
template <class T>
void CMRProjectXMLLoader::loadCode ( CMRProject2& project, T& parent, CMRXmlNode& node )
{
	//loop on childs to load actions
	CMRXmlNode cur = node.getFirstChild();
	while (cur.isValid())
	{
		if (loadCodeNode(project,parent,cur) == false)
			cmrFatal("Invalid tag name <%s> while loading project definitions actions in XML file.",cur.getName().c_str());
		cur = cur.getNext();
	}
}


/*******************  FUNCTION  *********************/
void CMRProjectXMLLoader::loadDefinition ( CMRProject2& project, CMRXmlNode& node )
{
	//check
	assert(node.isValid());
	assert(node.isNamed(CMR_NODE_DEFINITION));

	//extract info
	string mathName = node.getNonEmptyProperty(CMR_PROP_MATHNAME);
	string longName = node.getNonEmptyProperty(CMR_PROP_LONGNAME);

	//create the definition
	ProjectDefinition & def = project.addDefinition(mathName,longName);
	
	//load code
	loadCode(project,def,node);
}

/*******************  FUNCTION  *********************/
void CMRProjectXMLLoader::loadCellActions ( CMRProject2& project, CMRXmlNode& node )
{
	//check
	assert(node.isValid());
	assert(node.isNamed(CMR_NODE_CELL_ACTIONS));

	//loop on vars
	CMRXmlNode cur = node.getFirstChild();
	while (cur.isValid())
	{
		if (cur.isNamed(CMR_NODE_CELL_ACTION))
			loadCellAction(project,cur);
		else
			throw LatexException("Invalid tag name while loading project cell action in XML file.");
		cur = cur.getNext();
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectXMLLoader::loadCellAction ( CMRProject2& project, CMRXmlNode& node )
{
	//check
	assert(node.isValid());
	assert(node.isNamed(CMR_NODE_CELL_ACTION));

	//extract info
	string name = node.getNonEmptyProperty("name");

	//create the definition
	ProjectAction & action = project.addAction(name);

	//load code
	CMRXmlNode cur = node.getFirstChild();
	while (cur.isValid())
	{
		
		if(cur.isNamed(CMR_NODE_DEF_PARAM)) 
		{
			string paramMathName = cur.getNonEmptyProperty(CMR_PROP_MATHNAME);
			string paramLongName = cur.getNonEmptyProperty(CMR_PROP_LONGNAME);
			string type = cur.getNonEmptyProperty("type");
			cmrDebug("   -> param : %s",paramMathName.c_str());
			action.addParameter(paramMathName,paramLongName,type);
		} else if (loadCodeNode(project,action,cur) == false) { 
			cmrFatal("Invalid tag name <%s> while loading project definitions actions in XML file.",cur.getName().c_str());
		}
		cur = cur.getNext();
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectXMLLoader::loadInitCallActions ( CMRProject2& project, CMRXmlNode& node )
{
	//check
	assert(node.isValid());
	assert(node.isNamed("init"));
	
	//loop on childs
	CMRXmlNode cur = node.getFirstChild();
	while (cur.isValid())
	{
		if (cur.isNamed("callaction"))
			loadInitCallAction(project,cur);
		else
			throw LatexException("Invalid tag name while loading project init in XML file.");
		cur = cur.getNext();
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectXMLLoader::loadMainCallActions ( CMRProject2& project, CMRXmlNode& node )
{
	//check
	assert(node.isValid());
	assert(node.isNamed("mainloop"));
	
	//loop on childs
	CMRXmlNode cur = node.getFirstChild();
	while (cur.isValid())
	{
		if (cur.isNamed("callaction"))
			loadMainCallAction(project,cur);
		else
			throw LatexException("Invalid tag name while loading project init in XML file.");
		cur = cur.getNext();
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectXMLLoader::loadInitCallAction ( CMRProject2& project, CMRXmlNode& node )
{
	//check
	assert(node.isValid());
	assert(node.isNamed("callaction"));
	
	//extract info
	string name = node.getNonEmptyProperty("name");
	CMRProjectCallAction & action = project.addInitCallAction(name);
	
	//loop on childs
	CMRXmlNode cur = node.getFirstChild();
	while (cur.isValid())
	{
		if (cur.isNamed("parameter"))
		{
			string mathName = cur.getNonEmptyProperty("mathname");
			string value = cur.getContent();
			action.addParameter(mathName,value);
		} else if (cur.isNamed("zone")) {
			string value = cur.getContent();
			action.addZone(value);
		} else {
			throw LatexException("Invalid tag name while loading project init in XML file.");
		}
		cur = cur.getNext();
	}
}

/*******************  FUNCTION  *********************/
void CMRProjectXMLLoader::loadMainCallAction ( CMRProject2& project, CMRXmlNode& node )
{
	//check
	assert(node.isValid());
	assert(node.isNamed("callaction"));
	
	//extract info
	string name = node.getNonEmptyProperty("name");
	CMRProjectCallAction & action = project.addMainLoopCallAction(name);
	
	//loop on childs
	CMRXmlNode cur = node.getFirstChild();
	while (cur.isValid())
	{
		if (cur.isNamed("parameter"))
		{
			string mathName = cur.getNonEmptyProperty("mathname");
			string value = cur.getContent();
			action.addParameter(mathName,value);
		} else if (cur.isNamed("zone")) {
			string value = cur.getContent();
			action.addZone(value);
		} else {
			throw LatexException("Invalid tag name while loading project init in XML file.");
		}
		cur = cur.getNext();
	}
}

}
