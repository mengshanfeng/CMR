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
function CodeTreeNativeCode(context,code)
{
	this.context = context;
	this.formula = new LatexFormula();
	
	var parts = code.split("$");
	for (var i in parts) {
		if (i % 2 == 0) {
			var entry = new LatexEntity("\\mathrm");
			entry.mathrm = parts[i];
			this.formula.childs.push(entry);
		} else {
			this.formula.childs.push(new LatexFormula(parts[i]));
		}
	}
}

/******************* FUNCTION *********************/
CodeTreeNativeCode.prototype.render = function( templateFactory , codeType , context, latexEntity)
{
	var data = {
		object: this,
		context:context,
	};
	return templateFactory.render("native",codeType,data);
}

/********************  GLOBALS  *********************/
module.exports = CodeTreeNativeCode;
