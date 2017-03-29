/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 11/2014
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
var LatexEntity = require ("./LatexEntity.js");
var ProjectVariable = require ('./ProjectVariable.js');
var CodeTreeMathStep = require('./CodeTreeMathStep.js');
var Context = require('./Context.js');
var assert = require ('assert');

/*********************  CLASS  **********************/
function ProjectCodeTree(parentContext,indent)
{
	this.context = new Context(parentContext);
	if (indent == undefined)
		this.indent = 0;
	else
		this.indent = indent;
	this.entries = [];
};

/*******************  FUNCTION  *********************/
ProjectCodeTree.prototype.addLocalVariable = function(latexName,longName,type,defaultValue,dim)
{
	if (dim == undefined)
		dim = 0;
	ret = new ProjectVariable(latexName,longName,type,false);
	if (defaultValue != undefined)
		ret.loadValues(defaultValue,dim);
	this.context.registerEntity(ret);
	return ret;
}

/*******************  FUNCTION  *********************/
ProjectCodeTree.prototype.render = function( templateFactory , codeType , context, latexEntity)
{
	var data = {
		code: this,
		templateFactory: templateFactory
	};
	var out = templateFactory.render("code",codeType,data);
	
	//indent
	var lines = out.split("\n");
	var indent = "\t".repeat(this.indent);
	
	//return join
	return indent+lines.join("\n"+indent);
}

/*******************  FUNCTION  *********************/
ProjectCodeTree.prototype.renderGroup = function(templateFactory,group,type,sep)
{
	var res = "";
	var first = true;
	if (sep == undefined)
		sep = "";
	this.context.entities.forEach(function(value){
		if (value.group == group)
		{
			if (first == false)
				res+=sep;
			res += value.render(templateFactory,type,this.entities,null);
			first = false;
		}
	});
	return res;
}

/*******************  FUNCTION  *********************/
ProjectCodeTree.prototype.renderCode = function(templateFactory,type,sep)
{
	var res = "";
	var first = true;
	if (sep == undefined)
		sep = "";
	this.entries.forEach(function(value){
		if (first == false)
			res+=sep;
		res += value.render(templateFactory,type);
		first = false;
	});
	return res;
}

/*******************  FUNCTION  *********************/
ProjectCodeTree.prototype.addMathStep = function(latex)
{
	var step = new CodeTreeMathStep(this.context,latex);
	this.entries.push(step);
	return step;
}

/********************  GLOBALS  *********************/
module.exports = ProjectCodeTree;
