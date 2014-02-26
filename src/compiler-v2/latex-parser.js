/* parser generated by jison 0.4.13 */
/*
  Returns a Parser object of the following structure:

  Parser: {
    yy: {}
  }

  Parser.prototype: {
    yy: {},
    trace: function(),
    symbols_: {associative list: name ==> number},
    terminals_: {associative list: number ==> name},
    productions_: [...],
    performAction: function anonymous(yytext, yyleng, yylineno, yy, yystate, $$, _$),
    table: [...],
    defaultActions: {...},
    parseError: function(str, hash),
    parse: function(input),

    lexer: {
        EOF: 1,
        parseError: function(str, hash),
        setInput: function(input),
        input: function(),
        unput: function(str),
        more: function(),
        less: function(n),
        pastInput: function(),
        upcomingInput: function(),
        showPosition: function(),
        test_match: function(regex_match_array, rule_index),
        next: function(),
        lex: function(),
        begin: function(condition),
        popState: function(),
        _currentRules: function(),
        topState: function(),
        pushState: function(condition),

        options: {
            ranges: boolean           (optional: true ==> token location info will include a .range[] member)
            flex: boolean             (optional: true ==> flex-like lexing behaviour where the rules are tested exhaustively to find the longest match)
            backtrack_lexer: boolean  (optional: true ==> lexer regexes are tested in order and for each matching regex the action code is invoked; the lexer terminates the scan when a token is returned by the action code)
        },

        performAction: function(yy, yy_, $avoiding_name_collisions, YY_START),
        rules: [...],
        conditions: {associative list: name ==> set},
    }
  }


  token location info (@$, _$, etc.): {
    first_line: n,
    last_line: n,
    first_column: n,
    last_column: n,
    range: [start_number, end_number]       (where the numbers are indexes into the input string, regular zero-based)
  }


  the parseError function receives a 'hash' object with these members for lexer and parser errors: {
    text:        (matched text)
    token:       (the produced terminal token, if any)
    line:        (yylineno)
  }
  while parser (grammar) errors will also provide these members, i.e. parser errors deliver a superset of attributes: {
    loc:         (yylloc)
    expected:    (string describing the set of expected tokens)
    recoverable: (boolean: TRUE when the parser has a error recovery rule available for this particular error)
  }
*/
var latexParser = (function(){
var parser = {trace: function trace() { },
yy: {},
symbols_: {"error":2,"expression":3,"eq":4,"EOF":5,"decoWord":6,"-":7,"OPERATOR":8,"NOTUSED":9,"+":10,"*":11,"/":12,"group":13,"(":14,")":15,"word":16,"name":17,"function":18,"^":19,"decoParameter":20,"_":21,"{":22,"decoParameterValue":23,"}":24,",":25,"funcParameters":26,"funcParameterValue":27,"SHORT_NAME":28,"NUMBER":29,"FUNC_NAME":30,"$accept":0,"$end":1},
terminals_: {2:"error",5:"EOF",7:"-",8:"OPERATOR",10:"+",11:"*",12:"/",14:"(",15:")",19:"^",21:"_",22:"{",24:"}",25:",",28:"SHORT_NAME",29:"NUMBER",30:"FUNC_NAME"},
productions_: [0,[3,2],[4,1],[4,2],[4,2],[4,3],[4,3],[9,3],[9,3],[9,3],[9,3],[13,3],[16,1],[16,1],[16,1],[6,1],[6,3],[6,3],[20,1],[20,2],[20,3],[23,1],[23,3],[26,1],[26,2],[27,3],[17,1],[17,1],[18,1],[18,2]],
performAction: function anonymous(yytext, yyleng, yylineno, yy, yystate /* action[1] */, $$ /* vstack */, _$ /* lstack */) {
/* this == yyval */

var $0 = $$.length - 1;
switch (yystate) {
case 1:return $$[$0-1];
break;
case 2:this.$ = $$[$0]
break;
case 3:this.$ = "[-]" + $$[$0]
break;
case 4:this.$ = $$[$0-1] + " [*] " + $$[$0]
break;
case 5:this.$ = $$[$0-2] + " " + $$[$0-1] + " " + $$[$0]
break;
case 6:this.$ = $$[$0-2] + " - " + $$[$0]
break;
case 7:this.$ = $$[$0-2] + " + " + $$[$0]
break;
case 8:this.$ = $$[$0-2] + " - " + $$[$0]
break;
case 9:this.$ = $$[$0-2] + " * " + $$[$0]
break;
case 10:this.$ = $$[$0-2] + " / " + $$[$0]
break;
case 11:this.$ = "( " + $$[$0-1] + " )"
break;
case 12:this.$ = $$[$0]
break;
case 13:this.$ = $$[$0]
break;
case 14:this.$ = $$[$0]
break;
case 15:this.$ = $$[$0]
break;
case 16:this.$ = $$[$0-2] + '->exp(' + $$[$0] +')'
break;
case 17:this.$ = $$[$0-2] + '->ind(' + $$[$0] +')'
break;
case 18:this.$ = $$[$0]
break;
case 19:this.$ = "[-]" + $$[$0]
break;
case 20:this.$ = $$[$0-1]
break;
case 21:this.$ = $$[$0]
break;
case 22:this.$ = $$[$0-2] + " , " + $$[$0]
break;
case 23:this.$ = $$[$0]
break;
case 24:this.$ = $$[$0-1] + " , " + $$[$0]
break;
case 25:this.$ = $$[$0-1]
break;
case 26:this.$ = $$[$0]
break;
case 27:this.$ = $$[$0]
break;
case 28:this.$ = $$[$0]
break;
case 29:this.$ = $$[$0-1] + '->params(' + $$[$0] + ')'
break;
}
},
table: [{3:1,4:2,6:3,7:[1,4],13:7,14:[1,11],16:5,17:6,18:8,28:[1,9],29:[1,10],30:[1,12]},{1:[3]},{5:[1,13],6:14,7:[1,16],8:[1,15],13:7,14:[1,11],16:5,17:6,18:8,28:[1,9],29:[1,10],30:[1,12]},{5:[2,2],7:[2,2],8:[2,2],14:[2,2],15:[2,2],24:[2,2],25:[2,2],28:[2,2],29:[2,2],30:[2,2]},{6:17,13:7,14:[1,11],16:5,17:6,18:8,28:[1,9],29:[1,10],30:[1,12]},{5:[2,15],7:[2,15],8:[2,15],14:[2,15],15:[2,15],19:[1,18],21:[1,19],24:[2,15],25:[2,15],28:[2,15],29:[2,15],30:[2,15]},{5:[2,12],7:[2,12],8:[2,12],14:[2,12],15:[2,12],19:[2,12],21:[2,12],24:[2,12],25:[2,12],28:[2,12],29:[2,12],30:[2,12]},{5:[2,13],7:[2,13],8:[2,13],14:[2,13],15:[2,13],19:[2,13],21:[2,13],24:[2,13],25:[2,13],28:[2,13],29:[2,13],30:[2,13]},{5:[2,14],7:[2,14],8:[2,14],14:[2,14],15:[2,14],19:[2,14],21:[2,14],24:[2,14],25:[2,14],28:[2,14],29:[2,14],30:[2,14]},{5:[2,26],7:[2,26],8:[2,26],14:[2,26],15:[2,26],19:[2,26],21:[2,26],24:[2,26],25:[2,26],28:[2,26],29:[2,26],30:[2,26]},{5:[2,27],7:[2,27],8:[2,27],14:[2,27],15:[2,27],19:[2,27],21:[2,27],24:[2,27],25:[2,27],28:[2,27],29:[2,27],30:[2,27]},{4:20,6:3,7:[1,4],13:7,14:[1,11],16:5,17:6,18:8,28:[1,9],29:[1,10],30:[1,12]},{5:[2,28],7:[2,28],8:[2,28],14:[2,28],15:[2,28],19:[2,28],21:[2,28],22:[1,23],24:[2,28],25:[2,28],26:21,27:22,28:[2,28],29:[2,28],30:[2,28]},{1:[2,1]},{5:[2,4],7:[2,4],8:[2,4],14:[2,4],15:[2,4],24:[2,4],25:[2,4],28:[2,4],29:[2,4],30:[2,4]},{6:24,13:7,14:[1,11],16:5,17:6,18:8,28:[1,9],29:[1,10],30:[1,12]},{6:25,13:7,14:[1,11],16:5,17:6,18:8,28:[1,9],29:[1,10],30:[1,12]},{5:[2,3],7:[2,3],8:[2,3],14:[2,3],15:[2,3],24:[2,3],25:[2,3],28:[2,3],29:[2,3],30:[2,3]},{6:27,7:[1,28],13:7,14:[1,11],16:5,17:6,18:8,20:26,22:[1,29],28:[1,9],29:[1,10],30:[1,12]},{6:27,7:[1,28],13:7,14:[1,11],16:5,17:6,18:8,20:30,22:[1,29],28:[1,9],29:[1,10],30:[1,12]},{6:14,7:[1,16],8:[1,15],13:7,14:[1,11],15:[1,31],16:5,17:6,18:8,28:[1,9],29:[1,10],30:[1,12]},{5:[2,29],7:[2,29],8:[2,29],14:[2,29],15:[2,29],19:[2,29],21:[2,29],22:[1,23],24:[2,29],25:[2,29],27:32,28:[2,29],29:[2,29],30:[2,29]},{5:[2,23],7:[2,23],8:[2,23],14:[2,23],15:[2,23],19:[2,23],21:[2,23],22:[2,23],24:[2,23],25:[2,23],28:[2,23],29:[2,23],30:[2,23]},{4:33,6:3,7:[1,4],13:7,14:[1,11],16:5,17:6,18:8,28:[1,9],29:[1,10],30:[1,12]},{5:[2,5],7:[2,5],8:[2,5],14:[2,5],15:[2,5],24:[2,5],25:[2,5],28:[2,5],29:[2,5],30:[2,5]},{5:[2,6],7:[2,6],8:[2,6],14:[2,6],15:[2,6],24:[2,6],25:[2,6],28:[2,6],29:[2,6],30:[2,6]},{5:[2,16],7:[2,16],8:[2,16],14:[2,16],15:[2,16],24:[2,16],25:[2,16],28:[2,16],29:[2,16],30:[2,16]},{5:[2,18],7:[2,18],8:[2,18],14:[2,18],15:[2,18],24:[2,18],25:[2,18],28:[2,18],29:[2,18],30:[2,18]},{6:34,13:7,14:[1,11],16:5,17:6,18:8,28:[1,9],29:[1,10],30:[1,12]},{4:36,6:3,7:[1,4],13:7,14:[1,11],16:5,17:6,18:8,23:35,28:[1,9],29:[1,10],30:[1,12]},{5:[2,17],7:[2,17],8:[2,17],14:[2,17],15:[2,17],24:[2,17],25:[2,17],28:[2,17],29:[2,17],30:[2,17]},{5:[2,11],7:[2,11],8:[2,11],14:[2,11],15:[2,11],19:[2,11],21:[2,11],24:[2,11],25:[2,11],28:[2,11],29:[2,11],30:[2,11]},{5:[2,24],7:[2,24],8:[2,24],14:[2,24],15:[2,24],19:[2,24],21:[2,24],22:[2,24],24:[2,24],25:[2,24],28:[2,24],29:[2,24],30:[2,24]},{6:14,7:[1,16],8:[1,15],13:7,14:[1,11],16:5,17:6,18:8,24:[1,37],28:[1,9],29:[1,10],30:[1,12]},{5:[2,19],7:[2,19],8:[2,19],14:[2,19],15:[2,19],24:[2,19],25:[2,19],28:[2,19],29:[2,19],30:[2,19]},{24:[1,38],25:[1,39]},{6:14,7:[1,16],8:[1,15],13:7,14:[1,11],16:5,17:6,18:8,24:[2,21],25:[2,21],28:[1,9],29:[1,10],30:[1,12]},{5:[2,25],7:[2,25],8:[2,25],14:[2,25],15:[2,25],19:[2,25],21:[2,25],22:[2,25],24:[2,25],25:[2,25],28:[2,25],29:[2,25],30:[2,25]},{5:[2,20],7:[2,20],8:[2,20],14:[2,20],15:[2,20],24:[2,20],25:[2,20],28:[2,20],29:[2,20],30:[2,20]},{4:40,6:3,7:[1,4],13:7,14:[1,11],16:5,17:6,18:8,28:[1,9],29:[1,10],30:[1,12]},{6:14,7:[1,16],8:[1,15],13:7,14:[1,11],16:5,17:6,18:8,24:[2,22],25:[2,22],28:[1,9],29:[1,10],30:[1,12]}],
defaultActions: {13:[2,1]},
parseError: function parseError(str, hash) {
    if (hash.recoverable) {
        this.trace(str);
    } else {
        throw new Error(str);
    }
},
parse: function parse(input) {
    var self = this, stack = [0], vstack = [null], lstack = [], table = this.table, yytext = '', yylineno = 0, yyleng = 0, recovering = 0, TERROR = 2, EOF = 1;
    var args = lstack.slice.call(arguments, 1);
    this.lexer.setInput(input);
    this.lexer.yy = this.yy;
    this.yy.lexer = this.lexer;
    this.yy.parser = this;
    if (typeof this.lexer.yylloc == 'undefined') {
        this.lexer.yylloc = {};
    }
    var yyloc = this.lexer.yylloc;
    lstack.push(yyloc);
    var ranges = this.lexer.options && this.lexer.options.ranges;
    if (typeof this.yy.parseError === 'function') {
        this.parseError = this.yy.parseError;
    } else {
        this.parseError = Object.getPrototypeOf(this).parseError;
    }
    function popStack(n) {
        stack.length = stack.length - 2 * n;
        vstack.length = vstack.length - n;
        lstack.length = lstack.length - n;
    }
    function lex() {
        var token;
        token = self.lexer.lex() || EOF;
        if (typeof token !== 'number') {
            token = self.symbols_[token] || token;
        }
        return token;
    }
    var symbol, preErrorSymbol, state, action, a, r, yyval = {}, p, len, newState, expected;
    while (true) {
        state = stack[stack.length - 1];
        if (this.defaultActions[state]) {
            action = this.defaultActions[state];
        } else {
            if (symbol === null || typeof symbol == 'undefined') {
                symbol = lex();
            }
            action = table[state] && table[state][symbol];
        }
                    if (typeof action === 'undefined' || !action.length || !action[0]) {
                var errStr = '';
                expected = [];
                for (p in table[state]) {
                    if (this.terminals_[p] && p > TERROR) {
                        expected.push('\'' + this.terminals_[p] + '\'');
                    }
                }
                if (this.lexer.showPosition) {
                    errStr = 'Parse error on line ' + (yylineno + 1) + ':\n' + this.lexer.showPosition() + '\nExpecting ' + expected.join(', ') + ', got \'' + (this.terminals_[symbol] || symbol) + '\'';
                } else {
                    errStr = 'Parse error on line ' + (yylineno + 1) + ': Unexpected ' + (symbol == EOF ? 'end of input' : '\'' + (this.terminals_[symbol] || symbol) + '\'');
                }
                this.parseError(errStr, {
                    text: this.lexer.match,
                    token: this.terminals_[symbol] || symbol,
                    line: this.lexer.yylineno,
                    loc: yyloc,
                    expected: expected
                });
            }
        if (action[0] instanceof Array && action.length > 1) {
            throw new Error('Parse Error: multiple actions possible at state: ' + state + ', token: ' + symbol);
        }
        switch (action[0]) {
        case 1:
            stack.push(symbol);
            vstack.push(this.lexer.yytext);
            lstack.push(this.lexer.yylloc);
            stack.push(action[1]);
            symbol = null;
            if (!preErrorSymbol) {
                yyleng = this.lexer.yyleng;
                yytext = this.lexer.yytext;
                yylineno = this.lexer.yylineno;
                yyloc = this.lexer.yylloc;
                if (recovering > 0) {
                    recovering--;
                }
            } else {
                symbol = preErrorSymbol;
                preErrorSymbol = null;
            }
            break;
        case 2:
            len = this.productions_[action[1]][1];
            yyval.$ = vstack[vstack.length - len];
            yyval._$ = {
                first_line: lstack[lstack.length - (len || 1)].first_line,
                last_line: lstack[lstack.length - 1].last_line,
                first_column: lstack[lstack.length - (len || 1)].first_column,
                last_column: lstack[lstack.length - 1].last_column
            };
            if (ranges) {
                yyval._$.range = [
                    lstack[lstack.length - (len || 1)].range[0],
                    lstack[lstack.length - 1].range[1]
                ];
            }
            r = this.performAction.apply(yyval, [
                yytext,
                yyleng,
                yylineno,
                this.yy,
                action[1],
                vstack,
                lstack
            ].concat(args));
            if (typeof r !== 'undefined') {
                return r;
            }
            if (len) {
                stack = stack.slice(0, -1 * len * 2);
                vstack = vstack.slice(0, -1 * len);
                lstack = lstack.slice(0, -1 * len);
            }
            stack.push(this.productions_[action[1]][0]);
            vstack.push(yyval.$);
            lstack.push(yyval._$);
            newState = table[stack[stack.length - 2]][stack[stack.length - 1]];
            stack.push(newState);
            break;
        case 3:
            return true;
        }
    }
    return true;
}};
/* generated by jison-lex 0.2.1 */
var lexer = (function(){
var lexer = {

EOF:1,

parseError:function parseError(str, hash) {
        if (this.yy.parser) {
            this.yy.parser.parseError(str, hash);
        } else {
            throw new Error(str);
        }
    },

// resets the lexer, sets new input
setInput:function (input) {
        this._input = input;
        this._more = this._backtrack = this.done = false;
        this.yylineno = this.yyleng = 0;
        this.yytext = this.matched = this.match = '';
        this.conditionStack = ['INITIAL'];
        this.yylloc = {
            first_line: 1,
            first_column: 0,
            last_line: 1,
            last_column: 0
        };
        if (this.options.ranges) {
            this.yylloc.range = [0,0];
        }
        this.offset = 0;
        return this;
    },

// consumes and returns one char from the input
input:function () {
        var ch = this._input[0];
        this.yytext += ch;
        this.yyleng++;
        this.offset++;
        this.match += ch;
        this.matched += ch;
        var lines = ch.match(/(?:\r\n?|\n).*/g);
        if (lines) {
            this.yylineno++;
            this.yylloc.last_line++;
        } else {
            this.yylloc.last_column++;
        }
        if (this.options.ranges) {
            this.yylloc.range[1]++;
        }

        this._input = this._input.slice(1);
        return ch;
    },

// unshifts one char (or a string) into the input
unput:function (ch) {
        var len = ch.length;
        var lines = ch.split(/(?:\r\n?|\n)/g);

        this._input = ch + this._input;
        this.yytext = this.yytext.substr(0, this.yytext.length - len - 1);
        //this.yyleng -= len;
        this.offset -= len;
        var oldLines = this.match.split(/(?:\r\n?|\n)/g);
        this.match = this.match.substr(0, this.match.length - 1);
        this.matched = this.matched.substr(0, this.matched.length - 1);

        if (lines.length - 1) {
            this.yylineno -= lines.length - 1;
        }
        var r = this.yylloc.range;

        this.yylloc = {
            first_line: this.yylloc.first_line,
            last_line: this.yylineno + 1,
            first_column: this.yylloc.first_column,
            last_column: lines ?
                (lines.length === oldLines.length ? this.yylloc.first_column : 0)
                 + oldLines[oldLines.length - lines.length].length - lines[0].length :
              this.yylloc.first_column - len
        };

        if (this.options.ranges) {
            this.yylloc.range = [r[0], r[0] + this.yyleng - len];
        }
        this.yyleng = this.yytext.length;
        return this;
    },

// When called from action, caches matched text and appends it on next action
more:function () {
        this._more = true;
        return this;
    },

// When called from action, signals the lexer that this rule fails to match the input, so the next matching rule (regex) should be tested instead.
reject:function () {
        if (this.options.backtrack_lexer) {
            this._backtrack = true;
        } else {
            return this.parseError('Lexical error on line ' + (this.yylineno + 1) + '. You can only invoke reject() in the lexer when the lexer is of the backtracking persuasion (options.backtrack_lexer = true).\n' + this.showPosition(), {
                text: "",
                token: null,
                line: this.yylineno
            });

        }
        return this;
    },

// retain first n characters of the match
less:function (n) {
        this.unput(this.match.slice(n));
    },

// displays already matched input, i.e. for error messages
pastInput:function () {
        var past = this.matched.substr(0, this.matched.length - this.match.length);
        return (past.length > 20 ? '...':'') + past.substr(-20).replace(/\n/g, "");
    },

// displays upcoming input, i.e. for error messages
upcomingInput:function () {
        var next = this.match;
        if (next.length < 20) {
            next += this._input.substr(0, 20-next.length);
        }
        return (next.substr(0,20) + (next.length > 20 ? '...' : '')).replace(/\n/g, "");
    },

// displays the character position where the lexing error occurred, i.e. for error messages
showPosition:function () {
        var pre = this.pastInput();
        var c = new Array(pre.length + 1).join("-");
        return pre + this.upcomingInput() + "\n" + c + "^";
    },

// test the lexed token: return FALSE when not a match, otherwise return token
test_match:function (match, indexed_rule) {
        var token,
            lines,
            backup;

        if (this.options.backtrack_lexer) {
            // save context
            backup = {
                yylineno: this.yylineno,
                yylloc: {
                    first_line: this.yylloc.first_line,
                    last_line: this.last_line,
                    first_column: this.yylloc.first_column,
                    last_column: this.yylloc.last_column
                },
                yytext: this.yytext,
                match: this.match,
                matches: this.matches,
                matched: this.matched,
                yyleng: this.yyleng,
                offset: this.offset,
                _more: this._more,
                _input: this._input,
                yy: this.yy,
                conditionStack: this.conditionStack.slice(0),
                done: this.done
            };
            if (this.options.ranges) {
                backup.yylloc.range = this.yylloc.range.slice(0);
            }
        }

        lines = match[0].match(/(?:\r\n?|\n).*/g);
        if (lines) {
            this.yylineno += lines.length;
        }
        this.yylloc = {
            first_line: this.yylloc.last_line,
            last_line: this.yylineno + 1,
            first_column: this.yylloc.last_column,
            last_column: lines ?
                         lines[lines.length - 1].length - lines[lines.length - 1].match(/\r?\n?/)[0].length :
                         this.yylloc.last_column + match[0].length
        };
        this.yytext += match[0];
        this.match += match[0];
        this.matches = match;
        this.yyleng = this.yytext.length;
        if (this.options.ranges) {
            this.yylloc.range = [this.offset, this.offset += this.yyleng];
        }
        this._more = false;
        this._backtrack = false;
        this._input = this._input.slice(match[0].length);
        this.matched += match[0];
        token = this.performAction.call(this, this.yy, this, indexed_rule, this.conditionStack[this.conditionStack.length - 1]);
        if (this.done && this._input) {
            this.done = false;
        }
        if (token) {
            return token;
        } else if (this._backtrack) {
            // recover context
            for (var k in backup) {
                this[k] = backup[k];
            }
            return false; // rule action called reject() implying the next rule should be tested instead.
        }
        return false;
    },

// return next match in input
next:function () {
        if (this.done) {
            return this.EOF;
        }
        if (!this._input) {
            this.done = true;
        }

        var token,
            match,
            tempMatch,
            index;
        if (!this._more) {
            this.yytext = '';
            this.match = '';
        }
        var rules = this._currentRules();
        for (var i = 0; i < rules.length; i++) {
            tempMatch = this._input.match(this.rules[rules[i]]);
            if (tempMatch && (!match || tempMatch[0].length > match[0].length)) {
                match = tempMatch;
                index = i;
                if (this.options.backtrack_lexer) {
                    token = this.test_match(tempMatch, rules[i]);
                    if (token !== false) {
                        return token;
                    } else if (this._backtrack) {
                        match = false;
                        continue; // rule action called reject() implying a rule MISmatch.
                    } else {
                        // else: this is a lexer rule which consumes input without producing a token (e.g. whitespace)
                        return false;
                    }
                } else if (!this.options.flex) {
                    break;
                }
            }
        }
        if (match) {
            token = this.test_match(match, rules[index]);
            if (token !== false) {
                return token;
            }
            // else: this is a lexer rule which consumes input without producing a token (e.g. whitespace)
            return false;
        }
        if (this._input === "") {
            return this.EOF;
        } else {
            return this.parseError('Lexical error on line ' + (this.yylineno + 1) + '. Unrecognized text.\n' + this.showPosition(), {
                text: "",
                token: null,
                line: this.yylineno
            });
        }
    },

// return next match that has a token
lex:function lex() {
        var r = this.next();
        if (r) {
            return r;
        } else {
            return this.lex();
        }
    },

// activates a new lexer condition state (pushes the new lexer condition state onto the condition stack)
begin:function begin(condition) {
        this.conditionStack.push(condition);
    },

// pop the previously active lexer condition state off the condition stack
popState:function popState() {
        var n = this.conditionStack.length - 1;
        if (n > 0) {
            return this.conditionStack.pop();
        } else {
            return this.conditionStack[0];
        }
    },

// produce the lexer rule set which is active for the currently active lexer condition state
_currentRules:function _currentRules() {
        if (this.conditionStack.length && this.conditionStack[this.conditionStack.length - 1]) {
            return this.conditions[this.conditionStack[this.conditionStack.length - 1]].rules;
        } else {
            return this.conditions["INITIAL"].rules;
        }
    },

// return the currently active lexer condition state; when an index argument is provided it produces the N-th previous condition state, if available
topState:function topState(n) {
        n = this.conditionStack.length - 1 - Math.abs(n || 0);
        if (n >= 0) {
            return this.conditionStack[n];
        } else {
            return "INITIAL";
        }
    },

// alias for begin(condition)
pushState:function pushState(condition) {
        this.begin(condition);
    },

// return the number of states currently on the stack
stateStackSize:function stateStackSize() {
        return this.conditionStack.length;
    },
options: {},
performAction: function anonymous(yy,yy_,$avoiding_name_collisions,YY_START) {

var YYSTATE=YY_START;
switch($avoiding_name_collisions) {
case 0:/* white spaces ignored */
break;
case 1:/* ignore comments at end of line*/
break;
case 2:return 29                     /* Numbers (integer of float) */
break;
case 3:return 8                   /* A math operator to separate words */
break;
case 4:return 11;                         /* Mult operator              */
break;
case 5:return 12;                         /* Div operator               */
break;
case 6:return 7;                         /* Minus operator             */
break;
case 7:return 10;                         /* + operator                 */
break;
case 8:return 14;                         /* Start group                */
break;
case 9:return 15;                         /* End group                  */
break;
case 10:return 30;                 /* Function name (protected by \ and multi-char) */
break;
case 11:return 28;                /* Short variable name (1 letter) */
break;
case 12:return 22;                         /* Start protected parameter  */
break;
case 13:return 24;                         /* End protected parameter    */
break;
case 14:return 19;                         /* Start exponent decoration  */
break;
case 15:return 21;                         /* Start indice decoration    */
break;
case 16:return 25;                         /* Parameter separator        */
break;
case 17:return 5;
break;
}
},
rules: [/^(?:\s+)/,/^(?:%%.*$)/,/^(?:[0-9]+(\.[0-9]+)?)/,/^(?:[*/+])/,/^(?:\*)/,/^(?:\/)/,/^(?:-)/,/^(?:\+)/,/^(?:\()/,/^(?:\))/,/^(?:\\[A-Za-z][A-Za-z0-9]*)/,/^(?:[A-Za-z])/,/^(?:\{)/,/^(?:\})/,/^(?:\^)/,/^(?:@)/,/^(?:,)/,/^(?:$)/],
conditions: {"INITIAL":{"rules":[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17],"inclusive":true}}
};
return lexer;
})();
parser.lexer = lexer;
function Parser () {
  this.yy = {};
}
Parser.prototype = parser;parser.Parser = Parser;
return new Parser;
})();


if (typeof require !== 'undefined' && typeof exports !== 'undefined') {
exports.parser = latexParser;
exports.Parser = latexParser.Parser;
exports.parse = function () { return latexParser.parse.apply(latexParser, arguments); };
exports.main = function commonjsMain(args) {
    if (!args[1]) {
        console.log('Usage: '+args[0]+' FILE');
        process.exit(1);
    }
    var source = require('fs').readFileSync(require('path').normalize(args[1]), "utf8");
    return exports.parser.parse(source);
};
if (typeof module !== 'undefined' && require.main === module) {
  exports.main(process.argv.slice(1));
}
}