/******************************************************************************/
//import latex parser
var FormulaMatcher = require('../libs/FormulaMatcher.js');

/*******************  FUNCTION  *********************/
exports.testConstructorString = function(test)
{
	var matcher = new FormulaMatcher("abcd^(de)");
	matcher.markForCapture('b','std',true,true);
	var str = JSON.stringify(matcher);
	test.equals("a",str);
	test.done();
}
