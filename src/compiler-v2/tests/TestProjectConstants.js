/******************************************************************************/
//import latex parser
var ProjectConstant = require('../libs/ProjectVariable.js');
var LatexEntity = require('../libs/LatexEntity.js');
var TemplateFactory = require('../libs/TemplateFactory.js');
var factory = new TemplateFactory('./templates','cmr_cpp');

/*******************  FUNCTION  *********************/
exports.test_serial_definiton = function(test)
{
	var expected = "\n//Definition of constant A\nstatic const int longNameA = 10;";
	var constant = new ProjectConstant("A","longNameA","int");
	constant.loadValuesScalar("10");
	test.equal(expected,constant.render(factory,'definition'));
	test.done();
}

/*******************  FUNCTION  *********************/
exports.test_vector_definition = function(test)
{
	var expected = "\n//Definition of constant A\nstatic const int longNameA[4] = {10,20,30,40,};";
	var constant = new ProjectConstant("A","longNameA","int");
	constant.loadValuesVector("10 & 20 & 30 & 40");
	test.equal(expected,constant.render(factory,'definition'));
	test.done();
}

/*******************  FUNCTION  *********************/
exports.test_matrix_definition = function(test)
{
	var expected = "\n//Definition of constant A\nstatic const int longNameA[4][2] = {{10,20,30,40,},{11,21,31,41,}};";
	var constant = new ProjectConstant("A","longNameA","int");
	constant.loadValuesMatrix("10 & 20 & 30 & 40 \\\\ 11 & 21 & 31 & 41");
	test.equal(expected,constant.render(factory,'definition'));
	test.done();
}

/*******************  FUNCTION  *********************/
exports.test_serial_write_access = function(test)
{
	var expected = "\nlongNameA";
	var constant = new ProjectConstant("A","longNameA","int");
	constant.loadValuesScalar("10");
	test.throws(function(){expected,constant.render(factory,'write_access',{},new LatexEntity("A"),false)});
	test.done();
}


/*******************  FUNCTION  *********************/
exports.test_serial_read_access = function(test)
{
	var expected = "\nlongNameA";
	var constant = new ProjectConstant("A","longNameA","int");
	constant.loadValuesScalar("10");
	test.equal(expected,constant.render(factory,'read_access',{},new LatexEntity("A"),false));
	test.done();
}

/*******************  FUNCTION  *********************/
exports.test_serial_vector = function(test)
{
	var expected = "\nlongNameA[i]";
	var constant = new ProjectConstant("A","longNameA","int");
	constant.loadValuesVector("10 & 20 & 30 & 40");
	test.equal(expected,constant.render(factory,'read_access',{},new LatexEntity("A_i"),false));
	test.done();
}

/*******************  FUNCTION  *********************/
exports.test_serial_matrix = function(test)
{
	var expected = "\nlongNameA[i][j]";
	var constant = new ProjectConstant("A","longNameA","int");
	constant.loadValuesMatrix("10 & 20 & 30 & 40 \\\\ 11 & 21 & 31 & 41");
	test.equal(expected,constant.render(factory,'read_access',{},new LatexEntity("A_{i,j}"),false));
	test.done();
}
