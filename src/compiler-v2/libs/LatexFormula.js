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
var clone = require('clone');

/*********************  CLASS  **********************/
function FormulaIterator()
{
	this.position = 0;
};

/*******************  FUNCTION  *********************/
FormulaIterator.prototype.diff = function(it)
{
	assert.ok(it instanceof FormulaIterator);
	return this.position - it.position;
}

/*******************  FUNCTION  *********************/
FormulaIterator.prototype.move = function(delta)
{
	this.position += delta;
	return this;
}

/*********************  CLASS  **********************/
/**
 * Object representation of a latex formula, mostly composed
 * of a list of LatexEntity as childs stored into this.childs.
 * @param value Optional parameter to provide a formula as a latex string
**/
function LatexFormula(value)
{
	this.childs = [];
	this.tags = {};
	
	//parse and load if a string is provided in Latex format
	if (value != undefined && typeof value == 'string')
	{
		//parse to IR
		var ir = LatexParsor.parse(value);
		assert.ok(ir != undefined);

		//load
		this.loadFromIR(ir);
	} else if (value instanceof LatexEntity) {
		this.addChildEntity(value);
	}
}

/*******************  FUNCTION  *********************/
LatexFormula.prototype.begin = function()
{
	return new FormulaIterator();
}

/*******************  FUNCTION  *********************/
LatexFormula.prototype.end = function()
{
	var tmp = new FormulaIterator();
	tmp.position = this.childs.length;
	return tmp;
}

/*******************  FUNCTION  *********************/
/**
 * Insert a new LatexEntity as child.
 * @param entity The latex entity to insert.
**/
LatexFormula.prototype.addChildEntity = function(entity)
{
	//vars
	var toInsert;
	
	//check
	assert.ok(entity instanceof LatexEntity);
	
	//insert
	this.childs.push(entity)
	
	//return current to chain
	return this;
}

/*******************  FUNCTION  *********************/
/**
 * Help to convert the LatexFormula to debug string for unit tests.
**/
LatexFormula.prototype.toDebugString = function()
{
	//vars
	var ret = '';
	
	//loop in all chidls
	this.childs.forEach(function(value) {
		if (ret != '')
			ret += ' ';
		ret += value.toDebugString();
	});
	
	//return
	return ret;
}

/*******************  FUNCTION  *********************/
LatexFormula.prototype.toLatexString = function()
{
	//vars
	var ret = '';
	
	//loop in all chidls
	this.childs.forEach(function(value) {
		ret += value.toDebugString();
	});
	
	//return
	return ret;
}

/*******************  FUNCTION  *********************/
/**
 * Short function to export a simplified view of the formula (IR representation).
 * Its only for unit tests to only get non empty fields while using stringify methods.
**/
LatexFormula.prototype.exportToIR = function()
{
	var ret = {
		childs:[]
	};
	
	//convert childs
	this.childs.forEach(function(c) {
		ret.childs.push(c.exportToIR());
	});
	
	//tags (cloned via slice)
	if (Object.keys(this.tags).length > 0)
		ret.tags = clone(this.tags);
	
	//export
	return ret;
}

/*******************  FUNCTION  *********************/
LatexFormula.prototype.setTag = function(tagName,tagValue)
{
	//setup
	this.tags[tagName] = tagValue;
	
	//return current to chain
	return this;
}

/*******************  FUNCTION  *********************/
/**
 * Load the object formula from the IR reprensentation. In practive, the IR
 * has the same representation without to object functions attached.
**/
LatexFormula.prototype.loadFromIR = function(irFormula)
{
	//keep track of this
	var formula = this;

	//loop on each childs
	irFormula.childs.forEach(function(value) {
		var entity = new LatexEntity();
		entity.loadFromIR(value);
		formula.addChildEntity(entity);
	});

	if (irFormula.tags != undefined)
		this.tags = irFormula.tags;
}

/*******************  FUNCTION  *********************/
/**
 * Check if a given formula is equal (stricly) to the current one.
**/
LatexFormula.prototype.equal = function(formula)
{
	//check
	assert.ok(formula instanceof LatexFormula);
	
	//check child number
	if (this.childs.length != formula.childs.length)
		return false;
	
	//check all childs one by one
	for (var i in this.childs)
		if (this.childs[i].equal(formula.childs[i]) == false)
			return false;
	
	//ok all equal
	return true;
}

/********************  GLOBALS  *********************/
module.exports = LatexFormula;

/********************  HEADERS  *********************/
//post load latex entity to cut cycles
var LatexEntity  = require('./LatexEntity');
var LatexParsor  = require('../build/latex-parser').parser;
