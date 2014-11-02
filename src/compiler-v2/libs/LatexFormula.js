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
	}
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

/********************  GLOBALS  *********************/
//post load latex entity to cut cycles
var LatexEntity  = require('./LatexEntity');
var LatexParsor  = require('../build/latex-parser').parser;
