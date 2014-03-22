module.exports = function(grunt) {

  // Project configuration.
  grunt.initConfig({
    pkg: grunt.file.readJSON('package.json'),
    concat: {
      options: {
        separator: ';',
      },
      dist: {
        src: ['vendor/jquery/dist/jquery.js',
              'vendor/bootstrap/dist/js/bootstrap.js',
              'vendor/angular/angular.js',
              'vendor/angular-rpc/src/angular-rpc.js',
              'vendor/angular-ui-router/release/angular-ui-router.js',
              'src/**/*.js'],
        dest: 'application.js',
      },
      css : {
        src : ['vendor/bootstrap/dist/css/bootstrap.css'],
        dest : 'main.css'

      }
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
