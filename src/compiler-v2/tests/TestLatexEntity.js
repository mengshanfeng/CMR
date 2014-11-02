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

/*******************  FUNCTION  *********************/
exports.testIsOperator = function(test)
{
	var e1 = new LatexEntity("+");
	var e2 = new LatexEntity("-");
	var e3 = new LatexEntity("a");
	
	test.ok(e1.isOperator());
	test.ok(e2.isOperator());
	test.ok(e3.isOperator());
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testIsonlyOneName = function(test)
{
	var e1 = new LatexEntity("a");
	var e2 = new LatexEntity("b");
	var e3 = new LatexEntity("\\cos");
	var e4 = new LatexEntity("a^2");
	var e5 = new LatexEntity("a_2");
	
	test.ok(e1.isOnlyOneName());
	test.ok(e2.isOnlyOneName());
	test.ok(e3.isOnlyOneName());
	test.ok(e4.isOnlyOneName());
	test.ok(e5.isOnlyOneName());
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testGetKind = function(test)
{
	var e1 = new LatexEntity("a");
	var e1 = new LatexEntity("a^2");
	var e2 = new LatexEntity("+");
	var e3 = new LatexEntity("(a+b)");
	
	test.equal('member',e1.getKind());
	test.equal('member',e2.getKind());
	test.equal('operator',e3.getKind());
	test.equal('group',e4.getKind());
	
	test.done();
}
