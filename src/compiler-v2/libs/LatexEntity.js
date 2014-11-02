/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 11/2014
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
//import latex parser
var assert = require('assert');

/*********************  CLASS  **********************/
/**
 * Class to represent a latex entity.
 * @param value Optional parameter to build the entitiy from a latex string (must be a uniq element, not a composed formula).
**/
function LatexEntity(value)
{
	//setup internal vars
	this.name = "";
	this.exponents = [];
	this.indices = [];
	this.parameters = [];
	this.groupChild = null;
	this.tags = {};

	//parse and load if a string is provided in Latex format
	if (value != undefined && typeof value == 'string')
	{
		//parse to IR
		var ir = LatexParsor.parse(value);
		assert.ok(ir != undefined,"Invalid parsing of latex : "+value);
		assert.ok(ir.childs.length == 1,"LatexEntity must be initialized with a simple latex entitiy, not with a composed one : "+value);

		//load
		this.loadFromIR(ir.childs[0]);
	}
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
LatexEntity.prototype.forEachChilds = function(handler)
{
	//loop on each expo
	this.exponents.forEach(function(value) {
		handler(value);
	});

	//loop on each indices
	this.indices.forEach(function(value) {
		handler(value);
	});

	//loop on each parameters
	this.parameters.forEach(function(value) {
		handler(value);
	});

	//loop on each group childs
	if (this.groupChild != undefined)
	{
		handler(this.groupChild);
	}
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

/*******************  FUNCTION  *********************/
LatexEntity.prototype.childsEquals = function(a,b)
{
	//quick check of size
	if (a.length != b.length)
		return false;

	//check all content one by one
	for (var i in a)
		if (a[i].equal(b[i]) == false)
			return false;

	//ok all equals
	return true;
}

/*******************  FUNCTION  *********************/
/**
 * Check if two entities are equals (strictly).
**/
LatexEntity.prototype.equal = function(entity)
{
	//check type
	assert.ok(entity instanceof LatexEntity);
	
	//check name
	if (this.name != entity.name)
		return false;
	
	//check 
	if (this.childsEquals(this.indices,entity.indices) == false)
		return false;
	if (this.childsEquals(this.exponents,entity.exponents) == false)
		return false;
	if (this.childsEquals(this.parameters,entity.parameters) == false)
		return false;
	
	//group child
	if (this.groupChild == null && entity.groupChild != null)
		return false;
	if (this.groupChild != null && entity.groupChild == null)
		return false;
	if (this.groupChild != null)
		if (this.groupChild.equal(entity.groupChild) == false)
			return false;

	//ok all equal
	return true;
}

/*******************  FUNCTION  *********************/
var latexEntityOperators = ['*','+','=','/',',','>','<','[*]','[-]'];
LatexEntity.prototype.isOperator = function()
{
	return (latexEntityOperators.indexOf(this.name) >= 0);
}

/*******************  FUNCTION  *********************/
LatexEntity.prototype.isOnlyOneName = function()
{
	var cnt = this.indices.length + this.exponents.length + this.parameters.length;
	if (this.groupChild != null)
		cnt++;
	return cnt == 0;
}

/*******************  FUNCTION  *********************/
LatexEntity.prototype.getKind = function()
{
	if (this.isOperator())
	{
		assert.ok(this.isOnlyOneName());
		return 'operator';
	} else if (this.name == "()" && this.groupChild != null) {
		return 'group';
	} else {
		return 'member';
	}
}

/********************  GLOBALS  *********************/
module.exports = LatexEntity;

/********************  HEADERS  *********************/
//import latex parser
var LatexFormula  = require('./LatexFormula');
var LatexParsor  = require('../build/latex-parser').parser;
