function LatexEntity(name)
{
	//setup internal vars
	this.name = name;
	this.exponents = [];
	this.indices = [];
	this.parameters = [];
	this.groupContent = null;
	this.tags = {};
	if (name == '()')
		throw Error("todo");
}

LatexEntity.prototype.addExponent = function(expo)
{
	this.exponents.push(expo);
	return this;
}

LatexEntity.prototype.addIndice = function(indice)
{
	this.indices.push(indice);
	return this;
}

LatexEntity.prototype.addParameter = function(param)
{
	this.parameters.push(param);
	return this;
}

LatexEntity.prototype.setTag = function(tagName,tagValue)
{
	this.tags[tagName] = tagValue;
	return this;
}

function latexFormulaListToString(name,formulaList)
{
	var ret = '';
	formulaList.forEach(function(f) {
		if (ret != '')
			ret += ' ,';
		ret += f.toString();
	});
	if (formulaList.length == 0)
		return '';
	else
		return '->'+name+'( ' + ret + ' )';
}

LatexEntity.prototype.toString = function()
{
	var ret = this.name;
	ret += latexFormulaListToString('params',this.parameters);
	ret += latexFormulaListToString('exp',this.exponents);
	ret += latexFormulaListToString('ind',this.indices);
	return ret;
}

module.exports = LatexEntity;
