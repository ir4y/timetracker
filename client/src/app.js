TimeTrackerApp = angular.module("TimeTrackerApp", ['ui.router', 'ngRpc']);

angular.element(document).ready(function() {
    angular.bootstrap(document, ['TimeTrackerApp']);
});

TimeTrackerApp.config(['$stateProvider', '$urlRouterProvider', '$rpcProvider', 
    function ($stateProvider, $urlRouterProvider, $rpcProvider) {
        var start_interval = 60*60*2;
        var start_date = new Date(new Date().getTime()+start_interval*500),
            end_date = new Date(start_date.getTime()-start_interval*1000)
        $urlRouterProvider.otherwise('/projects/'+start_date.getTime()+'/'+end_date.getTime()+'/');
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
