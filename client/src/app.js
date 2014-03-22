TimeTrackerApp = angular.module("TimeTrackerApp", ['uiRouter']);

TimeTrackerApp.config(['$stateProvider', '$urlRouterProvider', function ($stateProvider, $urlRouterProvider) {
    //$locationProvider.html5Mode(true);
    $routeProvider.otherwise('/');
    $stateProvider.state({
        url: '/'
    });
}]);
