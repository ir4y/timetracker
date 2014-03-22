TimeTrackerApp.config(['$stateProvider', function($stateProvider) {
    $stateProvider
    .state('main.projects', {
        abstract: true,
        url: 'projects/',
        views: {
            'content': {
                controller: function($scope) { console.log($scope); },
                template: '<div ui-view></div>'
            }
        }
    })
    .state('main.projects.list', {
        url: 'list/',
        views: {
            '': {
                templateUrl: '/src/app/projects/templates/projects-list.html',
                controller: 'ProjectsListCtrl',
            }
        }
    });
}]);
