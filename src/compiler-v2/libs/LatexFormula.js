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
**/
function LatexFormula()
{
	this.childs = [];
	this.tags = {};
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

/********************  GLOBALS  *********************/
module.exports = LatexFormula;

/********************  GLOBALS  *********************/
//post load latex entity to cut cycles
var LatexEntity  = require('./LatexEntity');
