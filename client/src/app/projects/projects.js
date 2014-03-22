TimeTrackerApp.config(['$stateProvider', function($stateProvider) {
    $stateProvider
    .state('root.projects', {
        abstract: true,
        url: 'projects/',
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
