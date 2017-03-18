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
function ProjectVariable(latexName,longName,type,constant)
{
	this.longName = longName;
	this.latexName = latexName;
	this.type = type;
	this.matcher = new FormulaMatcher(latexName);
	this.formulas = [];
	this.dims = [];
	this.setConst(constant);
	this.doc = "";
};

/******************* FUNCTION *********************/
ProjectVariable.prototype.setDoc = function(doc)
{
	if (doc != undefined)
		this.doc = doc;
}

/******************* FUNCTION *********************/
ProjectVariable.prototype.setConst = function(status)
{
	if (status == undefined || status == true)
	{
		this.group = 'constant';
		this.constant = true;
	} else {
		this.group = 'variable';
		this.constant = false;
	}
}

/*******************  FUNCTION  *********************/
ProjectVariable.prototype.loadValues = function(data,dimensions)
{
	assert.ok(this.formulas.length == 0);
	switch(dimensions)
	{
		case 0:
			this.loadValuesScalar(data);
			break;
		case 1:
			this.loadValuesVector(data);
			break;
		case 2:
			this.loadValuesMatrix(data);
			break;
		default:
			throw new Error("Unsupported variable dimension : "+ dimensions);
			break;
	}
	//transform();
	
	return this;
}

/*******************  FUNCTION  *********************/
ProjectVariable.prototype.loadValuesScalar = function(data)
{
	//errors
		//errors
	if(data == undefined || data == "")
		throw new Error("Invalid empty data to be loaded as scalar !");
	
	//error
	vs1 = data.split(';');
	vs2 = data.split('\\\\');
	
	if (vs1.length > 1 || vs2.length > 1) 
		throw new Error("Caution, you say scalar but provide vector or matrix as data !");
	
	this.formulas.push(new LatexFormula(data));
	
	return this;
}

/*******************  FUNCTION  *********************/
ProjectVariable.prototype.loadValuesVector = function( data )
{
	//errors
	if(data == "")
		throw new Error("Invalid empty data to be loaded as scalar !");
	
	//error
	vs2 = data.split("\\\\");
	if (vs2.length > 1)
		throw new Error("Caution, you say vector but provide matrix as data !");
	
	//split
	vs = data.split("&");
	this.addDimension(vs.length);
	
	//push all
	var self = this;
	vs.forEach(function(value) {
		self.formulas.push(new LatexFormula(value));
	});

 	if (self.formulas == 1)
		console.log("Warning, you get a unique 0.0 value for a vector, maybe this is a mistake !\n");
	
	return this;
}

/*******************  FUNCTION  *********************/
ProjectVariable.prototype.loadValuesMatrix = function( data )
{
	//vars
	var dim1 = -1;
	var dim2 = -1;
	
	//errors
	if(data == "")
		throw new Error("Invalid empty data to be loaded as scalar !");
	
	var ms = data.split("\\\\");
	dim1 = ms.length;
	
	self = this;
	ms.forEach(function(value) {
		var vs = value.split("&");
		if (dim2 == -1)
		{
			dim2 = vs.length;
		} else if(dim2 != vs.length) {
			throw new Error("Caution you prides lines which do not have the same size !");
		}
		vs.forEach(function(serValue) {
			self.formulas.push(new LatexFormula(serValue));
		});
	});
	
	this.addDimension(dim2);
	this.addDimension(dim1);
	
	if (this.formulas.length == 1 && this.formulas[0] == 0.0)
		console.log("Warning, you get a unique 0.0 value for a matrix, maybe this is a mistake !\n");
}

/*******************  FUNCTION  *********************/
ProjectVariable.prototype.addDimension = function( size )
{
	this.dims.push(size);
	switch(this.dims.length)
	{
		case 1:
			this.matcher.model.childs[0].addIndice(new LatexFormula("\\cmr{\\id}{i}"));
			this.matcher.markForCapture("\\cmr{\\id}{i}","wildcard",false,true);
			break;
		case 2:
			this.matcher.model.childs[0].addIndice(new LatexFormula("\\cmr{\\id}{j}"));
			this.matcher.markForCapture("\\cmr{\\id}{j}","wildcard",false,true);
			break;
		default:
			throw new Error("Unsupported constent dimension : " + size);
	}
}

/*******************  FUNCTION  *********************/
ProjectVariable.prototype.toDebugString = function( data )
{
	//default
	var ret = this.matcher.toDebugString()+"\n";
		
	//dims
	ret += "    - dims       : " + dims.size() + " : [ ";
	for (var i in dims)
	{
		if ( i > 0 )
			ret += ", ";
		ret += dims[i];
	};
	ret += " ]\n";
	
	//values
	ret += "    - values     :";
	for (var i in dims)
	{
		if ( i > 0 )
			ret += ", ";
		ret += formulas[i];
	}
	ret += "\n";
}

/*******************  FUNCTION  *********************/
ProjectVariable.prototype.render = function( templateFactory , codeType , context, latexEntity)
{
	var data = {
		object: this,
		context:context,
		latexEntity: latexEntity
	};
	if (codeType == "write_access" && this.constant)
		throw new Error("Unsupported write access on constant !");
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
	var ret = [];

	//not managed
	assert.ok(this.dims.length <= 2);
	
	//vector
	if (this.dims.length >= 1)
	{
		assert.ok(status.capture["\\cmr{\\id}{i}"] != undefined);
		ret.push(status.capture["\\cmr{\\id}{i}"]);
	}
	
	//matrix
	if (this.dims.length >= 2)
	{
		assert.ok(status.capture["\\cmr{\\id}{j}"] != undefined);
		ret.push(status.capture["\\cmr{\\id}{j}"]);
	}
	
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
