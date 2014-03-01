/******************************************************************************/
//import latex parser
var parser = require('./build/latex-parser').parser;

/******************************************************************************/
//Describe all unit tests
var testData =  {
	"basics":[
		{ input:"a",                output:"a",                            name:"Simple variable name"},
		{ input:"\\pi",             output:"\\pi",                         name:"Composed variable name"},
		{ input:"33",               output:"33",                           name:"Integers"},
		{ input:"33.5",             output:"33.5",                         name:"Floats"}
	],
	"functions":[
		{ input:"\\cos",            output:"\\cos",                        name:"Function name"},
		{ input:"\\cos{i}",         output:"\\cos->params(i)",             name:"Function with 1 simple arg"},
		{ input:"\\cos{i}{j}",      output:"\\cos->params(i,j)",           name:"Function with 1 simple arg"},
		{ input:"\\cos{i}{j}{k}",   output:"\\cos->params(i,j,k)",         name:"Function with 1 simple arg"}
	],
	"exponents": [
		{ input:"a^b",              output:"a->exp(b)",                    name:"Simple power"},
		{ input:"a^{b}",            output:"a->exp(b)",                    name:"Simple protected power"},
		{ input:"a^{b,c}",          output:"a->exp(b,c)",                  name:"Multiple simple power"},
		{ input:"a^{b,c,d}",        output:"a->exp(b,c,d)",                name:"Another multiple simple power to check resurse"}
	],
	"indices": [
		{ input:"a_i",              output:"a->ind(i)",                    name:"Simple indice"},
		{ input:"a_{i}",            output:"a->ind(i)",                    name:"Simple protected indice"},
		{ input:"a_{i,j}",          output:"a->ind(i,j)",                  name:"Multiple simple indices"},
		{ input:"a_{i,j,k}",        output:"a->ind(i,j,k)",                name:"Another multiple simple indices to check resurse"}
	]
};
/*
(a)                           %% Simple group
(a+b)                         %% Composed group
(a+b)*(c+d)                   %% More complex composed group
(a+b)^33                      %% Group end exp
(a+b)@{i,j}                   %% group and indices
a^(a+b)                       %% Group as exp
3 a                           %% Implicit mult
3 * a                         %% explicit mult
ab                            %% Another implicit mult without spaces
3a                            %% Another implicit mult without spaces
a+b-c*d/e                     %% All ops on simple vars
a^\cos{5}                     %% Func as exp
a^\cos{5}{6}                  %% More complex func as exp
a^{b^2,c^2}                   %% exp in exp
-33                           %% First minus
a*(-5)                        %% First minus in subgroup
a*(-\cos{a}{b}^6)             %% Furst minus in more complex
a*(-\cos{-a}{-b}^-6)          %% First minus in more complex
-a*5                          %% Another check for first minus
-a*\cos{a}{b}^6               %% Another check for first minus
\cos{a+b}                     %% Use composed as arg
\cos{a+b}{d+e}                %% Use composed as multi-arg
\cos{a+b}{d+e}{h+j}           %% Use composed as multi-arg
a^{a+b}                       %% Use composed as exp
a^{a+b,c+d}                   %% Use composed as multi-exp
a^{a+b,c+d,e+f}               %% Use composed as multi-arg
a@{a+b}                       %% Use composed as indice
a@{a+b,c+d}                   %% Use composed as multi-indices
a@{a+b,c+d,e+f}               %% Use composed as multi-indices
a  b                          %% Check spaces
a		b                     %% Check tabs
a	  	b                     %% Check mix space + tabs
*/

/******************************************************************************/
function runTest(test,def)
{
// 	console.log(i);
	var res = parser.parse(def.input);
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
		
		
// 		list[cur.name] = function(test) {
// 			runTest(test,cur,i);
// 		};
	}
	exports[g] = list;
}
