MainApp.config(['$stateProvider', function($stateProvider) ->
    $stateProvider.state('projects',{
        abstract: true,
        url: 'projects/',
        resolve: function(){
            return [];
        } 
    })
    .state('projects.list', {
        url: 'list/',
        views: {
            'content': {
                templateUrl: '/src/app/projects/templates/projects-list.html',
                controller: 'ProjectsListCtrl',
            }
        }
    }
    );
]);
