// mymodule.js
var parser = require("./latex-parser").parser;

function exec (input) {
    return parser.parse(input);
}

var stdin = process.stdin;
stdin.on('data', function(data) {
	var tmp = data.toString().split('\n');
	for (var i in tmp)
	{
		console.log("%s",tmp[i]);
		var twenty = exec(tmp[i]);
		console.log("%s",twenty);
	}
});
