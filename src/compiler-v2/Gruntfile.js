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
		}
	})
	
	//load plugins
	grunt.loadNpmTasks('grunt-jison');
	
	//register tasks
	grunt.registerTask('default', ['jison']);
}
