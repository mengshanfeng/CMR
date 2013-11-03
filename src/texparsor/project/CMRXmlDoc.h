/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
                      : Jean-Baptiste Besnard
             LICENSE  : CeCILL-C
*****************************************************/

#ifndef CMR_XML_DOC_H
#define CMR_XML_DOC_H

/********************  HEADERS  *********************/
#include <string>
#include <libxml/xmlschemas.h>

/*********************  CLASS  **********************/
class CMRXmlNode
{
	public:
		CMRXmlNode(xmlNodePtr node);
		std::string getName(void) const;
		bool isNamed(const std::string & name) const;
		std::string getContent(void) const;
		bool isValid(void) const;
		CMRXmlNode getFirstChild(void);
		CMRXmlNode getFirstChild(const std::string & tagname);
		CMRXmlNode getNextSibiling(void);
		CMRXmlNode getNext(void);
		std::string getChildContent( const std::string& tagname );
		std::string getProperty(const std::string & name) const;
		std::string getNonEmptyProperty(const std::string & name) const;
		CMRXmlNode getUniqChild( const std::string& tagname );
	private:
		xmlNodePtr node;
};

/*********************  CLASS  **********************/
class CMRXmlDoc
{
	public:
		CMRXmlDoc(void);
		~CMRXmlDoc(void);
		void validateWithSchema(const std::string & xsltFile);
		void validateWithDTD(const std::string & dtdFile);
		CMRXmlNode getRootNode(void);
		void loadFromFile(const std::string & xmlFile);
		void loadFromMem(const std::string & xml);
	private:
		xmlDocPtr doc;
		xmlNodePtr rootNode;
};

#endif //CMR_XML_DOC_H
