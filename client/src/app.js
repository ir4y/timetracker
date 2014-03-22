TimeTrackerApp = angular.module("TimeTrackerApp", ['ui.router', 'ngRpc']);

angular.element(document).ready(function() {
    angular.bootstrap(document, ['TimeTrackerApp']);
});

TimeTrackerApp.config(['$stateProvider', '$urlRouterProvider', '$rpcProvider', 
    function ($stateProvider, $urlRouterProvider, $rpcProvider) {
        $urlRouterProvider.otherwise('/');
        $rpcProvider.setUrl('ws://' + window.location.host + '/bullet');
        $rpcProvider.connect();
    }]);

TimeTrackerApp.config(['$stateProvider', function($stateProvider) {
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


TimeTrackerApp.controller('RootCtrl', ['$scope', function($scope){
}]);
