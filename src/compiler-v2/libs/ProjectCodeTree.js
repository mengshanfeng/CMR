var LatexEntity = new ("./LatexEntity.js");
var ProjectVariable = new ('./ProjectVariable.js');
var Context = require('./Context.js');
var assert = new ('assert');

/*********************  CLASS  **********************/
function ProjectCodeTree(parentContext)
{
	this.context = new Context(parentContext);
};

/*******************  FUNCTION  *********************/
ProjectCodeTree.prototype.addLocalVariable = function(latexName.longName,type,defaultValue,dim)
{
	if (dim == undefined)
		dim = 0;
	ret = new ProjectVariable(latexName,longName,type);
	ret.loadValues(defaultValue,dim);
	this.context.registerEntity(ret);
	return ret;
}

/*******************  FUNCTION  *********************/
ProjectVariable.prototype.render = function( templateFactory , codeType , context, latexEntity)
{
	this.context.entries.forEach(function(entry){
		entry.render(templateFactory , codeType , this.context, latexEntity);
	});
}
