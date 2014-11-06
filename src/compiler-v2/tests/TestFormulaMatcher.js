/******************************************************************************/
//import latex parser
var FormulaMatcher = require('../libs/FormulaMatcher.js');
var LatexFormula = require('../libs/LatexFormula.js');

/*******************  FUNCTION  *********************/
exports.testConstructorString = function(test)
{
	var matcher = new FormulaMatcher("abcd^(de)");
	matcher.markForCapture('b','std',true,true);
	var str = JSON.stringify(matcher.model.exportToIR());
	var ref = {"childs":[{"name":"a"},{"name":"[*]"},{"name":"b","tags":{"CMR_MATCHER_CAPTURE":{"optional":true,"wildcard":true}}},{"name":"[*]"},{"name":"c"},{"name":"[*]"},{"name":"d","exponents":[{"childs":[{"name":"()","groupChild":{"childs":[{"name":"d"},{"name":"[*]"},{"name":"e"}]}}]}]}]};
	var refstr = JSON.stringify(ref);

	test.equals(refstr,str);

	test.done();
}

/*******************  FUNCTION  *********************/
exports.testBasicMatch_1 = function(test)
{
	var matcher = new FormulaMatcher("a");
	var f1 = new LatexFormula("a");
	var f2 = new LatexFormula("b");
	
	test.equal(JSON.stringify({matched:1,capture:{}}),JSON.stringify(matcher.match(f1)));
	test.equal(JSON.stringify(false),JSON.stringify(matcher.match(f2)));
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testBasicCapture_1 = function(test)
{
	debugger;
	
	var matcher = new FormulaMatcher("a");
	matcher.markForCapture("a","std",true,true);
	var f1 = new LatexFormula("a");
	var f2 = new LatexFormula("b");
	
	test.equal(JSON.stringify({matched:0,capture:{"a":new LatexFormula("a")}}),JSON.stringify(matcher.match(f1)));
	test.equal(JSON.stringify({matched:0,capture:{"a":new LatexFormula("b")}}),JSON.stringify(matcher.match(f2)));
	
	test.done();
}
