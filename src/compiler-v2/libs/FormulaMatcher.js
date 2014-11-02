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
var LatexFormula = require('./LatexFormula.js');
var LatexEntity = require('./LatexEntity.js');

/*********************  CLASS  **********************/
function FormulaMatcherFilter()
{
	
}

/*********************  CLASS  **********************/
/**
 * Class to manage formula maching.
 * @param model Define the latex model to match. The captured parameters are defined
 * after initialization with markForCapture() methods.
**/
function FormulaMatcher(model)
{
	//build the latex formula
	this.model = new LatexFormula(model);
}

/*******************  FUNCTION  *********************/
/**
 * Mark the latex entity for capture.
 * @param value Define the latex entity as a latex string.
 * @param filter Define a filter to accept or reject certain kind of element to capture.
 *               It must be a function taking an entity as parameter and returning a boolean.
 *               It can also be a string for some default filters :
 *                 - "member"
 *                 - "operator"
 *                 - "group"
 *                 - "wildcard"
 *                 - "std"
 *                 - "all"
 * @param optional Boolean to define the capture as optional or not
 * @param wildcard Boolean to define if it can capture multiple elements or not.
**/
FormulaMatcher.prototype.markForCapture = function(value,filter,optional,wildcard)
{
	//errors
	assert.ok(value != undefined);
	assert.ok(filter instanceof FormulaMatcherFilter || typeof filter == 'string');
	
	//build the entity
	var entity = new LatexEntity(value);
	
	//if filter is string build the filter object
	if (typeof filter == 'string')
	{
		switch(filter)
		{
			case 'member':
				filter = function(e) {return e.getKind() == 'member';};
				break;
			case 'operator':
				filter = function(e) {return e.getKind() == 'operator';};
				break;
			case 'group':
				filter = function(e) {return e.getKind() == 'group';};
				break;
			case 'wildcard':
				filter = function(e) {return true;};
				break;
			case 'std':
				filter = function(e) {var kind = e.getKind(); return kind == 'member' || kind == 'group';};
				break;
			default:
				assert.fail('Invalid filter type : '+filter);
				break;
		}
	}
	
	//call the internal function
	this.internalMarkForCapture(this.model,entity,filter,optional,wildcard);
	
	//return this to chain
	return this;
}

/*******************  FUNCTION  *********************/
/**
 * Mark the latex entity for capture.
 * @param model Define the model on which to apply the loops to tag the captured object.
 * @param value Define the latex entity as a LatexEntity object.
 * @param filter Define a filter to accept or reject certain kind of element to capture.
 * @param optional Boolean to define the capture as optional or not
 * @param wildcard Boolean to define if it can capture multiple elements or not.
**/
FormulaMatcher.prototype.internalMarkForCapture = function(model,value,filter,optional,wildcard)
{
	var matcher = this;
	
	//check
	assert.ok(value instanceof LatexEntity);
	assert.ok(model instanceof LatexEntity || model instanceof LatexFormula);
	
	//check type
	if (model instanceof LatexFormula)
	{
		//loop on childs
		model.childs.forEach(function(e) {
			matcher.internalMarkForCapture(e,value,filter,optional,wildcard);
		});
	} else if (model instanceof LatexEntity) {
		//check if equal, otherwise, loop on childs
		if (model.equal(value))
		{
			//ok mark it for capture
			model.setTag('CMR_MATCHER_CAPTURE',{filter:filter,optional:optional,wildcard:wildcard});
		} else {
			//loop over childs
			model.forEachChilds(function(f) { matcher.internalMarkForCapture(f,value,filter,optional,wildcard); });
		}
	} else {
		assert.fail("Invalid type of object : "+typeof model);
	}
}

/********************  GLOBALS  *********************/
module.exports = FormulaMatcher;
