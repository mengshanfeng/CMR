/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 11/2014
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
var LatexEntity = require ("./LatexEntity.js");
var Context = require('./Context.js');
var assert = require ('assert');

/********************* CLASS **********************/
function CodeTreeForLoop(codeType,context,indent,iterator)
{
	this.iterator = new LatexEntity(iterator);
	this.context = context;
	this.code = new codeType(context,1);
}

/******************* FUNCTION *********************/
CodeTreeForLoop.prototype.getCode = function()
{
	return this.code;
}

/******************* FUNCTION *********************/
CodeTreeForLoop.prototype.render = function( templateFactory , codeType , context, latexEntity)
{
	var data = {
		object: this,
		context:context,
		templateFactory:templateFactory
	};
	return templateFactory.render("forloop",codeType,data);
}

/********************  GLOBALS  *********************/
module.exports = CodeTreeForLoop;

