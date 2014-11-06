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
	var cnt = 0;
	
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
			//inc cnt
			cnt++;
		} else {
			//loop over childs
			model.forEachChilds(function(f) { cnt += matcher.internalMarkForCapture(f,value,filter,optional,wildcard); });
		}
	} else {
		assert.fail("Invalid type of object : "+typeof model);
	}
	
	//check cnt, need to get capture only once, multiple is forbidden
	assert.ok(cnt == 0 || cnt == 1,"Capture must be limited to only one element, get "+cnt+" for variable "+model.toDebugString());
	
	//return ref counter to check uniq in the whole tree
	return cnt;
}

/*******************  FUNCTION  *********************/
FormulaMatcher.prototype.match = function(formula)
{
	//prepare capture
	var status = {
		matched:0,
		capture:{}
	};
	
	//run
	var res = this.matchFormula(this.model,formula,status);
	
	//ok return
	if (res)
		return status;
	else
		return false;
}

/*******************  FUNCTION  *********************/
FormulaMatcher.prototype.isWildcardEntity = function(entity)
{
	//check type
	assert.ok(entity instanceof LatexEntity);
	
	//check if has tags
	var tags = entity.tags['CMR_MATCHER_CAPTURE'];
	if (tags == undefined)
		return false;
	
	//return status
	return tags.wildcard;
}

/*******************  FUNCTION  *********************/
/**
 * 
**/
FormulaMatcher.prototype.matchFormula = function(model,formula,status)
{
	//check type
	assert.ok(formula instanceof LatexFormula);
	assert.ok(model instanceof LatexFormula);
	
	//trivial
	if (formula.childs.length < model.childs.length)
		return false;
	
	//loop on all elements of formula
	var f = 0;
	var m = 0;
	while (f < model.childs.length && m < formula.childs.length)	
	{
		//check matching of entity
		var res = this.matchEntity(model.childs[m],formula.childs[f],status);
		
		//not match so exit now
		if (res == false)
			return false;
		
		//if wildcard, check next
		if (this.isWildcardEntity(model.childs[m]) && m < model.childs.length - 1 && f < formula.childs.length - 1)
		{
			//check matching of next
			var ignored = {matched:0,capture:{}};
			var resNextWithCur  = this.matchEntity(model.childs[m],formula.childs[f+1],ignored);
			var resNextWithNext = this.matchEntity(model.childs[m+1],formula.childs[f+1],idnored);
			//inc if next can be captured
			if (resNextWithNext == true)
				m++;
		} else {
			m++;
		}
		
		//inc forula id
		f++;
	}
	
	return (f == formula.childs.length && m == model.childs.length);
}

/*******************  FUNCTION  *********************/
FormulaMatcher.prototype.matchEntityChild = function(modelList,formulaList,status)
{
	//check sizes
	if (modelList.length != formulaList.length)
		return false;

	//loop
	for (var i in modelList)
	{
		if (this.matchFormula(modelList[i],formulaList[i],status) == false)
			return false;
	}
	
	return true;
}

/*******************  FUNCTION  *********************/
FormulaMatcher.prototype.matchEntity = function(model,entity,status)
{
	//if all undefined return ok
	if (model == undefined && entity == undefined)
		return true;
	
	//check types
	assert.ok(model instanceof LatexEntity);
	assert.ok(entity instanceof LatexEntity);
	
	//check if capture
	var tags = model.tags['CMR_MATCHER_CAPTURE'];
	if (tags == undefined)
	{
		if (model.name != entity.name)
			return false;
		if (this.matchEntityChild(model.exponents,entity.exponents,status) == false)
			return false;
		if (this.matchEntityChild(model.indices,entity.indices,status) == false)
			return false;
		if (this.matchEntityChild(model.parameters,entity.parameters,status) == false)
			return false;
		if (this.matchEntity(model.groupChilds,entity.groupChilds,status) == false)
			return false;
		status.matched++;
	} else {
		return this.doCaptureEntity(model,entity,status);
	}
}

/*******************  FUNCTION  *********************/
FormulaMatcher.prototype.doCaptureEntity = function(model,entity,status)
{
	//get tags
	var tags = model.tags['CMR_MATCHER_CAPTURE'];
	assert.ok(tags != undefined);
	
	//check if accept
	if (tags.filter(entity) == false)
		return false;
	
	//extraact capture name
	//TODO implement toLatex()
	var name = model.toDebugString();
	
	//capture
	if (status.capture[name] == undefined)
		status.capture[name] = new LatexFormula();
	
	//push
	status.capture[name].addChildEntity(entity);
	
	//return ok
	return true;
}

/********************  GLOBALS  *********************/
module.exports = FormulaMatcher;
