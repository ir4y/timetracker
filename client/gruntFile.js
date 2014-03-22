module.exports = function(grunt) {

  // Project configuration.
  grunt.initConfig({
    pkg: grunt.file.readJSON('package.json'),
      concat: {
        options: {
          separator: ';',
        },
      dist: {
        src: ['vendor/angular/angular.js', 'vendor/angular-rpc/src/angular-rpc.js', 'vendor/angular-ui-router/release/angular-ui-router.js'],
        dest: 'src/main.js',
      },
    },
    watch: {
      dev: {
        files: ['src/**/*.js', 'vendor/**/*.js'],
        tasks: ['concat'],
        options: {
          spawn: false,
        },
      },
    },
  });

  grunt.loadNpmTasks('grunt-contrib-watch');
  grunt.loadNpmTasks('grunt-contrib-concat');
};