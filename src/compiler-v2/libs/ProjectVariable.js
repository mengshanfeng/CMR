/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 11/2014
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
var FormulaMatcher = require('./FormulaMatcher.js');
var LatexFormula = require('./LatexFormula.js');
var TemplateFactory = require('./TemplateFactory.js');
var assert = require('assert');

/*********************  CLASS  **********************/
function ProjectVariable(latexName, longName, type, ghostCnt, id )
{
	if (id == undefined)
		id = 0;
	this.id = id;
	this.longName = longName;
	this.latexName = latexName;
	this.type = type;
	this.ghostCnt = ghostCnt;
	this.matcher = new FormulaMatcher(latexName);
	this.group = 'variable';
	this.memoryModel = "CMRMemoryModelColMajor";
	this.defs = [];
	
	this.matcher.markForCapture("i","wildcard",false,true);
	this.matcher.markForCapture("j","wildcard",false,true);
};

/*******************  FUNCTION  *********************/
ProjectVariable.prototype.addDimension = function( name,size,start )
{
	assert.ok(size > 0);
	assert.ok(name != "");
	if (start == undefined)
		start = 0;
	this.defs.push({
		name:name,
		size:size,
		start:start
	});
	this.matcher.markForCapture(name,"wildcard",false,true);
}

/*******************  FUNCTION  *********************/
ProjectVariable.prototype.toDebugString = function( data )
{
	//default
	return this.matcher.toDebugString();
}

/*******************  FUNCTION  *********************/
ProjectVariable.prototype.render = function( templateFactory , codeType , context, latexEntity)
{
	var data = {
		object: this,
		context:context,
		latexEntity: latexEntity
	};
	return templateFactory.render("variable",codeType,data);
}

/*******************  FUNCTION  *********************/
ProjectVariable.prototype.getUsageOps = function(context,latexEntity,write)
{
	//error
	if (this.write)
		throw new Error("Constant cannot be used to write in !");
	
	//extract matching
	var f = new LatexFormula(latexEntity);
	var status = this.matcher.capture(f);
	var ret = [
		status.capture["i"],
		status.capture["j"]
	];
	
	return ret;
}

/*******************  FUNCTION  *********************/
ProjectVariable.prototype.genAccessCCode = function(context,entity,write)
{
	var ret = {
		longName:this.longName
	};
	var loopType = context.readKey("CMRActionLoopType");
	
	//select out mode
	if (loopType == "CMRMeshOperationSimpleLoop" || loopType == "CMRMeshOperationSimpleLoopWithPos" || loopType == "")
	{
		if (write)
			ret.varName = "out";
		else
			ret.varName = "in";
	} else if (loopType == "CMRMeshOperationSimpleLoopInPlace" || loopType == "CMRMeshOperationSimpleLoopInPlaceWithPos") {
		ret.varName = "cell";
	} else {
		throw new Error("Invalid action loop type : "+loopType);
	}

	//tmp.name = shortName;
	var status = this.matcher.capture(entity);
	assert.ok(status != false);
	
	ret.array = [
		//TODO to CCode
		status.capture["i"].toLatexString(),
		status.capture["j"].toLatexString()
	];
	
	this.defs.forEach(function(value) {
		ret.array.push(status.capture[value].toLatexString());
	});
	return ret;
}

// 
// /*******************  FUNCTION  *********************/
// void ProjectVariable::printValues ( ostream& out ) const
// {
// 	switch(dims.size())
// 	{
// 		case 0:
// 			out << formulas[0];
// 			break;
// 		case 1:
// 			for (int i = 0 ; i < dims[0] ; i++)
// 				out << formulas[i] << " ";
// 			break;
// 		case 2:
// 			for (int j = 0 ; j < dims[1] ; j++)
// 			{
// 				for (int i = 0 ; i < dims[0] ; i++)
// 					out << formulas[dims[0]*j+i] << " ";
// 				out << endl;
// 			}
// 			break;
// 		default:
// 			throw LatexException("Dims > 2 not suppported !");
// 	}
// }
// 
// /*******************  FUNCTION  *********************/
// void ProjectVariable::transform ( void )
// {
// 	CMRModelBasedReplacement frac("\\frac{a}{b}","a/b");
// 	for (ConstantFormulaVector::iterator it = formulas.begin() ; it != formulas.end() ; ++it)
// 		transform(*it,frac);
// }
// 
// /*******************  FUNCTION  *********************/
// void ProjectVariable::transform ( LatexFormulas& formula, CMRModelBasedReplacement& action )
// {
// 	for (LatexEntityVector::iterator it = formula.begin() ; it != formula.end() ; ++it)
// 		if (formula.hasInfo("cmrNoTranform") == false)
// 			this->transform(**it,action);
// }
// 
// /*******************  FUNCTION  *********************/
// void ProjectVariable::transform ( LatexEntity& entity, CMRModelBasedReplacement& action )
// {
// 	action.apply(entity);
// 	
// 	LatexFormulasVector & indices = entity.getIndices();
// 	for (LatexFormulasVector::iterator it = indices.begin() ; it != indices.end() ; ++it)
// 		if ((*it)->hasInfo("cmrNoTranform") == false)
// 			this->transform(**it,action);
// 	
// 	LatexFormulasVector & exponents = entity.getExponents();
// 	for (LatexFormulasVector::iterator it = exponents.begin() ; it != exponents.end() ; ++it)
// 		if ((*it)->hasInfo("cmrNoTranform") == false)
// 			this->transform(**it,action);
// 	
// 	LatexFormulasVector & params = entity.getParameters();
// 	for (LatexFormulasVector::iterator it = params.begin() ; it != params.end() ; ++it)
// 		if ((*it)->hasInfo("cmrNoTranform") == false)
// 			this->transform(**it,action);
// }
// 
// }


/********************  GLOBALS  *********************/
module.exports = ProjectVariable;
