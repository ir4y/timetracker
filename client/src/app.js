TimeTrackerApp = angular.module("TimeTrackerApp", ['ui.router']);

TimeTrackerApp.config(['$stateProvider', '$urlRouterProvider', function ($stateProvider, $urlRouterProvider) {
    //$locationProvider.html5Mode(true);
    $urlRouterProvider.otherwise('/projects/list/');

    $stateProvider
    .state('root', {
        url: '/',
        views: {
            '': {
                controller: 'RootCtrl',
                template: '<div ui-view="content"></div>'
            }
        }
    });
}]);

TimeTrackerApp.controller('MainCtrl', ['$scope', function($scope){

}]);

TimeTrackerApp.controller('RootCtrl', ['$scope', function($scope){

}]);
