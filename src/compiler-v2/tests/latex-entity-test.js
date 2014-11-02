/******************************************************************************/
//import latex parser
var LatexEntity = require('../libs/LatexEntity');

/*******************  FUNCTION  *********************/
exports.testConstructorString = function(test)
{
	var entity = new LatexEntity("a");
	var str = entity.toDebugString();
	test.equals("a",str);
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testConstructorEmpty = function(test)
{
	var entity = new LatexEntity();
	var str = entity.toDebugString();
	test.equals("",str);
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testEqual_only_name = function(test)
{
	var e1 = new LatexEntity("a");
	var e2 = new LatexEntity("a");
	var e3 = new LatexEntity("b");
	
	test.ok(e1.equal(e2) == true);
	test.ok(e1.equal(e3) == false);
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testEqual_expo = function(test)
{
	var e1 = new LatexEntity("a^2_c");
	var e2 = new LatexEntity("a^2_c");
	var e3 = new LatexEntity("b^3_c");
	
	test.ok(e1.equal(e2) == true);
	test.ok(e1.equal(e3) == false);
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testEqual_expo = function(test)
{
	var e1 = new LatexEntity("(a^{2+ad}_c-(78a))");
	var e2 = new LatexEntity("(a^{2+ad}_c-(78a))");
	var e3 = new LatexEntity("(a^{2+ad}_c-(78d))");
	
	test.ok(e1.equal(e2) == true);
	test.ok(e1.equal(e3) == false);
	
	test.done();
}
