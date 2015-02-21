/******************************************************************************/
//import latex parser
var ProjectConstant = require('../libs/ProjectConstant.js');
var LatexEntity = require('../libs/LatexEntity.js');

/*******************  FUNCTION  *********************/
exports.test_serial_definiton = function(test)
{
	var expected = "//Definition of constant A\nstatic const int longNameA = 10;";
	var constant = new ProjectConstant("A","longNameA","int");
	constant.loadValuesScalar("10");
	test.equal(expected,constant.genCode('definition'));
	test.done();
}

/*******************  FUNCTION  *********************/
exports.test_vector_definition = function(test)
{
	var expected = "//Definition of constant A\nstatic const int longNameA[4] = {10,20,30,40,};";
	var constant = new ProjectConstant("A","longNameA","int");
	constant.loadValuesVector("10 & 20 & 30 & 40");
	test.equal(expected,constant.genCode('definition'));
	test.done();
}

/*******************  FUNCTION  *********************/
exports.test_matrix_definition = function(test)
{
	var expected = "//Definition of constant A\nstatic const int longNameA[4][2] = {{10,20,30,40,},{11,21,31,41,}};";
	var constant = new ProjectConstant("A","longNameA","int");
	constant.loadValuesMatrix("10 & 20 & 30 & 40 \\\\ 11 & 21 & 31 & 41");
	test.equal(expected,constant.genCode('definition'));
	test.done();
}

/*******************  FUNCTION  *********************/
exports.test_serial_usage = function(test)
{
	var expected = "longNameA";
	var constant = new ProjectConstant("A","longNameA","int");
	constant.loadValuesScalar("10");
	test.equal(expected,constant.genCode('usage',{},new LatexEntity("A"),false));
	test.done();
}

/*******************  FUNCTION  *********************/
exports.test_serial_vector = function(test)
{
	var expected = "longNameA[i]";
	var constant = new ProjectConstant("A","longNameA","int");
	constant.loadValuesVector("10 & 20 & 30 & 40");
	test.equal(expected,constant.genCode('usage',{},new LatexEntity("A_i"),false));
	test.done();
}

/*******************  FUNCTION  *********************/
exports.test_serial_matrix = function(test)
{
	var expected = "longNameA[i][j]";
	var constant = new ProjectConstant("A","longNameA","int");
	constant.loadValuesMatrix("10 & 20 & 30 & 40 \\\\ 11 & 21 & 31 & 41");
	test.equal(expected,constant.genCode('usage',{},new LatexEntity("A_{i,j}"),false));
	test.done();
}
