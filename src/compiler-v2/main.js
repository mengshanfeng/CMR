/******************************************************************************/
//import latex parser
var FormulaMatcher = require('./libs/FormulaMatcher.js');
var LatexFormula = require('./libs/LatexFormula.js');
var Project = require("./libs/Project.js");

// /*******************  FUNCTION  *********************/
// exports.testConstructorString = function(test)
// {
// 	var matcher = new FormulaMatcher("abcd^(de)");
// 	matcher.markForCapture('b','std',true,true);
// 	var str = JSON.stringify(matcher.model.exportToIR());
// 	var ref = {"childs":[{"name":"a"},{"name":"[*]"},{"name":"b","tags":{"CMR_MATCHER_CAPTURE":{"optional":true,"wildcard":true}}},{"name":"[*]"},{"name":"c"},{"name":"[*]"},{"name":"d","exponents":[{"childs":[{"name":"()","groupChild":{"childs":[{"name":"d"},{"name":"[*]"},{"name":"e"}]}}]}]}]};
// 	var refstr = JSON.stringify(ref);
// 
// 	test.equals(refstr,str);
// 
// 	test.done();
// }
// 
// /*******************  FUNCTION  *********************/
// exports.testBasicMatch_1 = function(test)
// {
	var matcher = new FormulaMatcher("a");
	var formula = new LatexFormula("a");
	
	console.log(matcher.match(formula));
	
	var project = new Project("test");
	project.addConstant("A","constA","int").loadValuesVector("10 & 20 & 30 & 40");
	
	console.log(project.render("cmr_cpp"));
	
// 	test.done();
// }
