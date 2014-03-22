TimeTrackerApp.config(['$stateProvider', function($stateProvider) {
    $stateProvider
    .state('root.projects', {
        url: 'projects/:start_date/:end_date/',
        resolve: {
            projects: function($rpc){
                return $rpc.send('tasks.get_projects_list', {}).then(
                    function(response) { 
                        return response.result;
                    }
                );
            },
            users: function($rpc){
                return $rpc.send('tasks.get_users_list', {}).then(
                    function(response) {
                        return response.result;
                    }
                );
            },
            actions: function($rpc, $stateParams){
                var start_date = new Date(parseInt($stateParams.start_date)),
                    end_date = new Date(parseInt($stateParams.end_date));
                return $rpc.send('tasks.get_actions_list', [
                    end_date.toLocaleString(),
                    start_date.toLocaleString(), 
                ]).then(
                    function(response) {
                        return response.result;
                    }
                );
            }
        },
        views: {
            'content': {
                templateUrl: '/static/src/app/projects/templates/projects.html',
                controller: ['$stateParams', '$scope', 'projects', 'users', 'actions',
                function($stateParams, $scope, projects, users, actions){
                    $scope.projects = projects;
                    $scope.users = users;
                    $scope.actions = actions;

                    var gen_time_list = function(start_date, end_date, interval) {
                        interval = interval || 10;
                        var start_date = new Date(start_date),
                            end_date = new Date(end_date),
                            i = 0;
                        var time_list = [];
                        for(i = 0; (new Date(start_date.getTime()-i*60*1000) > end_date); i += interval) {
                           time_list.push({
                               start_date: new Date( start_date.getTime()-i*60*1000), 
                               end_date: new Date(start_date.getTime()-(i+interval)*60*1000)
                           }); 
                        }
                        return time_list;
                    };

                    var start_date = new Date(parseInt($stateParams.start_date)),
                        end_date = new Date(parseInt($stateParams.end_date));

                    $scope.time_list = gen_time_list(start_date, end_date);

                }]
            }
        }
    })
    .state('root.projects.tasks', {
        url: ':project_id/',
        resolve: {
            tasks: function($rpc, $stateParams){
                return $rpc.send('tasks.get_tasks_for_project', [$stateParams.project_id]).then(
                    function(response) {
                        return response.result;
                    }
                );
            }
        },
        views: {
            'lists': {
                templateUrl: '/static/src/app/projects/templates/tasks.html',
                controller: 'TasksCtrl',
            }
        }
    });
}]);

TimeTrackerApp.controller('TasksCtrl', ['$scope', 'tasks', function($scope, tasks){ 
    $scope.tasks = tasks;
}]);

TimeTrackerApp.filter('user_actions', function() {
    return function(actions, user) {
        return actions.filter(function(action){return action.user==user.id});
  };
});

TimeTrackerApp.filter('in_period', function() {
    return function(actions, time_period) {
        var start_date = new Date(time_period.start_date),
            end_date = new Date(time_period.end_date);
        return actions.filter(function(action){ 
            var date = new Date(action.created);
            console.log(date, end_date, start_date);
            return date >= end_date && date <= start_date; 
        });
  };
});

