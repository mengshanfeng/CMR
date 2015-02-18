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
var override = require('json-override');
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
 * Add an optional external exponent to manage exponent on all elements.
**/
FormulaMatcher.prototype.markOptionalExponent = function()
{
	if (this.model.childs.length != 1)
		throw new Error("Unexpected composed entity");
	this.model.childs[0].addExponent(new LatexFormula("\\CMRExponent"));
	this.markForCapture("\\CMRExponent","std",true);
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
	//setup defaults
	if (filter == undefined)
		filter = 'std';
	if (optional == undefined)
		optional = false;
	if (wildcard == undefined)
		wildcard = false;
	
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
		//if (model.equal(value))
		if (model.name == value.name)
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
FormulaMatcher.prototype.match = function(formula,options,iterator)
{
	//prepare capture
	var status = {
		matched:0,
		capture:{},
	};
	
	var defaultOptions = {
		rootPartial:false,
	};
	
	//override
	if (options == undefined)
		options = defaultOptions;
	else
		options = override(defaultOptions,options,true);

	//iterator
	if (iterator == undefined)
		iterator = formula.begin();
	
	//run
	var res = this.matchFormula(this.model,formula,status,options,iterator);
	
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
FormulaMatcher.prototype.matchFormula = function(model,formula,status,options,iterator)
{
	//check type
	assert.ok(formula instanceof LatexFormula);
	assert.ok(model instanceof LatexFormula);
	assert.ok(options != undefined);
	
	//trivial
	if (formula.childs.length < model.childs.length)
		return false;
	
	//setup options if not set
	var rootPartial = options.rootPartial;
	if (rootPartial)
	{
		options = clone(options);
		options.rootPartial = false;
	}
	
	//iterator
	if (iterator == undefined)
		iterator = formula.begin();
	
	//loop on all elements of formula
	var f = iterator.position;
	var m = 0;
	while (m < model.childs.length && f < formula.childs.length)	
	{
		//check matching of entity
		var res = this.matchEntity(model.childs[m],formula.childs[f],status,options);
		
		//not match so exit now
		if (res == false)
			return false;
		
		//if wildcard, check next
		if (this.isWildcardEntity(model.childs[m]) && m < model.childs.length - 1 && f < formula.childs.length - 1)
		{
			//check matching of next
			var ignored = {matched:0,capture:{}};
			var resNextWithCur  = this.matchEntity(model.childs[m],formula.childs[f+1],ignored,options);
			var resNextWithNext = this.matchEntity(model.childs[m+1],formula.childs[f+1],idnored,options);
			//inc if next can be captured
			if (resNextWithNext == true)
				m++;
		} else {
			m++;
		}
		
		//inc forula id
		f++;
	}
	
	//check final status
	ret = ((f == formula.childs.length || rootPartial == true) && m == model.childs.length)
	
	//update iterator if needed
	if (ret)
		iterator.position = f;
	
	//return
	return ret;
}

/*******************  FUNCTION  *********************/
FormulaMatcher.prototype.isOptionalEntity = function(model)
{
	assert.ok(model instanceof LatexFormula);
	if (model.childs == undefined)
		return false;
	if (model.childs.length > 1)
		return false;
	if (model.childs[0].tags == undefined)
		return false;
	var tag = model.childs[0].tags['CMR_MATCHER_CAPTURE'];
	if (tag == undefined)
		return false;
	return tag.optional;
}

/*******************  FUNCTION  *********************/
FormulaMatcher.prototype.isValidOptionalLenth = function(modelList,formuaList)
{
	//setup vars
	var min = 0;
	var max = modelList.length;
	
	//count optional and required
	for (var i in modelList)
	{
		if (this.isOptionalEntity(modelList[i]) == false)
		{
			min++;
		} else if (min > 0) {
			throw new Error("Invalid use of optional followed by non optional members !");
		}
	}
	
	return formuaList.length >= min && formuaList.length <= max;
}

/*******************  FUNCTION  *********************/
FormulaMatcher.prototype.matchEntityChild = function(modelList,formulaList,status,options)
{
	//check sizes
	//if (modelList.length != formulaList.length)
	if (this.isValidOptionalLenth(modelList,formulaList) == false)
		return false;

	//loop
	for (var i in formulaList)
	{
		if (this.matchFormula(modelList[i],formulaList[i],status,options) == false)
			return false;
	}
	
	return true;
}

/*******************  FUNCTION  *********************/
FormulaMatcher.prototype.matchEntity = function(model,entity,status,options)
{
	//if all undefined return ok
	if (model == undefined && entity == undefined)
		return true;
	
	//check types
	assert.ok(model instanceof LatexEntity);
	assert.ok(entity instanceof LatexEntity);
	
	//match childs
	if (this.matchEntityChild(model.exponents,entity.exponents,status,options) == false)
		return false;
	if (this.matchEntityChild(model.indices,entity.indices,status,options) == false)
		return false;
	if (this.matchEntityChild(model.parameters,entity.parameters,status,options) == false)
		return false;
	if (this.matchEntity(model.groupChilds,entity.groupChilds,status,options) == false)
		return false;
	
	//check if capture
	var tags = model.tags['CMR_MATCHER_CAPTURE'];
	if (tags == undefined)
	{
		if (model.name != entity.name)
			return false;
		status.matched++;
	} else {
		return this.doCaptureEntity(model,entity,status,options);
	}
}

/*******************  FUNCTION  *********************/
FormulaMatcher.prototype.doCaptureEntity = function(model,entity,status,options)
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

/*******************  FUNCTION  *********************/
FormulaMatcher.prototype.toDebugString = function()
{
	return this.model.toDebugString();
}

/********************  GLOBALS  *********************/
module.exports = FormulaMatcher;
