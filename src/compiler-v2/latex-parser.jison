/* Parsor for latex math equations */

/* Lexical grammar */
%lex

%%
\s+                        /* white spaces ignored */
\%\%.*$                    /* ignore comments at end of line*/
[0-9]+("."[0-9]+)?         return 'NUMBER'                     /* Numbers (integer of float) */
[*/+]                      return 'OPERATOR'                   /* A math operator to separate words */
"*"                        return '*';                         /* Mult operator              */
"/"                        return '/';                         /* Div operator               */
"-"                        return '-';                         /* Minus operator             */
"+"                        return '+';                         /* + operator                 */
"("                        return '(';                         /* Start group                */
")"                        return ')';                         /* End group                  */
\\[A-Za-z][A-Za-z0-9]*     return 'FUNC_NAME';                 /* Function name (protected by \ and multi-char) */
[A-Za-z]                   return 'SHORT_NAME';                /* Short variable name (1 letter) */
"{"                        return '{';                         /* Start protected parameter  */
"}"                        return '}';                         /* End protected parameter    */
"^"                        return '^';                         /* Start exponent decoration  */
[_]                        return '_';                         /* Start indice decoration    */
","                        return ',';                         /* Parameter separator        */
<<EOF>>                    return 'EOF';

/lex

/* operator associations and precedence */
%left '+' '-'
%left '*' '/'
%left '^' '_'
%left UMINUS


%start expression

%% /* language grammar */

/* Global expression */
expression
	: eq EOF
		{return $1;}
	;

/* equation is composed of multiple decoWord with optional operators to separate them, if not => implici mult */
eq
	: decoWord                               /* Simple uniq decoWord */
		{$$ = $1}
	| '-' decoWord %prec UMINUS              /* Invert sign */
		{$$ = "[-] " + $2}
	| eq decoWord                           /* Composed with implicit mult */
		{$$ = $1 + " [*] " + $2}
	| eq 'OPERATOR' decoWord               /* Composed by math operators */
		{$$ = $1 + " " + $2 + " " + $3}
	| eq '-' decoWord                       /* Composed by - operator, not in operator otherwise invert sign don't work */
		{$$ = $1 + " - " + $3}
	;

/* Old def previously used instead of 'OPERATOR', need to remove this */
NOTUSED
	: eq '+' decoWord                       /* Composed by + operator */
		{$$ = $1 + " + " + $3}
	| eq '-' decoWord                       /* Composed by - operator */
		{$$ = $1 + " - " + $3}
	| eq '*' decoWord                       /* Composed by * operator */
		{$$ = $1 + " * " + $3}
	| eq '/' decoWord                       /* Composed by / operator */
		{$$ = $1 + " / " + $3}
	;

/* group permit to control priority of composed decoWords and protected by () */
group
	: '(' eq ')'
		{$$ = "( " + $2 + " )"}
	;

/* Define a word which if a simple name, a group of a unction. An entity will get some decoration in 'decoWord' definition. **/
word
	: name
		{$$ = $1}
	| group
		{$$ = $1}
	| function
		{$$ = $1}
	;

/* Permit to add decoration to the word (exp or indices) */
decoWord
	: word
		{$$ = $1}
	| decoWord '^' decoParameter
		{$$ = $1 + '->exp( ' + $3 +' )'}
	| decoWord '_' decoParameter
		{$$ = $1 + '->ind( ' + $3 +' )'}
	;

/* Add decoration on words (exp, indices) */
decoParameter
	: word
		{$$ = $1}
	| '-' word %prec UMINUS
		{$$ = "[-] " + $2}
	| "{" decoParameterValue "}"
		{$$ = $2}
	;

/* Decoration values */
decoParameterValue
	: eq
		{$$ = $1}
	| eq "," decoParameterValue
		{$$ = $1 + " , " + $3}
	;

/* Function parameters */
funcParameters 
	: funcParameterValue
		{$$ = $1}
	| funcParameters funcParameterValue
		{$$ = $1 + " , " + $2}
	;

/* Function parameter protected by {} */
funcParameterValue
	: "{" eq "}"
		{$$ = $2}
	;

/* Define a basic name which is a short one letter variable name or a number. */
name
	: SHORT_NAME
		{$$ = $1}
	| NUMBER
		{$$ = $1}
	;

/* Define a function which can be a simple function name with or without parameters */
function
	: FUNC_NAME
		{$$ = $1}
	| FUNC_NAME funcParameters
		{$$ = $1 + '->params( ' + $2 + ' )'}
	;
