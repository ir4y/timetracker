TimeTrackerApp = angular.module("TimeTrackerApp", ['ui.router', 'ngRpc']);

TimeTrackerApp.config(['$stateProvider', '$urlRouterProvider', '$rpcProvider', 
    function ($stateProvider, $urlRouterProvider, $rpcProvider) {
        $urlRouterProvider.otherwise('/');
        $rpcProvider.setUrl('ws://' + window.location.host + '/bullet');
    }]);

TimeTrackerApp.config(['$stateProvider', function($stateProvider) {
    $stateProvider
    .state('root', {
        url: '/',
        resolve: {
            socket: function($rpc) {
                return $rpc.connect();
                //.then(function(socket){return socket;});
            }
        },
        views: {
            '': {
                controller: 'RootCtrl',
                template: '<div ui-view="content"></div>'
            }
        }
    });
}]);


TimeTrackerApp.controller('MainCtrl', ['$scope', function($scope){
    console.log('MainCtrl');
}]);
TimeTrackerApp.controller('RootCtrl', ['$scope', function($scope){
    console.log('RootCtrl');
}]);
