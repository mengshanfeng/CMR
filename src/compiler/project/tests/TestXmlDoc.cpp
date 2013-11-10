/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include "CMRXmlDoc.h"
#include <../parsor/CMRLatexException.h>

/**********************  USING  *********************/
using namespace std;
using namespace testing;

/*********************  CONSTS  *********************/
static const char * CST_VALUE_1 = "<?xml version='1.0' encoding='UTF-8'?>\n<test parama='a' paramb='b'>\n\
	<childA>A1</childA>\n\
	<childB>B1</childB>\n\
	<childB>B2</childB>\n\
	<childC>C1</childC>\n\
	<childA>A2</childA>\n\
</test>";

/*********************  CLASS  **********************/
class TestXmlDoc : public Test
{
	public:
		TestXmlDoc(void);
	protected:
		CMRXmlDoc doc;
};

/*******************  FUNCTION  *********************/
TestXmlDoc::TestXmlDoc ( void )
{
	doc.loadFromMem(CST_VALUE_1);
}

/*******************  FUNCTION  *********************/
TEST_F(TestXmlDoc,constructor)
{
	
}

/*******************  FUNCTION  *********************/
TEST_F(TestXmlDoc,getName)
{
	EXPECT_EQ("test",doc.getRootNode().getName());
}

/*******************  FUNCTION  *********************/
TEST_F(TestXmlDoc,isNamed)
{
	EXPECT_TRUE(doc.getRootNode().isNamed("test"));
	EXPECT_FALSE(doc.getRootNode().isNamed("TEST"));
}

/*******************  FUNCTION  *********************/
TEST_F(TestXmlDoc,getContent)
{
	CMRXmlNode node = doc.getRootNode().getFirstChild();
	EXPECT_EQ("A1",node.getContent());
}

/*******************  FUNCTION  *********************/
TEST_F(TestXmlDoc,isValid_true)
{
	CMRXmlNode node = doc.getRootNode();
	EXPECT_TRUE(node.isValid());
}

/*******************  FUNCTION  *********************/
TEST_F(TestXmlDoc,isValid_false)
{
	CMRXmlNode node(NULL);
	EXPECT_FALSE(node.isValid());
}

/*******************  FUNCTION  *********************/
TEST_F(TestXmlDoc,getFirstChild)
{
	CMRXmlNode node = doc.getRootNode().getFirstChild();
	EXPECT_EQ("childA",node.getName());
	EXPECT_EQ("A1",node.getContent());
}

/*******************  FUNCTION  *********************/
TEST_F(TestXmlDoc,getFirstChildNamed)
{
	CMRXmlNode node = doc.getRootNode().getFirstChild("childB");
	EXPECT_EQ("childB",node.getName());
	EXPECT_EQ("B1",node.getContent());
}

/*******************  FUNCTION  *********************/
TEST_F(TestXmlDoc,getNextSibiling)
{
	CMRXmlNode node = doc.getRootNode().getFirstChild();
	node = node.getNextSibiling();
	EXPECT_EQ("childA",node.getName());
	EXPECT_EQ("A2",node.getContent());
}

/*******************  FUNCTION  *********************/
TEST_F(TestXmlDoc,getNext)
{
	CMRXmlNode node = doc.getRootNode().getFirstChild();
	node = node.getNext();
	EXPECT_EQ("childB",node.getName());
	EXPECT_EQ("B1",node.getContent());
}

/*******************  FUNCTION  *********************/
TEST_F(TestXmlDoc,getChildContent_ok)
{
	CMRXmlNode node = doc.getRootNode();
	string res = node.getChildContent("childC");
	EXPECT_EQ("C1",res);
}

/*******************  FUNCTION  *********************/
TEST_F(TestXmlDoc,getChildContent_multi)
{
	CMRXmlNode node = doc.getRootNode();
	EXPECT_THROW(node.getChildContent("childA"),CMRLatexException);
}

/*******************  FUNCTION  *********************/
TEST_F(TestXmlDoc,getChildContent_none)
{
	CMRXmlNode node = doc.getRootNode();
	EXPECT_THROW(node.getChildContent("childD"),CMRLatexException);
}

/*******************  FUNCTION  *********************/
TEST_F(TestXmlDoc,getProperty)
{
	CMRXmlNode node = doc.getRootNode();
	EXPECT_EQ("a",node.getProperty("parama"));
	EXPECT_EQ("b",node.getProperty("paramb"));
}
