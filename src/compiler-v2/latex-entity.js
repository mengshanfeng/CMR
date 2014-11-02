/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 11/2014
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/******************************************************************************/
//import latex parser
var assert = require('assert');

/*********************  CLASS  **********************/
/**
 * Class to represent a latex entity.
**/
function LatexEntity(name)
{
	//setup internal vars
	this.name = name;
	this.exponents = [];
	this.indices = [];
	this.parameters = [];
	this.groupChild = null;
	this.tags = {};
}

/*******************  FUNCTION  *********************/
LatexEntity.prototype.addExponent = function(formula)
{
	//check
	assert.ok(formula instanceof LatexFormula);
	
	//insert
	this.exponents.push(formula);
	
	//return current to chain
	return this;
}

/*******************  FUNCTION  *********************/
LatexEntity.prototype.addIndice = function(formula)
{
	//check
	assert.ok(formula instanceof LatexFormula);
	
	//insert
	this.indices.push(formula);
	
	//return current to chain
	return this;
}

/*******************  FUNCTION  *********************/
LatexEntity.prototype.addParameter = function(formula)
{
	//check
	assert.ok(formula instanceof LatexFormula);
	
	//insert
	this.parameters.push(formula);
	
	//return current to chain
	return this;
}

/*******************  FUNCTION  *********************/
LatexEntity.prototype.setGroupChild = function(formula)
{
	//check
	assert.ok(formula instanceof LatexFormula);
	assert.ok(this.groupChild == null);
	
	//insert
	this.groupChild = formula;
	
	//return current to chain
	return this;
}

/*******************  FUNCTION  *********************/
LatexEntity.prototype.setTag = function(tagName,tagValue)
{
	//setup
	this.tags[tagName] = tagValue;
	
	//return current to chain
	return this;
}

/*******************  FUNCTION  *********************/
function latexFormulaListToString(name,formulaList)
{
	var ret = '';
	formulaList.forEach(function(f) {
		if (ret != '')
			ret += ' , ';
		ret += f.toDebugString();
	});
	if (formulaList.length == 0)
		return '';
	else
		return '->'+name+'( ' + ret + ' )';
}

/*******************  FUNCTION  *********************/
LatexEntity.prototype.toDebugString = function()
{
	var ret = "";
	if (this.name != '()')
		ret += this.name;
	if (this.groupChild != null && this.name == '()')
		ret += '( '+this.groupChild.toDebugString() + ' )';
	ret += latexFormulaListToString('params',this.parameters);
	ret += latexFormulaListToString('exp',this.exponents);
	ret += latexFormulaListToString('ind',this.indices);
	return ret;
}

/*******************  FUNCTION  *********************/
LatexEntity.prototype.loadFromIR = function(irEntity)
{
	var cur = this;
	this.name = irEntity.name;
	if (irEntity.tags != undefined)
		this.tags = irEntity.tags;
	
	//loop on each expo
	if (irEntity.exponents != undefined)
		irEntity.exponents.forEach(function(value) {
			var formula = new LatexFormula();
			formula.loadFromIR(value);
			cur.addExponent(formula);
		});
	
	//loop on each indices
	if (irEntity.indices != undefined)
		irEntity.indices.forEach(function(value) {
			var formula = new LatexFormula();
			formula.loadFromIR(value);
			cur.addIndice(formula);
		});
	
	//loop on each parameters
	if (irEntity.parameters != undefined)
		irEntity.parameters.forEach(function(value) {
			var formula = new LatexFormula();
			formula.loadFromIR(value);
			cur.addParameter(formula);
		});
	
	//loop on each group childs
	if (irEntity.groupChild != undefined)
	{
		var formula = new LatexFormula();
		formula.loadFromIR(irEntity.groupChild);
		cur.setGroupChild(formula);
	}
}

/********************  GLOBALS  *********************/
module.exports = LatexEntity;

/******************************************************************************/
//import latex parser
var LatexFormula  = require('./latex-formula');
