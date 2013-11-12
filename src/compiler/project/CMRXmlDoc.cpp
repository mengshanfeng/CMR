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
#include "../parsor/LatexException.h"
#include "CMRXmlDoc.h"
#include "common/MsgFormat.h"

/**********************  USING  *********************/
using namespace std;

/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*******************  FUNCTION  *********************/
CMRXmlDoc::CMRXmlDoc ( void )
{
	this->doc = NULL;
	this->rootNode = NULL;
}

/*******************  FUNCTION  *********************/
CMRXmlDoc::~CMRXmlDoc ( void )
{
	//free memory
	if (doc != NULL)
		xmlFreeDoc(doc);

	//reset pointers
	this->doc = NULL;
	this->rootNode = NULL;
}

/*******************  FUNCTION  *********************/
void CMRXmlDoc::loadFromMem ( const string& xml )
{
	doc = xmlParseMemory(xml.c_str(),xml.size());
	if (doc == NULL)
		throw LatexException("Invalid xml format.");
	rootNode = xmlDocGetRootElement(doc);
	if (rootNode == NULL)
		throw LatexException("Fail to get root element in XML document.");
}

/*******************  FUNCTION  *********************/
void CMRXmlDoc::loadFromFile ( const string& xmlFile )
{
	doc = xmlParseFile(xmlFile.c_str());
	if (doc == NULL)
		throw LatexException("Invalid filename.");
	rootNode = xmlDocGetRootElement(doc);
	if (rootNode == NULL)
		throw LatexException("Fail to get root element in XML document.");
	
}

/*******************  FUNCTION  *********************/
void CMRXmlDoc::validateWithDTD ( const string& dtdFile )
{
	/* vars */
	bool status = true;
	xmlDtdPtr dtd;
	xmlValidCtxtPtr vctxt;

	/* errors */
	assert(doc != NULL);
	assert(rootNode != NULL);
	assert(dtdFile.empty() == false);
	
	/* Nothing to do */
	if (rootNode == NULL || dtdFile.empty())
		return;

	/* Open XML schema file */
	dtd = xmlParseDTD(NULL, BAD_CAST(dtdFile.c_str()));
	assert(dtd != NULL);
	//assume_m(pctxt != NULL,"Fail to open XML schema file to validate config : %s.",xml_shema_path);


	/* Create validation context */
	if ((vctxt = xmlNewValidCtxt()) == NULL) {
		assert(vctxt != NULL);
		//sctk_fatal("Fail to create validation context from XML schema file : %s.",xml_shema_path);
	}

	/* Create validation output system */
	vctxt->userData = (void *) stderr;
	vctxt->error = (xmlValidityErrorFunc) fprintf;
	vctxt->warning = (xmlValidityWarningFunc) fprintf;

	/* Validation */
	status = (xmlValidateDtd(vctxt, doc,dtd) == 0);

	/* Free the schema */
	xmlFreeDtd(dtd);
	 xmlFreeValidCtxt(vctxt);

	if (!status)
		throw LatexException("XML file is invalid.");
}

/*******************  FUNCTION  *********************/
void CMRXmlDoc::validateWithSchema ( const string& xsltFile )
{
	/* vars */
	bool status = true;
	xmlSchemaPtr schema;
	xmlSchemaValidCtxtPtr vctxt;
	xmlSchemaParserCtxtPtr pctxt;

	/* errors */
	assert(doc != NULL);
	assert(rootNode != NULL);
	assert(xsltFile.empty() == false);
	
	/* Nothing to do */
	if (rootNode == NULL || xsltFile.empty())
		return;

	/* Open XML schema file */
	pctxt = xmlSchemaNewParserCtxt(xsltFile.c_str());
	assert(pctxt != NULL);
	//assume_m(pctxt != NULL,"Fail to open XML schema file to validate config : %s.",xml_shema_path);

	/* Parse the schema */
	schema = xmlSchemaParse(pctxt);
	xmlSchemaFreeParserCtxt(pctxt);
	assert(schema != NULL);
	//assume_m(schema != NULL,"Fail to parse the XML schema file to validate config : %s.",xml_shema_path);

	/* Create validation context */
	if ((vctxt = xmlSchemaNewValidCtxt(schema)) == NULL) {
		assert(vctxt != NULL);
		//sctk_fatal("Fail to create validation context from XML schema file : %s.",xml_shema_path);
	}

	/* Create validation output system */
	xmlSchemaSetValidErrors(vctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);

	/* Validation */
	status = (xmlSchemaValidateDoc(vctxt, doc) == 0);

	/* Free the schema */
	xmlSchemaFree(schema);
	xmlSchemaFreeValidCtxt(vctxt);

	if (!status)
		throw LatexException("XML file is invalid.");
}

