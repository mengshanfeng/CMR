/******************************************************************************/
//import latex parser
var FormulaMatcher = require('../libs/FormulaMatcher.js');

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
