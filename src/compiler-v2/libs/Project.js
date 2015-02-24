/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 11/2014
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
var TemplateFactory = require('./TemplateFactory.js');
var ProjectConstant = require('./ProjectConstant.js');
var Context = require('./Context.js');

/*********************  CLASS  **********************/
function Project(name)
{
	this.name = name;
	this.context = new Context();
}

/*******************  FUNCTION  *********************/
Project.prototype.addEntity = function(entity)
{
	this.context.registerEntity(entity);
}

/*******************  FUNCTION  *********************/
Project.prototype.addConstant = function(latexName,longName,type)
{
	var cst = new ProjectConstant(latexName,longName,type);
	this.addEntity(cst);
	return cst;
}

/*******************  FUNCTION  *********************/
Project.prototype.render = function(lang)
{
	if (lang == undefined)
		lang = "cmr_cpp";
	var factory = new TemplateFactory("./templates/",lang);
	this.templateFactory = factory;
	var data = {
		project:this,
	};
	return factory.render('project','source',data);
}

/*******************  FUNCTION  *********************/
Project.prototype.renderGroup = function(group,type)
{
	var res = "";
	var self = this;
	this.context.entities.forEach(function(value){
		if (value.group == group)
			res += value.render(self.templateFactory,type,this.entities,null);
	});
	return res;
}

/********************  GLOBALS  *********************/
module.exports = Project;