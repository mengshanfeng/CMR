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
function ProjectDefinition(parentContext,latexName,longName,type)
{
	this.longName = longName;
	this.latexName = latexName;
	if (type == undefined)
		this.type = "double";
	else
		this.type = type;
	this.matcher = new FormulaMatcher(latexName);
	this.doc = "";
	this.context = new Context(parentContext);
	this.code = new ProjectCodeTree(this.context,1);
	this.group = "definition";
	this.parameters = [];
	
	this.matcher.markForCapture("i","wildcard",false,true);
	this.matcher.markForCapture("j","wildcard",false,true);
	
	this.context.registerEntity(new ProjectVariable("i","x","int",false));
	this.context.registerEntity(new ProjectVariable("j","y","int",false));
}

/******************* FUNCTION *********************/
ProjectDefinition.prototype.setDoc = function(doc)
{
	if (doc != undefined)
		this.doc = doc;
}

/******************* FUNCTION *********************/
ProjectDefinition.prototype.getCode = function()
{
	return this.code;
}

/******************* FUNCTION *********************/
ProjectDefinition.prototype.addParameter = function(mathName,longName,type,doc)
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
ProjectDefinition.prototype.render = function( templateFactory , codeType , context, latexEntity)
{
	var data = {
		object: this,
		templateFactory: templateFactory
	};
	return templateFactory.render("definition",codeType,data);
}

/********************  GLOBALS  *********************/
module.exports = ProjectDefinition;
