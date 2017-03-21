var LatexEntity = require ("./LatexEntity.js");
var ProjectVariable = require ('./ProjectVariable.js');
var Context = require('./Context.js');
var assert = require ('assert');

/*********************  CLASS  **********************/
function ProjectCodeTree(parentContext)
{
	this.context = new Context(parentContext);
	this.indend = 0;
	this.entries = [];
};

/*******************  FUNCTION  *********************/
ProjectCodeTree.prototype.addLocalVariable = function(latexName,longName,type,defaultValue,dim)
{
	if (dim == undefined)
		dim = 0;
	ret = new ProjectVariable(latexName,longName,type);
	ret.loadValues(defaultValue,dim);
	this.context.registerEntity(ret);
	return ret;
}

/*******************  FUNCTION  *********************/
ProjectCodeTree.prototype.render = function( templateFactory , codeType , context, latexEntity)
{
	this.context.entries.forEach(function(entry){
		entry.render(templateFactory , codeType , this.context, latexEntity);
	});
}

/********************  GLOBALS  *********************/
module.exports = ProjectCodeTree;
