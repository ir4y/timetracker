TimeTrackerApp.config(['$stateProvider', function($stateProvider) {
    $stateProvider
    .state('root.projects', {
        abstract: true,
        url: 'projects/',
        views: {
            'content': {
                template: '<div ui-view></div>'
            }
        }
    })
    .state('root.projects.list', {
        url: 'list/',
        views: {
            '': {
                template: '/src/app/projects/templates/projects-list.html',
                controller: 'ProjectsListCtrl',
            }
        }
    });
}]);

TimeTrackerApp.controller('ProjectsListCtrl', ['$scope', function($scope){
    console.log($scope);
}]);
