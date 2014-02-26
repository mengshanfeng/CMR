/******************************************************************************/
//import latex parser
var parser = require('./latex-parser').parser;

/******************************************************************************/
//Describe all unit tests
var testData =  {
	"basics":[
		{ input:"a",          output:"a",              name:"Simple variable name"},
		{ input:"\\pi",       output:"\\pi",           name:"Composed variable name"}
	]
};
/*
33                            %% Integers
33.5                          %% Float
\cos                          %% Function name
\cos{i}                       %% Function with 1 simple arg
\cos{i}{j}                    %% Function with 2 simple args
\cos{i}{j}{k}                 %% Function with 3 simple args
a^b                           %% Simple power
a^{b}                         %% Simple protected power
a^{b,c}                       %% Multiple simple power
a^{b,c,d}                     %% Another multiple simple power check recurse
a@i                           %% Simple indice
a@{i}                         %% Simple protected indice
a@{i,j}                       %% Multiple simple indices
a@{i,j,k}                     %% Another multiple simple indices to check recurse
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
function runTest(test,def,i)
{
	console.log(i);
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
		list[cur.name] = function(test) {
			runTest(test,cur,i);
		};
	}
	exports[g] = list;
}
