/******************************************************************************/
//import latex parser
var LatexEntity  = require('./latex-entity');
var LatexFormula = require('./latex-formula');
var parser       = require('./build/latex-parser').parser;

/******************************************************************************/
//Describe all unit tests
var testData =  {
	"basics":[
		{ input:"a",                output:"a",                                name:"Simple variable name"},
		{ input:"\\pi",             output:"\\pi",                             name:"Composed variable name"},
		{ input:"33",               output:"33",                               name:"Integers"},
		{ input:"33.5",             output:"33.5",                             name:"Floats"}
	],
	"functions":[
		{ input:"\\cos",            output:"\\cos",                            name:"Function name"},
		{ input:"\\cos{i}",         output:"\\cos->params( i )",               name:"Function with 1 simple arg"},
		{ input:"\\cos{i}{j}",      output:"\\cos->params( i , j )",           name:"Function with 2 simple arg"},
		{ input:"\\cos{i}{j}{k}",   output:"\\cos->params( i , j , k )",       name:"Function with 3 simple arg"}
	],
	"exponents": [
		{ input:"a^b",              output:"a->exp( b )",                      name:"Simple power"},
		{ input:"a^{b}",            output:"a->exp( b )",                      name:"Simple protected power"},
		{ input:"a^{b,c}",          output:"a->exp( b , c )",                  name:"Multiple simple power"},
		{ input:"a^{b,c,d}",        output:"a->exp( b , c , d )",              name:"Another multiple simple power to check resurse"}
	],
	"indices": [
		{ input:"a_i",              output:"a->ind( i )",                      name:"Simple indice"},
		{ input:"a_{i}",            output:"a->ind( i )",                      name:"Simple protected indice"},
		{ input:"a_{i,j}",          output:"a->ind( i , j )",                  name:"Multiple simple indices"},
		{ input:"a_{i,j,k}",        output:"a->ind( i , j , k )",              name:"Another multiple simple indices to check resurse"}
	],
	"groups": [
		{ input:"(a)",              output:"( a )",                            name:"Simple group"},
		{ input:"(a+b)",            output:"( a + b )",                        name:"Composed group"},
		{ input:"(a+b)*(c+d)",      output:"( a + b ) * ( c + d )",            name:"More complex composed group"},
		{ input:"(a+b)^33",         output:"( a + b )->exp( 33 )",             name:"Group end exp"},
		{ input:"(a+b)_{i,j}",      output:"( a + b )->ind( i , j )",          name:"Group and indices"},
		{ input:"a^(a+b)",          output:"a->exp( ( a + b ) )",              name:"Group as exp"},
	],
	"implicit-mult":[
		{ input:"3 a",              output:"3 [*] a",                          name:"Implicit mult"},
		{ input:"3 * a",            output:"3 * a",                            name:"Explicit mult"},
		{ input:"ab",               output:"a [*] b",                          name:"Another implicit mult without spaces"},
		{ input:"3a",               output:"3 [*] a",                          name:"Another implicit mult without spaces and nums"},
		{ input:"3(a+b)",           output:"3 [*] ( a + b )",                  name:"Another implicit mult with group"},
	],
	"mix-composed":[
		{ input:"a+b-c*d/e",        output:"a + b - c * d / e",                name:"All ops on simple vars"},
		{ input:"a^\\cos{5}",       output:"a->exp( \\cos->params( 5 ) )",     name:"Func as exp"},
		{ input:"a^\\cos{5}{6}",    output:"a->exp( \\cos->params( 5 , 6 ) )", name:"More complex func as exp"},
		{ input:"a^{b^2,c^2}",      output:"a->exp( b->exp( 2 ) , c->exp( 2 ) )",name:"exp in exp"},
		{ input:"\\cos{a+b}",       output:"\\cos->params( a + b )",           name:"Use composed as arg"},
		{ input:"\\cos{a+b}{d+e}",  output:"\\cos->params( a + b , d + e )",   name:"Use composed as multi-arg"},
		{ 
			input:"\\cos{a+b}{d+e}{h+j}",
			output:"\\cos->params( a + b , d + e , h + j )",
			name:"Use another composed as multi-arg"
		},
		{ input:"a^{a+b}",          output:"a->exp( a + b )",                   name:"Use composed as exp"},
		{ input:"a^{a+b,c+d}",      output:"a->exp( a + b , c + d )",           name:"Use composed as multi-exp"},
		{ input:"a^{a+b,c+d,e+f}",  output:"a->exp( a + b , c + d , e + f )",   name:"Use composed as multi-arg"},
		{ input:"a_{a+b}",          output:"a->ind( a + b )",                   name:"Use composed as indice"},
		{ input:"a_{a+b,c+d}",      output:"a->ind( a + b , c + d )",           name:"Use composed as multi-indices"},
		{ input:"a_{a+b,c+d,e+f}",  output:"a->ind( a + b , c + d , e + f )",   name:"Use another composed as multi-indices"},
		{ input:"a^{b}_{c}",        output:"a->exp( b )->ind( c )",             name:"Mixed exp/ind protected"},
		{ input:"a^b_c",            output:"a->exp( b )->ind( c )",             name:"Mixed exp/ind not protected"},
		{ input:"a^bc_d",           output:"a->exp( b ) [*] c->ind( d )",       name:"Mixed exp/ind sep"},
	],
	"minus": [
		{ input:"-33",              output:"[-] 33",                           name:"First minus"},
		{ input:"a*(-5)",           output:"a * ( [-] 5 )",                    name:"First minus in subgroup"},
		{ input:"-a*5",             output:"[-] a * 5",                        name:"Another check for first minus"},
		{ 
			input:"-a*\\cos{a}{b}^6",
			output:"[-] a * \\cos->params( a , b )->exp( 6 )",
			name:"Another check for first minus as first"
		},
		{ 
			input:"a*(-\\cos{a}{b}^6)",
			output:"a * ( [-] \\cos->params( a , b )->exp( 6 ) )",   
			name:"First minus in more complex"
		},
		{ 
			input:"a*(-\\cos{-a}{-b}^-6)",
			output:"a * ( [-] \\cos->params( [-] a , [-] b )->exp( [-] 6 ) )",
			name:"First minus in more complex"
		},
	],
	"whitespaces": [
		{ input:"a b",              output:"a [*] b",                          name:"Check one spaces"},
		{ input:"a  b",             output:"a [*] b",                          name:"Check multiple spaces"},
		{ input:"a		b",         output:"a [*] b",                          name:"Check tabs"},
		{ input:"a	 b",            output:"a [*] b",                          name:"Check mix tab + space"},
	]
};

/******************************************************************************/
function runTest(test,def)
{
// 	console.log(i);
	var res = JSON.stringify(parser.parse(def.input));
	test.equal(res,def.output,"Get error in parsor !");
	test.done();
}

/******************************************************************************/
for (var g in testData)
{
	var list = new Object();
	list.__counter__ = 0;
	list.tearDown = function(callback) {
		list.__counter__++;
		callback();
	}

	for (var i in testData[g])
	{
		var cur = testData[g][i];
		
		list[cur.name + " : " + cur.input] = (function(d){
			return function(test) {
				runTest(test, d);
			};
		})(cur);
	}
	exports[g] = list;
}
