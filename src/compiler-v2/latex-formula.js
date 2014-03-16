function LatexFormula(entity)
{
	this.content = [entity];
	this.tag = {};
}

LatexFormula.prototype.addEntity = function(entity)
{
	this.content.push(entity);
	return this;
}

LatexFormula.prototype.toString = function()
{
	var ret = '';
	this.content.forEach(function(value) {
		if (ret != '')
			ret += ' ';
		ret += value.toString();
	});
	return ret;
}

module.exports = LatexFormula;