TimeTrackerApp = angular.module("TimeTrackerApp", ['ui.router']);

TimeTrackerApp.config(['$stateProvider', '$urlRouterProvider', function ($stateProvider, $urlRouterProvider) {
    //$locationProvider.html5Mode(true);
    $urlRouterProvider.otherwise('/projects/list/');
    $stateProvider.state('main', {
        url: '/',
        controller: function($scope){}
    });
}]);

TimeTrackerApp.controller('MainCtrl', ['$scope', function($scope){
console.log('MainCtrl');
}]);
