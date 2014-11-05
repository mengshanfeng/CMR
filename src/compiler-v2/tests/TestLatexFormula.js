/******************************************************************************/
//import latex parser
var LatexFormula = require('../libs/LatexFormula');

/*******************  FUNCTION  *********************/
exports.testConstructorString = function(test)
{
	var formula = new LatexFormula("abc");
	var str = formula.toDebugString();
	test.equals("a [*] b [*] c",str);
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testConstructorEmpty = function(test)
{
	var formula = new LatexFormula();
	var str = formula.toDebugString();
	test.equals("",str);
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testEquals = function(test)
{
	var f1 = new LatexFormula("abcd");
	var f2 = new LatexFormula("abcd");
	var f3 = new LatexFormula("abc");
	var f4 = new LatexFormula("abce");
	
	test.ok(f1.equal(f2) == true);
	test.ok(f1.equal(f3) == false);
	test.ok(f1.equal(f4) == false);
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testExportToIR = function(test)
{
	var formula = new LatexFormula("a^2+b_3+(ab)");
	formula.setTag('TEST','test');
	var ref    = {"childs":[{"name":"a","exponents":[{"childs":[{"name":"2"}]}]},{"name":"+"},{"name":"b","indices":[{"childs":[{"name":"3"}]}]},{"name":"+"},{"name":"()","groupChild":{"childs":[{"name":"a"},{"name":"[*]"},{"name":"b"}]}}],"tags":{"TEST":"test"}};
	var refstr = JSON.stringify(ref);
	var ir     = JSON.stringify(formula.exportToIR());
	
	test.equal(refstr,ir);
	
	test.done();
}
