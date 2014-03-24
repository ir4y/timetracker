import json
import datetime
from os.path import join
from flask import request
from flask.ext import restful
from flask.ext.login import login_user, current_user
from flask.ext.mongorest import MongoRest, operators as ops
from flask.ext.mongorest import methods
from flask.ext.mongorest.authentication import AuthenticationBase
from flask.ext.mongorest.resources import Resource
from flask.ext.mongorest.views import ResourceView

from timetracker import app
from timetracker.models import User, Project, Task, Action, Screenshot


class UserIdAuthentication(AuthenticationBase):
    def authorized(self):
        if 'AUTHORIZATION' in request.headers:
            authorization = request.headers['AUTHORIZATION']
            try:
                login_user(User.objects.get(id=authorization))
                return True
            except:
                return False
        return False


class UserResource(Resource):
    document = User


class ProjectResource(Resource):
    document = Project

    def get_queryset(self):
        return self.document.objects.filter(
            users=current_user.id)


class TaskResource(Resource):
    document = Task
    filters = {
        'project': [ops.Exact, ]
    }


class ActionResource(Resource):
    document = Action
    filters = {
        'task': [ops.Exact, ]
    }


api = MongoRest(app)


@api.register(name='user', url='/api/user/')
class UserView(ResourceView):
    authentication_methods = [UserIdAuthentication]
    resource = UserResource
    methods = [methods.Create, methods.Update, methods.Fetch, methods.List]


@api.register(name='project', url='/api/project/')
class ProjectView(ResourceView):
    authentication_methods = [UserIdAuthentication]
    resource = ProjectResource
    methods = [methods.Create, methods.Update, methods.Fetch, methods.List]


@api.register(name='task', url='/api/task/')
class TaskView(ResourceView):
    authentication_methods = [UserIdAuthentication]
    resource = TaskResource
    methods = [methods.Create, methods.Update, methods.Fetch, methods.List]


@api.register(name='action', url='/api/action/')
class ActionView(ResourceView):
    authentication_methods = [UserIdAuthentication]
    resource = ActionResource
    methods = [methods.Create, methods.Update, methods.Fetch, methods.List]


@app.route('/api/upload/', methods=['POST'])
def upload():
    new_file = join(
        app.config['UPLOAD_PATH'],
        datetime.datetime.now().strftime(
            "%Y.%m.%d_%H:%M.png"))
    file = open(new_file, "w")
    file.write(request.data)
    file.close()

    screenshot = Screenshot()
    screenshot.image.put(open(new_file))
    screenshot.save()
    return str(screenshot.id)


system_api = restful.Api(app)


class LoginResource(restful.Resource):
    def post(self):
        try:
            user = User.objects.get(**json.loads(request.data))
            last_action = Action.objects.filter(
                user=user).order_by('-created').first()
            return {'user_id': str(user.id),
                    'last_task_id': str(last_action.task.id)
                    if last_action else None}
        except:
            return {'error': 'no such user'}, 400

system_api.add_resource(LoginResource, '/api/login/')
