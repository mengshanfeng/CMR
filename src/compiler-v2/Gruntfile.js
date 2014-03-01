'use strict';

/** Configure grunt operations **/
module.exports = function(grunt) {
	//setup configs
	grunt.initConfig({
		//Configs to compile jison files to runnable js modules
		jison: {
			'latex-parsor' : {
				options: {moduleType : 'commonjs'},
				files: {'build/latex-parser.js':'latex-parser.jison'}
			}
		},
		
		nodeunit: {
			all: ['./*-test.js'],
		}
	})
	
	//load plugins
	grunt.loadNpmTasks('grunt-jison');
	grunt.loadNpmTasks('grunt-contrib-nodeunit');
	
	//register tasks
	grunt.registerTask('test', ['nodeunit']);
	grunt.registerTask('default', ['jison']);
}
