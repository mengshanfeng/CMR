var assert = require('assert');

/*********************  CLASS  **********************/
function Context(parent)
{
	this.entities = [];
	this.parent = parent;
	this.keys = {};
}

/*******************  FUNCTION  *********************/
Context.prototype.setKey = function(key,value)
{
	assert.ok(key != undefined && key != "");
	this.keys[key] = value;
}

/*******************  FUNCTION  *********************/
Context.prototype.readKey = function(key)
{
	assert.ok(key != undefined && key != "");
	var cur = this;
	while (cur != undefined)
	{
		if (cur.keys[key] != undefined)
			return cur.keys[key];
		else
			cur = cur.parent;
	}
}

/*******************  FUNCTION  *********************/
Context.prototype.registerEntity = function(entity)
{
	this.entities.push(entity);
}

/********************  GLOBALS  *********************/
module.exports = Context;