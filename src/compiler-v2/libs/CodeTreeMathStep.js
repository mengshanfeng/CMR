/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 11/2014
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
var LatexEntity = require ("./LatexEntity.js");
var LatexFormula = require ("./LatexFormula.js");
var Context = require('./Context.js');
var assert = require ('assert');

/********************* CLASS **********************/
function CodeTreeMathStep(context,latexStep)
{
	this.context = context;
	
	var left = latexStep.split("=")[0];
	var right = latexStep.split("=")[1];
	
	this.left = new LatexEntity(left);
	this.right = new LatexFormula(right);
}

/******************* FUNCTION *********************/
CodeTreeMathStep.prototype.render = function( templateFactory , codeType , context, latexEntity)
{
	var data = {
		object: this,
		context:context,
	};
	return templateFactory.render("mathstep",codeType,data);
}

/********************  GLOBALS  *********************/
module.exports = CodeTreeMathStep;
