TimeTrackerApp.config(['$stateProvider', function($stateProvider) {
    $stateProvider
    .state('root.projects', {
        abstract: true,
        url: 'projects/',
        views: {
            'content': {
                templateUrl: '/static/src/app/projects/templates/projects-base.html',
                controller: ['$scope', '$rpc', function($scope, $rpc){
                    $rpc.send('tasks.get_projects_list', {}).then( 
                        function(response) { 
                            $scope.projects = response.result;
                        }
                    );
                }]
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

TimeTrackerApp.controller('ProjectsListCtrl', ['$scope', function($scope){ console.log($scope);
}]);
