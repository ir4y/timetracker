TimeTrackerApp = angular.module("TimeTrackerApp", ['ui.router', 'ngRpc']);

TimeTrackerApp.config(['$stateProvider', '$urlRouterProvider', '$rpcProvider', 
    function ($stateProvider, $urlRouterProvider, $rpcProvider) {
    $urlRouterProvider.otherwise('/projects/list/');
    $rpcProvider.setUrl('ws://' + window.location.host + '/bullet');
    $rpcProvider.connect();

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
