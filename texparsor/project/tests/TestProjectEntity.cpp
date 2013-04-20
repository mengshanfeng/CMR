/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 06/2012
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <svUnitTest.h>
#include <CMRProjectEntity.h>
#include <../parsor/CMRLatexFormula.h>
#include <sstream>

/**********************  USING  *********************/
using namespace svUnitTest;
using namespace std;

/*********************  CLASS  **********************/
class MockProjectEntity : public CMRProjectEntity
{
	public:
		MockProjectEntity ( const string& latexName, const string& longName ) : CMRProjectEntity(latexName,longName) {};
		virtual ostream& genDefinitionCCode ( ostream& out, const CMRProjectContext& context ) {return out;};
		virtual ostream& genUsageCCode ( ostream& out, const CMRProjectContext& context, CMRLatexEntity2& entity ) {return out;};
};

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectEntity,testConstructor)
{
	MockProjectEntity entity("A_{i,j}^2","test");
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectEntity,testGetLatexName)
{
	MockProjectEntity entity("A_{i,j}^2","test");
	SVUT_ASSERT_EQUAL("A_{i,j}^2",entity.getLatexName());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectEntity,testAddIndice_ok)
{
	MockProjectEntity entity("A_{i,j}^2","test");
	entity.changeCaptureType("i",CMR_CAPTURE_REQUIRED);
	SVUT_ASSERT_EQUAL("A_{i,j}^2",entity.getLatexName());
	entity.addIndice("k");
	SVUT_ASSERT_EQUAL("A_{i,j,k}^2",entity.getLatexName());
	SVUT_ASSERT_NOT_THROW(CMRLatexException, entity.addIndice("i"));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectEntity,testAddIndice_duplicate)
{
	MockProjectEntity entity("A_{i,j}^2","test");
	entity.changeCaptureType("i",CMR_CAPTURE_REQUIRED);
	SVUT_ASSERT_EQUAL("A_{i,j}^2",entity.getLatexName());
	SVUT_ASSERT_THROW(CMRLatexException, entity.addIndice("i",CMR_CAPTURE_REQUIRED));
	entity.addIndice("2");
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectEntity,testAddExponent_ok)
{
	MockProjectEntity entity("A_{i,j}^2","test");
	entity.changeCaptureType("i",CMR_CAPTURE_REQUIRED);
	SVUT_ASSERT_EQUAL("A_{i,j}^2",entity.getLatexName());
	entity.addExponent("k");
	SVUT_ASSERT_EQUAL("A_{i,j}^{2,k}",entity.getLatexName());
	SVUT_ASSERT_NOT_THROW(CMRLatexException, entity.addExponent("i"));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectEntity,testAddExponent_duplicate)
{
	MockProjectEntity entity("A_{i,j}^2","test");
	entity.changeCaptureType("i",CMR_CAPTURE_REQUIRED);
	SVUT_ASSERT_EQUAL("A_{i,j}^2",entity.getLatexName());
	SVUT_ASSERT_THROW(CMRLatexException, entity.addExponent("i",CMR_CAPTURE_REQUIRED));
	entity.addExponent("2");
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectEntity,testAddParameter_ok)
{
	MockProjectEntity entity("A_{i,j}^2","test");
	entity.changeCaptureType("i",CMR_CAPTURE_REQUIRED);
	SVUT_ASSERT_EQUAL("A_{i,j}^2",entity.getLatexName());
	entity.addParameter("k");
	SVUT_ASSERT_EQUAL("A_{i,j}^2{k}",entity.getLatexName());
	entity.addParameter("l");
	SVUT_ASSERT_EQUAL("A_{i,j}^2{k}{l}",entity.getLatexName());
	SVUT_ASSERT_NOT_THROW(CMRLatexException, entity.addParameter("i"));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectEntity,testAddParameter_duplicate)
{
	MockProjectEntity entity("A_{i,j}^2","test");
	entity.changeCaptureType("i",CMR_CAPTURE_REQUIRED);
	SVUT_ASSERT_EQUAL("A_{i,j}^2",entity.getLatexName());
	SVUT_ASSERT_THROW(CMRLatexException, entity.addParameter("i",CMR_CAPTURE_REQUIRED));
	entity.addParameter("2");
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectEntity,testChangeCaptureType_ok)
{
	MockProjectEntity entity("A_{i,j}^2","test");
	entity.changeCaptureType("i",CMR_CAPTURE_REQUIRED);
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectEntity,testChangeCaptureType_invalid)
{
	MockProjectEntity entity("A_{i,j}^2","test");
	SVUT_ASSERT_THROW(CMRLatexException,entity.changeCaptureType("m",CMR_CAPTURE_REQUIRED));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectEntity,testMatch_1)
{
	MockProjectEntity entity("A_{eq,k}","test");
	entity.changeCaptureType("k",CMR_CAPTURE_REQUIRED);
	CMRLatexEntity2 le("A_{eq,42}");
	SVUT_ASSERT_TRUE(entity.match(le));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectEntity,testMatch_2)
{
	MockProjectEntity entity("A_{eq,k}","test");
	entity.changeCaptureType("k",CMR_CAPTURE_REQUIRED);
	CMRLatexEntity2 le("A_{eq2,42}");
	SVUT_ASSERT_FALSE(entity.match(le));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectEntity,testMatch_3)
{
	MockProjectEntity entity("A_{eq,k}","test");
	entity.changeCaptureType("k",CMR_CAPTURE_REQUIRED);
	CMRLatexEntity2 le("A_{eq}");
	SVUT_ASSERT_FALSE(entity.match(le));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectEntity,testMatch_4)
{
	MockProjectEntity entity("A_{eq,k}","test");
	entity.changeCaptureType("k",CMR_CAPTURE_REQUIRED);
	CMRLatexEntity2 le("A_{eq,42,43}");
	SVUT_ASSERT_FALSE(entity.match(le));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectEntity,testMatch_5)
{
	MockProjectEntity entity("A_{eq,k}^{4}","test");
	entity.changeCaptureType("k",CMR_CAPTURE_REQUIRED);
	CMRLatexEntity2 le("A_{eq,42}^4");
	SVUT_ASSERT_TRUE(entity.match(le));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectEntity,testCapture_1)
{
	MockProjectEntity entity("A_{a,b}^{c,d}","test");
	entity.changeCaptureType("a",CMR_CAPTURE_REQUIRED);
	entity.changeCaptureType("b",CMR_CAPTURE_REQUIRED);
	entity.changeCaptureType("c",CMR_CAPTURE_REQUIRED);
	entity.changeCaptureType("d",CMR_CAPTURE_REQUIRED);

	CMRLatexEntity2 le("A_{1,2}^{3,4}");
	
	SVUT_ASSERT_TRUE(entity.match(le));
	
	CMRProjectCaptureMap capture;
	entity.capture(le,capture);
	
	SVUT_ASSERT_EQUAL(4,capture.size());
	SVUT_ASSERT_EQUAL("1",capture["a"]->getString());
	SVUT_ASSERT_EQUAL("2",capture["b"]->getString());
	SVUT_ASSERT_EQUAL("3",capture["c"]->getString());
	SVUT_ASSERT_EQUAL("4",capture["d"]->getString());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectEntity,testCapture_2)
{
	MockProjectEntity entity("\\frac{a}{b}","test");
	entity.changeCaptureType("a",CMR_CAPTURE_REQUIRED);
	entity.changeCaptureType("b",CMR_CAPTURE_REQUIRED);

	CMRLatexEntity2 le("\\frac{1}{2}");
	
	SVUT_ASSERT_TRUE(entity.match(le));
	
	CMRProjectCaptureMap capture;
	entity.capture(le,capture);
	
	SVUT_ASSERT_EQUAL(2,capture.size());
	SVUT_ASSERT_EQUAL("1",capture["a"]->getString());
	SVUT_ASSERT_EQUAL("2",capture["b"]->getString());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectEntity,testCapture_3)
{
	MockProjectEntity entity("A_{a,b}^{c,d}","test");
	entity.changeCaptureType("a",CMR_CAPTURE_REQUIRED);
	entity.changeCaptureType("b",CMR_CAPTURE_REQUIRED);
	entity.changeCaptureType("c",CMR_CAPTURE_REQUIRED);
	entity.changeCaptureType("d",CMR_CAPTURE_REQUIRED);

	CMRLatexEntity2 le("A_{1,2,5}^{3,4}");
	
	SVUT_ASSERT_FALSE(entity.match(le));
	
	CMRProjectCaptureMap capture;
	SVUT_ASSERT_THROW(CMRLatexException, entity.capture(le,capture));
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectEntity,testGetShortName)
{
	MockProjectEntity entity("A_{i,j}^2","test");
	SVUT_ASSERT_EQUAL("A",entity.getShortName());
}

/*******************  FUNCTION  *********************/
SVUT_DECLARE_FLAT_TEST(TestProjectEntity,testGetLongName)
{
	MockProjectEntity entity("A_{i,j}^2","test");
	SVUT_ASSERT_EQUAL("test",entity.getLongName());
}

/********************  MACRO  ***********************/
SVUT_USE_DEFAULT_MAIN
