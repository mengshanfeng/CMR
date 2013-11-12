/********************  HEADERS  *********************/
#include <string>


/********************  NAMESPACE  *******************/
namespace CMRCompiler
{

/*********************  TYPES  **********************/
class CMRProject2;
class CMRXmlNode;

/*********************  CLASS  **********************/
class CMRProjectXMLLoader
{
	public:
		void load(CMRProject2 & project, std::string & filename);
	protected:
		void load(CMRProject2 & project, CMRXmlNode & rootNode);
		void loadElements(CMRProject2 & project, CMRXmlNode & node);
		void loadConsts(CMRProject2 & project, CMRXmlNode & node);
		void loadConst(CMRProject2 & project, CMRXmlNode & node);
		void loadMesh(CMRProject2 & project, CMRXmlNode & node);
		void loadMeshVar(CMRProject2 & project, CMRXmlNode & node);
		void loadGlobIterators(CMRProject2 & project, CMRXmlNode & node);
		void loadGlobIterator(CMRProject2 & project, CMRXmlNode & node);
		void loadDefinitions(CMRProject2 & project, CMRXmlNode & node);
		void loadDefinition(CMRProject2 & project, CMRXmlNode & node);
		void loadCellActions(CMRProject2 & project, CMRXmlNode & node);
		void loadCellAction(CMRProject2 & project, CMRXmlNode & node);
		void loadInitCallActions(CMRProject2 & project, CMRXmlNode & node);
		void loadInitCallAction(CMRProject2 & project, CMRXmlNode & node);
		void loadMainCallActions(CMRProject2 & project, CMRXmlNode & node);
		void loadMainCallAction(CMRProject2 & project, CMRXmlNode & node);
		template <class T> void loadCode(CMRProject2 & project, T & parent , CMRXmlNode & node);
		template <class T> bool loadCodeNode(CMRProject2 & project, T & parent , CMRXmlNode & node);
};

}