/*******************  FUNCTION  *********************/
CMRXmlNode CMRXmlDoc::getRootNode ( void )
{
	return CMRXmlNode(rootNode);
}

/*******************  FUNCTION  *********************/
CMRXmlNode::CMRXmlNode ( xmlNodePtr node )
{
	this->node = node;
	if (node != NULL)
		assert(node->type == XML_ELEMENT_NODE);
}

/*******************  FUNCTION  *********************/
string CMRXmlNode::getName ( void ) const
{
	assert(node != NULL);
	return (char*)node->name;
}

/*******************  FUNCTION  *********************/
bool CMRXmlNode::isNamed ( const string& name ) const
{
	assert(node != NULL);
	return (xmlStrcmp(BAD_CAST(name.c_str()),node->name) == 0);
}

/*******************  FUNCTION  *********************/
string CMRXmlNode::getContent ( void ) const
{
	//errors
	assert(node != NULL);
	
	//extract
	string res;
	xmlChar * tmp = xmlNodeGetContent(node);
	if (tmp != NULL)
	{
		res = (char*)tmp;
		xmlFree(tmp);
	}
	
	return res;
}

/*******************  FUNCTION  *********************/
bool CMRXmlNode::isValid ( void ) const
{
	return (node != NULL);
}

/*******************  FUNCTION  *********************/
CMRXmlNode CMRXmlNode::getFirstChild ( const string& tagname )
{
	//get first child
	CMRXmlNode cur = this->getFirstChild();
	
	//loop while and not found
	while (cur.isValid())
	{
		if (cur.isNamed(tagname))
			break;
		cur = cur.getNext();
	}
	
	return cur;
}

/*******************  FUNCTION  *********************/
CMRXmlNode CMRXmlNode::getFirstChild ( void )
{
	//nothing
	if (node == NULL)
	{
		return CMRXmlNode(NULL);
	} else if ((node->type == XML_ELEMENT_NODE) && (node->children != NULL)) {
		xmlNodePtr res = xmlFirstElementChild(node);
		return CMRXmlNode(res);
	} else {
		return CMRXmlNode(NULL);
	}
}

/*******************  FUNCTION  *********************/
CMRXmlNode CMRXmlNode::getNext ( void )
{
	if (node == NULL)
		return CMRXmlNode(NULL);
	else
		return CMRXmlNode(xmlNextElementSibling(node));
}

/*******************  FUNCTION  *********************/
CMRXmlNode CMRXmlNode::getNextSibiling ( void )
{
	CMRXmlNode cur(getNext());
	
	while (cur.isValid() && xmlStrcmp(node->name,cur.node->name) != 0)
		cur = cur.getNext();
	
	return cur;
}

/*******************  FUNCTION  *********************/
CMRXmlNode CMRXmlNode::getUniqChild ( const string& tagname )
{
	CMRXmlNode cur = getFirstChild(tagname);
	if (cur.isValid())
	{
		assert(cur.isNamed(tagname));
		if (cur.getNextSibiling().isValid())
			throw LatexException("Caution, you want to get a tag value, but there is multiple instances of this tag.");
	}
	return cur;
}

/*******************  FUNCTION  *********************/
string CMRXmlNode::getChildContent ( const string& tagname )
{
	CMRXmlNode node = getUniqChild(tagname);
	if (node.isValid() == false)
		throw LatexException("Failed to find the requested tagname.");
	return node.getContent();
}

/*******************  FUNCTION  *********************/
string CMRXmlNode::getProperty ( const string& name ) const
{
	if (node == NULL)
		return "";
	
	xmlAttr *attr = node->properties;
	while ( attr )
	{
		if (xmlStrcmp(BAD_CAST(name.c_str()),attr->name) == 0)
			break;
		attr = attr->next;
	}
	
	if (attr == NULL)
		return "";
	else
		return (char*)attr->children->content;
}

/*******************  FUNCTION  *********************/
string CMRXmlNode::getNonEmptyProperty ( const string& name ) const
{
	string res = getProperty(name);
	if (res.empty())
		MsgFormat("Caution, get empty property '%1' while readine XML.").arg(name).exception();
	return res;
}

}
