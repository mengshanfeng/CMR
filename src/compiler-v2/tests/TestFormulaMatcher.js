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
exports.testFromOldVersionMatchCapture_1 = function(test)
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

/*******************  FUNCTION  *********************/
exports.testFromOldVersionMarchCapture_2 = function(test)
{
	var matcher = new FormulaMatcher("a^b");
	matcher.markForCapture('b');
	
	var ok1 = new LatexFormula("a^3");
	var nok1 = new LatexFormula("a^3+2");
	var nok2 = new LatexFormula("b^3");
	var nok3 = new LatexFormula("b^2");
	var nok4 = new LatexFormula("c^{3+2}+2");
	
	test.ok(matcher.match(ok1) != false);
	test.equal(false,matcher.match(nok1));
	test.equal(false,matcher.match(nok2));
	test.equal(false,matcher.match(nok3));
	test.equal(false,matcher.match(nok4));
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testFromOldVersionMatchCapture_3 = function(test)
{
	var matcher =  new FormulaMatcher("a_{i+\\frac{1}{2}}");
	matcher.markForCapture("i");
	
	var test1 = new LatexFormula("a_{k+\\frac{1}{2}}");
	var test2 = new LatexFormula("a_{j+\\frac{1}{2}}");
	var test3 = new LatexFormula("a_{j+1+\\frac{1}{2}}");
	var test4 = new LatexFormula("a_{(j+1)+\\frac{1}{2}}");
	var test5 = new LatexFormula("b_{i+\\frac{1}{2}}");
	var test6 = new LatexFormula("a_{i+\\frac{1}{4}}");
	
	test.ok(matcher.match(test1));
	test.ok(matcher.match(test2));
	test.ok(!matcher.match(test3));
	test.ok(matcher.match(test4));
	test.ok(!matcher.match(test5));
	test.ok(!matcher.match(test6));
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testFomrOldVersionMatchCapture_4 = function(test)
{
	var matcher =  new FormulaMatcher("x^y");
	matcher.markForCapture("x");
	matcher.markForCapture("y");
	
	var test1 = new LatexFormula("a^2");
	var test2 = new LatexFormula("b^c");
	var test3 = new LatexFormula("a^{2,4}");
	
	test.ok(matcher.match(test1));
	test.ok(matcher.match(test2));
	test.ok(!matcher.match(test3));
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testFromOldVersionMatchCapture_5 = function(test)
{
	var matcher = new FormulaMatcher("xyz");
	matcher.markForCapture("x");
	matcher.markForCapture("y");
	matcher.markForCapture("z");
	
	var test1 = new LatexFormula("a+b");
	var test2 = new LatexFormula("abc");
	var test3 = new LatexFormula("a+cd");
	var test4 = new LatexFormula("a*b*c");
	
	test.ok(!matcher.match(test1));
	test.ok(matcher.match(test2));
	test.ok(!matcher.match(test3));
	test.ok(!matcher.match(test4));
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testFromOldVersionMatchCapture_6 = function(test)
{
	var matcher = new FormulaMatcher("x+z");
	matcher.markForCapture("x");
	matcher.markForCapture("+",'operator');
	matcher.markForCapture("z");
	
	var test1 = new LatexFormula("a+b");
	var test2 = new LatexFormula("abc");
	var test3 = new LatexFormula("a+cd");
	var test4 = new LatexFormula("a*b*c");
	
	test.ok(matcher.match(test1));
	test.ok(!matcher.match(test2));
	test.ok(!matcher.match(test3));
	test.ok(!matcher.match(test4));
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testFromOldVersionMiddleMatch_1 = function(test)
{
	var matcher = new FormulaMatcher("a^b");
	matcher.markForCapture("b");
	
	var test1 = new LatexFormula("a^3");
	
	var it = test1.begin();
	test.ok(matcher.match(test1,{},it));
	test.equal(0,it.diff(test1.end()));
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testFromOldVersionMiddleMatch_2 = function(test)
{
	var matcher = new FormulaMatcher("a^b");
	matcher.markForCapture("b");
	
	var test2 = new LatexFormula("cc+a^3");
	var test3 = new LatexFormula("dd+a^(3+2)+rerer");
	
	var it = test2.begin();
	var itRef = test2.begin();
	test.ok(!matcher.match(test2,{},it));
	test.equal(0,it.diff(itRef) );
	it = test2.begin().move(4);
	test.ok(matcher.match(test2,{},it));
	test.equal(5,it.diff(itRef) );
	test.equal(0,it.diff(test2.end()) );

	test.done();
}


/*******************  FUNCTION  *********************/
exports.testFromOldVersionMiddleMatch_3 = function(test)
{
	var matcher = new FormulaMatcher("a^b");
	matcher.markForCapture("b");

	var test3 = new LatexFormula("dd+a^(3+2)+rerer");
	
	var it = test3.begin();
	test.ok(!matcher.match(test3,{rootPartial:true},it));
	test.equal(0,it.diff(test3.begin()));
	it = test3.begin().move(4);
	test.ok(matcher.match(test3,{rootPartial:true},it));
	test.equal(5,it.diff(test3.begin()));
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testFromOldVersionToDebugString = function(test)
{
	var matcher = new FormulaMatcher("d d + a^( 3 + 2 ) + r ");
	out = matcher.toDebugString();
	test.equal("d [*] d + a->exp( ( 3 + 2 ) ) + r",out);
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testFromOldVersionToDebugString = function(test)
{
	var matcher = new FormulaMatcher("d d + a^( 3 + 2 ) + r ");
	out = matcher.toDebugString();
	test.equal("d [*] d + a->exp( ( 3 + 2 ) ) + r",out);
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testFromOldVersionOptionalExponent_1 = function(test)
{
	var matcher = new FormulaMatcher("a^b");
	matcher.markForCapture("b","std",true);
	
	var test1 = new LatexFormula("a");
	var test2 = new LatexFormula("a^16");
	var test3 = new LatexFormula("a^{16,18}");
	
	test.ok(matcher.match(test1));
	test.ok(matcher.match(test2));
	test.ok(!matcher.match(test3));
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testFromOldVersionOptionalExponent_2 = function(test)
{
	var matcher = new FormulaMatcher("a");
	matcher.markOptionalExponent();
	
	var test1 = new LatexFormula("a");
	var test2 = new LatexFormula("a^16");
	var test3 = new LatexFormula("a^{16,18}");

	test.ok(matcher.match(test1));
	test.ok(matcher.match(test2));
	test.ok(!matcher.match(test3));
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testFromOldVersionOptionalExponent_3 = function(test)
{
	var matcher = new FormulaMatcher("(ab)");
	matcher.markOptionalExponent();
	
	var test1 = new LatexFormula("(ab)");
	var test2 = new LatexFormula("(ab)^16");
	var test3 = new LatexFormula("(ab)^{16,18}");
	
	test.ok(matcher.match(test1));
	test.ok(matcher.match(test2));
	test.ok(!matcher.match(test3));
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testFromOldVersionOptionalExponent_3 = function(test)
{
	var matcher = new FormulaMatcher("ab");
	test.throws(function(){matcher.markOptionalExponent()},Error);
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testFromOldVersionCaptureOptionalExponent_1 = function(test)
{
	var matcher = new FormulaMatcher("a");
	matcher.markOptionalExponent();
	
	//check matching
	var value = new LatexFormula("a");
	var status = matcher.capture(value);
	
	test.ok(status.capture["\\CMRExponent"] == undefined);
	test.equal(1,status.matched);
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testFromOldVersionCaptureOptionalExponent_2 = function(test)
{
	var matcher = new FormulaMatcher("a");
	matcher.markOptionalExponent();
	
	//check matching
	var value = new LatexFormula("a^3");
	var status = matcher.capture(value);
	
	test.equal('3',status.capture["\\CMRExponent"].toDebugString());
	test.equal(1,status.matched);
	
	test.done();
}

exports.testFromOldVersionCaptureOptionalExponent_3 = function(test)
{
	var matcher = new FormulaMatcher("a");
	matcher.markOptionalExponent();
	
	//check matching
	var value = new LatexFormula("a^{3,2}");
	var status = matcher.capture(value);
	
	test.ok(status === false);
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testCaptureOptionalCapture1 = function(test)
{
	var matcher = new FormulaMatcher("a^e");
	matcher.markForCapture('e');
	
	//check matching
	var value = new LatexFormula("a^3");
	var status = matcher.capture(value);
	
	test.equal('3',status.capture["e"].toDebugString());
	test.equal(1,status.matched);
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testCaptureOptionalCapture2 = function(test)
{
	var matcher = new FormulaMatcher("a^e");
	matcher.markForCapture('e');
	
	//check matching
	var value = new LatexFormula("a");
	var status = matcher.capture(value);
	
	test.ok(status === false);
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testCaptureWildcard0 = function(test)
{
	var matcher = new FormulaMatcher("a_{x+\\frac{1}{2}}");
	matcher.markForCapture('x');
	
	//check matching
	var value = new LatexFormula("a_{i+\\frac{1}{2}}");
	var status = matcher.capture(value);
	
	test.ok(status.capture["x"] != undefined);
	test.equal('i',status.capture["x"].toDebugString());
	test.equal(5,status.matched);
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testCaptureWildcard1 = function(test)
{
	var matcher = new FormulaMatcher("a_{x+\\frac{1}{2}}");
	matcher.markForCapture('x','std',false,true);
	
	//check matching
	var value = new LatexFormula("a_{i+\\frac{1}{2}}");
	var status = matcher.capture(value);
	
	test.ok(status.capture["x"] != undefined);
	test.equal('i',status.capture["x"].toDebugString());
	test.equal(5,status.matched);
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testCaptureWildcard2 = function(test)
{
	var matcher = new FormulaMatcher("a_{x+\\frac{1}{2}}");
	matcher.markForCapture('x','wildcard',false,true);
	
	//check matching
	var value = new LatexFormula("a_{i+\\frac{2}{2}}");
	var status = matcher.capture(value);
	test.ok(!status);
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testCaptureWildcard3 = function(test)
{
	var matcher = new FormulaMatcher("a_{x+\\frac{1}{2}}");
	matcher.markForCapture('x','wildcard',false,true);
	
	//check matching
	var value = new LatexFormula("a_{\\frac{2}{2}}");
	var status = matcher.capture(value);
	test.ok(!status);
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testCaptureWildcard3 = function(test)
{
	var matcher = new FormulaMatcher("a_{x+\\frac{1}{2}}");
	matcher.markForCapture('x','wildcard',true,true);
	
	//check matching
	var value = new LatexFormula("a_{\\frac{2}{2}}");
	var status = matcher.capture(value);
	test.ok(!status);
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testCaptureWildcard5 = function(test)
{
	var matcher = new FormulaMatcher("a_{x+\\frac{1}{2}}");
	matcher.markForCapture('x','wildcard',false,true);
	
	//check matching
	var value = new LatexFormula("a_{i+\\frac{1}{2}}");
	var status = matcher.capture(value);
	
	test.ok(status.capture["x"] != undefined);
	test.equal('i',status.capture["x"].toDebugString());
	test.equal(5,status.matched);
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testCaptureWildcard6 = function(test)
{
	var matcher = new FormulaMatcher("a_{x}");
	matcher.markForCapture('x','wildcard',false,true);
	
	//check matching
	var value = new LatexFormula("a_{i+\\frac{1}{2}}");
	var status = matcher.capture(value);
	
	test.ok(status != false);
	test.ok(status.capture["x"] != undefined);
	test.equal('i + \\frac->params( 1 , 2 )',status.capture["x"].toDebugString());
	test.equal(1,status.matched);
	
	test.done();
}

/*******************  FUNCTION  *********************/
exports.testCaptureWithParams = function(test)
{
	var matcher = new FormulaMatcher("a_{x}");
	matcher.markForCapture('x');
	
	//check matching
	var value = new LatexFormula("a_{\\frac{1}{2}}");
	debugger;
	var status = matcher.capture(value);
	
	test.ok(status != false);
	test.ok(status.capture["x"] != undefined);
	test.equal('\\frac->params( 1 , 2 )',status.capture["x"].toDebugString());
	test.equal(1,status.matched);
	
	test.done();
}


