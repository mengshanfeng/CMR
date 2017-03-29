/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 11/2014
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
var TemplateFactory = require('./TemplateFactory.js');
var ProjectCodeTree = require('./ProjectCodeTree.js');
var FormulaMatcher = require('./FormulaMatcher.js');
var ProjectVariable = require('./ProjectVariable.js');
var Context = require('./Context.js');
var assert = require('assert');

/******************* FUNCTION *********************/
function ProjectAction(parentContext,name,type)
{
	this.name = name;
	this.type = type;
	this.doc = "";
	this.context = new Context(parentContext);
	this.code = new ProjectCodeTree(this.context,2);
	this.group = "action";
	this.parameters = [];
	
	this.context.registerEntity(new ProjectVariable("i","x","int",false));
	this.context.registerEntity(new ProjectVariable("j","y","int",false));
}

/******************* FUNCTION *********************/
ProjectAction.prototype.setDoc = function(doc)
{
	if (doc != undefined)
		this.doc = doc;
}

/******************* FUNCTION *********************/
ProjectAction.prototype.getCode = function()
{
	return this.code;
}

/******************* FUNCTION *********************/
ProjectAction.prototype.addParameter = function(mathName,longName,type,doc)
{
	//add to list
	this.parameters.push({
		mathName:mathName,
		longName:longName,
		type:type,
		doc:doc
	});
	
	//matcher
	this.matcher.markForCapture(mathName,"wildcard",false,true);
}

/*******************  FUNCTION  *********************/
ProjectAction.prototype.render = function( templateFactory , codeType , context, latexEntity)
{
	var data = {
		object: this,
		templateFactory: templateFactory
	};
	return templateFactory.render("action",codeType+"_"+this.type,data);
}

/********************  GLOBALS  *********************/
module.exports = ProjectAction;
