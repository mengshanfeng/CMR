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
	var matcher = new FormulaMatcher("a");
	matcher.markForCapture("a","std",true,true);
	var f1 = new LatexFormula("a");
	var f2 = new LatexFormula("b");
	
	test.equal(JSON.stringify({matched:0,capture:{"a":new LatexFormula("a")}}),JSON.stringify(matcher.match(f1)));
	test.equal(JSON.stringify({matched:0,capture:{"a":new LatexFormula("b")}}),JSON.stringify(matcher.match(f2)));
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testFromOldVersionMarkForCapture_1 = function(test)
{
	var matcher = new FormulaMatcher("a^{3,5}+c+v_{i+\\frac{1}{2}}");
	matcher.markForCapture('c','std',false,false);
	var str = JSON.stringify(matcher.model.exportToIR());
	var ref = {"childs":[{"name":"a","exponents":[{"childs":[{"name":"3"}]},{"childs":[{"name":"5"}]}]},{"name":"+"},{"name":"c","tags":{"CMR_MATCHER_CAPTURE":{"optional":false,"wildcard":false}}},{"name":"+"},{"name":"v","indices":[{"childs":[{"name":"i"},{"name":"+"},{"name":"\\frac","parameters":[{"childs":[{"name":"1"}]},{"childs":[{"name":"2"}]}]}]}]}]};
	var refstr = JSON.stringify(ref);

	test.equals(refstr,str);

	test.done();
}

/*******************  FUNCTION  *********************/
//TODO currently capture nothing, but need to check if we can capture the name only in this case and remove the exponents on matching.
exports.testFromOldVersionMarkForCapture_2 = function(test)
{
	var matcher = new FormulaMatcher("a^{3,5}+c+v_{i+\\frac{1}{2}}");
	matcher.markForCapture('a','std',false,false);
	var str = JSON.stringify(matcher.model.exportToIR());
	var ref = {"childs":[{"name":"a","exponents":[{"childs":[{"name":"3"}]},{"childs":[{"name":"5"}]}],"tags":{"CMR_MATCHER_CAPTURE":{"optional":false,"wildcard":false}}},{"name":"+"},{"name":"c"},{"name":"+"},{"name":"v","indices":[{"childs":[{"name":"i"},{"name":"+"},{"name":"\\frac","parameters":[{"childs":[{"name":"1"}]},{"childs":[{"name":"2"}]}]}]}]}]};
	var refstr = JSON.stringify(ref);

	test.equals(refstr,str);

	test.done();
}

/*******************  FUNCTION  *********************/
exports.testFromOldVersionMarkForCapture_3 = function(test)
{
	var matcher = new FormulaMatcher("a^{3,5}+c+v_{i+\\frac{1}{2}}");
	matcher.markForCapture('i','std',false,false);
	var str = JSON.stringify(matcher.model.exportToIR());
	var ref = {"childs":[{"name":"a","exponents":[{"childs":[{"name":"3"}]},{"childs":[{"name":"5"}]}]},{"name":"+"},{"name":"c"},{"name":"+"},{"name":"v","indices":[{"childs":[{"name":"i","tags":{"CMR_MATCHER_CAPTURE":{"optional":false,"wildcard":false}}},{"name":"+"},{"name":"\\frac","parameters":[{"childs":[{"name":"1"}]},{"childs":[{"name":"2"}]}]}]}]}]};
	var refstr = JSON.stringify(ref);

	test.equals(refstr,str);

	test.done();
}

/*******************  FUNCTION  *********************/
//TODO currently capture nothing, but need to check if we can capture the name only in this case and remove the exponents on matching.
exports.testFromOldVersionMarkForCapture_4 = function(test)
{
	var matcher = new FormulaMatcher("a^{3,5}+c+v_{i+\\frac{1}{2}}");
	matcher.markForCapture('v','std',false,false);
	var str = JSON.stringify(matcher.model.exportToIR());
	var ref = {"childs":[{"name":"a","exponents":[{"childs":[{"name":"3"}]},{"childs":[{"name":"5"}]}]},{"name":"+"},{"name":"c"},{"name":"+"},{"name":"v","indices":[{"childs":[{"name":"i"},{"name":"+"},{"name":"\\frac","parameters":[{"childs":[{"name":"1"}]},{"childs":[{"name":"2"}]}]}]}],"tags":{"CMR_MATCHER_CAPTURE":{"optional":false,"wildcard":false}}}]};
	var refstr = JSON.stringify(ref);

	test.equals(refstr,str);

	test.done();
}

