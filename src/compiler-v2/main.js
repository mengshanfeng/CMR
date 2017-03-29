/******************************************************************************/
//import latex parser
var FormulaMatcher = require('./libs/FormulaMatcher.js');
var LatexFormula = require('./libs/LatexFormula.js');
var Project = require("./libs/Project.js");
var xmldoc = require("xmldoc");
var fs = require('fs');

/*******************  FUNCTION  *********************/
function loadCodeBlock(parentCode,parentXml)
{
	parentXml.eachChild(function(c,i,a) {
		switch (c.name) {
			case "declvar":
				var cst = parentCode.addLocalVariable(c.attr["mathname"],c.attr["longname"],c.attr["type"]);
				var dims = c.attr["dims"];
				if (dims == undefined)
					dims = 0;
				//cst.loadValues(c.val,parseInt(dims));
				cst.setDoc(c.attr["doc"]);
				break;
			case "mathstep":
				parentCode.addMathStep(c.val);
				break;
			case "ccode":
				parentCode.addNativeCode(c.val);
				break;
			case "foreach":
				var block = parentCode.addForEach(c.attr["iterator"]);
				loadCodeBlock(block.getCode(),c);
				break;
		}
	});
}

/*******************  FUNCTION  *********************/
function loadProjectXml(project,xml)
{
	//load name
	project.setName(xml.valueWithPath("project.name"));
	
	//load authors
	var authors = xml.descendantWithPath("project.authors");
	authors.eachChild(function(child,index,array) {
		project.addAuthor(child.val,child.attr.mail);
	});
	
	//version
	project.setVersion(xml.valueWithPath("project.version"));
// 	
	//doc
	project.setDoc(xml.valueWithPath("project.doc").toString().trim());
	
	//load consts
	xml.descendantWithPath("elements.consts").eachChild(function(child,index,array) {
		var cst = project.addConstant(child.attr["mathname"],child.attr["longname"],child.attr["type"]);
		var dims = child.attr["dims"];
		if (dims == undefined)
			dims = 0;
		cst.loadValues(child.val,parseInt(dims));
		cst.setDoc(child.attr["doc"]);
	});
	
	//load mesh vars
	//load consts
	xml.descendantWithPath("elements.mesh").eachChild(function(child,index,array) {
		var ghost = child.attr["ghost"];
		if (ghost == undefined)
			ghost = 0;
		var v = project.addMeshVariable(child.attr["mathname"],child.attr["longname"],child.attr["type"], ghost);
		v.setDoc(child.attr["doc"]);
	});
	
	//load definitions
	xml.descendantWithPath("elements.defs").eachChild(function(child,index,array) {
		var def = project.addDefinition(child.attr["mathname"],child.attr["longname"],child.attr["type"]);
		def.setDoc(child.attr["doc"]);
		
		//extract extra parameters
		child.eachChild(function(c,i,a) {
			if (c.name == "defparameter")
				def.addParameter(c.attr["mathname"],c.attr["longname"],c.attr["type"],c.attr["doc"]);
		});
		
		//load inner bolck
		loadCodeBlock(def.getCode(),child);
	});
	
	//load actions
	xml.descendantWithPath("elements.cellactions").eachChild(function(child,index,array) {
		var action = project.addAction(child.attr["name"],child.attr["loop"]);
		action.setDoc(child.attr["doc"]);
		
		//extract extra parameters
		child.eachChild(function(c,i,a) {
			if (c.name == "actionparameter")
				action.addParameter(c.attr["mathname"],c.attr["longname"],c.attr["type"],c.attr["doc"]);
		});
		
		//load inner bolck
		loadCodeBlock(action.getCode(),child);
	});
}

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
	/*project.addAuthor("Sébastien Valat","sebastien.valat@cern.ch");
	project.addHeader("LBMStruct.h");
	project.addConstant("A","constA","int").loadValuesVector("10 & 20 & 30 & 40");
	project.addMeshVariable("d", "density", "int", 1);*/
	
	var projectXml = fs.readFileSync("../../examples/lbm/lbm.cmr.xml");
	var xml = new xmldoc.XmlDocument(projectXml);
	loadProjectXml(project,xml);
	
	console.log(project.render("cmr_cpp"));
	
// 	test.done();
// }
