MainApp = angular.module("TimeTrackerApp", []);

MainApp.config(['$stateProvider', '$urlRouterProvider', function ($stateProvider, $urlRouterProvider) {
    //$locationProvider.html5Mode(true);
    $routeProvider.otherwise('/');
    $stateProvider.state({
        url: '/'
    });
}]);
