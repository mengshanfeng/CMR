/*****************************************************
             PROJECT  : CartesinMeshRuntime
             VERSION  : 0.0.0
             DATE     : 11/2014
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  GLOBALS  *********************/
var fs = require('fs');
var assert = require('assert');
var ejs = require('ejs');

/*********************  CLASS  **********************/
function TemplateFactory(rootDir, lang)
{
	this.rootDir = rootDir;
	this.lang = lang;
	this.cache = {};
}

/*******************  FUNCTION  *********************/
TemplateFactory.prototype.get = function(element,action)
{
	var filename = this.rootDir+"/"+this.lang+"/"+element+"_"+action+".ejs";
	if (this.cache[filename] == undefined)
	{
		console.log(filename);
		var template = fs.readFileSync(filename,{encoding:'UTF-8'});
		if (template == false)
			throw new Error("Invalid action "+action+" on element " + element);
		this.cache[filename] = template;
	}

	return this.cache[filename];
}

/*******************  FUNCTION  *********************/
TemplateFactory.prototype.render = function(element , action , data)
{
	var cache = this.cache[element+"_"+action];
	var code = "";
	code = ejs.render(this.get(element,action),data);
	return code.replace(/\n$/,"");
}

/********************  GLOBALS  *********************/
module.exports = TemplateFactory;
