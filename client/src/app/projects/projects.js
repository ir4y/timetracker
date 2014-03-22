TimeTrackerApp.config(['$stateProvider', function($stateProvider) {
    $stateProvider
    .state('root.projects', {
        abstract: true,
        url: 'projects/',
        resolve: {
            projects: [ '$rpc', function($rpc) {
                //$rpc.send().then(
                    //function(response){
                        //return response.result;
                    //},
                    //function(response){
                        //console.log('Error', response);
                    //}
                //)
            }]
        },
        views: {
            'content': {
                templateUrl: '/static/src/app/projects/templates/projects-base.html',
            }
        }
    })
    .state('root.projects.list', {
        url: 'list/',
        views: {
            '': {
                templateUrl: '/static/src/app/projects/templates/projects-list.html',
                controller: 'ProjectsListCtrl',
            }
        }
    });
}]);

TimeTrackerApp.controller('ProjectsListCtrl', ['$scope', function($scope){
    console.log($scope);
}]);