/*******************  FUNCTION  *********************/
exports.testFromOldVersionMarkForCapture_5 = function(test)
{
	var matcher = new FormulaMatcher("a^{3,5}+c+v_{i+\\frac{d*u^k}{2}}");
	matcher.markForCapture('d','std',false,false);
	var str = JSON.stringify(matcher.model.exportToIR());
	var ref = {"childs":[{"name":"a","exponents":[{"childs":[{"name":"3"}]},{"childs":[{"name":"5"}]}]},{"name":"+"},{"name":"c"},{"name":"+"},{"name":"v","indices":[{"childs":[{"name":"i"},{"name":"+"},{"name":"\\frac","parameters":[{"childs":[{"name":"d","tags":{"CMR_MATCHER_CAPTURE":{"optional":false,"wildcard":false}}},{"name":"*"},{"name":"u","exponents":[{"childs":[{"name":"k"}]}]}]},{"childs":[{"name":"2"}]}]}]}]}]};
	var refstr = JSON.stringify(ref);

	test.equals(refstr,str);

	test.done();
}

/*******************  FUNCTION  *********************/
exports.testFromOldVersionMarkForCapture_6 = function(test)
{
	var matcher = new FormulaMatcher("a^{3,5}+c+v_{i+\\frac{d*u^k}{2}}");
	matcher.markForCapture('k','std',false,false);
	var str = JSON.stringify(matcher.model.exportToIR());
	var ref = {"childs":[{"name":"a","exponents":[{"childs":[{"name":"3"}]},{"childs":[{"name":"5"}]}]},{"name":"+"},{"name":"c"},{"name":"+"},{"name":"v","indices":[{"childs":[{"name":"i"},{"name":"+"},{"name":"\\frac","parameters":[{"childs":[{"name":"d"},{"name":"*"},{"name":"u","exponents":[{"childs":[{"name":"k","tags":{"CMR_MATCHER_CAPTURE":{"optional":false,"wildcard":false}}}]}]}]},{"childs":[{"name":"2"}]}]}]}]}]};
	var refstr = JSON.stringify(ref);

	test.equals(refstr,str);

	test.done();
}

/*******************  FUNCTION  *********************/
exports.testFromOldVersionMarkForCapture_7 = function(test)
{
	var matcher = new FormulaMatcher("x^y");
	matcher.markForCapture('x','std',false,false);
	matcher.markForCapture('y','std',false,false);
	var str = JSON.stringify(matcher.model.exportToIR());
	var ref = {"childs":[{"name":"x","exponents":[{"childs":[{"name":"y","tags":{"CMR_MATCHER_CAPTURE":{"optional":false,"wildcard":false}}}]}],"tags":{"CMR_MATCHER_CAPTURE":{"optional":false,"wildcard":false}}}]};
	var refstr = JSON.stringify(ref);

	test.equals(refstr,str);

	test.done();
}

/*******************  FUNCTION  *********************/
exports.testFromOldVersionMarchExact_1 = function(test)
{
	var matcher = new FormulaMatcher("a^3");
	
	var ok = new LatexFormula("a^3");
	var nok1 = new LatexFormula("a^3+2");
	var nok2 = new LatexFormula("a^2");
	var nok3 = new LatexFormula("b^3");
	
	test.ok(matcher.match(ok) != false);
	test.equal(false,matcher.match(nok1));
	test.equal(false,matcher.match(nok2));
	test.equal(false,matcher.match(nok3));
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testFromOldVersionMarchExact_2 = function(test)
{
	var matcher = new FormulaMatcher("a^3+b^2");
	
	var ok = new LatexFormula("a^3+b^2");
	var nok1 = new LatexFormula("a^3+b^2+c");
	var nok2 = new LatexFormula("a^2+b^2");
	var nok3 = new LatexFormula("b^2");
	var nok4 = new LatexFormula("a^3+b^3");
	
	test.ok(matcher.match(ok) != false);
	test.equal(false,matcher.match(nok1));
	test.equal(false,matcher.match(nok2));
	test.equal(false,matcher.match(nok3));
	test.equal(false,matcher.match(nok4));
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testFromOldVersionMarchPartialRoot_1 = function(test)
{
	var matcher = new FormulaMatcher("a^3");
	
	var ok1 = new LatexFormula("a^3");
	var ok2 = new LatexFormula("a^3+2");
	var nok1 = new LatexFormula("a^2");
	var nok2 = new LatexFormula("b^3");
	
	var options = {rootPartial:true};
	test.ok(matcher.match(ok1,options) != false);
	test.ok(matcher.match(ok2,options) != false);
	test.equal(false,matcher.match(nok1,options));
	test.equal(false,matcher.match(nok2,options));
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testFromOldVersionMarchCapture_1 = function(test)
{
	var matcher = new FormulaMatcher("a^3");
	matcher.markForCapture('a');
	
	var ok1 = new LatexFormula("a^3");
	var ok2 = new LatexFormula("b^3");
	var nok1 = new LatexFormula("a^3+2");
	var nok2 = new LatexFormula("b^2");
	var nok3 = new LatexFormula("c^{3+2}+2");
	
	test.ok(matcher.match(ok1) != false);
	test.ok(matcher.match(ok2) != false);
	test.equal(false,matcher.match(nok1));
	test.equal(false,matcher.match(nok2));
	test.equal(false,matcher.match(nok3));
	
	test.done();
}
