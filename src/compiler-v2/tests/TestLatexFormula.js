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
