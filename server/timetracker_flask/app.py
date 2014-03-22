import datetime
from flask import Flask
from flask.ext import admin
from flask.ext.admin.contrib.mongoengine import ModelView
from flask.ext.mongoengine import MongoEngine
from flask.ext.mongorest import MongoRest
from flask.ext.mongorest.views import ResourceView
from flask.ext.mongorest.resources import Resource
from flask.ext.mongorest import operators as ops
from flask.ext.mongorest import methods


app = Flask(__name__)
app.config['SECRET_KEY'] = '123456790'
app.config['MONGODB_SETTINGS'] = {'DB': 'timetracker'}


db = MongoEngine(app)
api = MongoRest(app)


class User(db.Document):
    name = db.StringField(max_length=40)
    email = db.StringField(max_length=40)
    password = db.StringField(max_length=40)


class Project(db.Document):
    title = db.StringField()
    users = db.ListField(db.ReferenceField('User'))


class Task(db.Document):
    project = db.ReferenceField('Project')
    title = db.StringField()
    compleated = db.BooleanField(default=False)


ACTION_CHOICES = (
    ('resume', 'Resume'),
    ('suspend', 'Suspend'),
    ('screenshot', 'Screenshot'),
)


class Action(db.Document):
    task = db.ReferenceField('Task')
    created = db.DateTimeField(default=datetime.datetime.now)
    action = db.StringField(choices=ACTION_CHOICES)
    user = db.ReferenceField('User')
    image = db.StringField()


class UserResource(Resource):
    document = User


class ProjectResource(Resource):
    document = Project


class TaskResource(Resource):
    document = Task


class ActionResource(Resource):
    document = Action


@api.register(name='user', url='/api/user/')
class UserView(ResourceView):
    resource = UserResource
    methods = [methods.Create, methods.Update, methods.Fetch, methods.List]


@api.register(name='project', url='/api/project/')
class ProjectView(ResourceView):
    resource = ProjectResource
    methods = [methods.Create, methods.Update, methods.Fetch, methods.List]


@api.register(name='task', url='/api/task/')
class TaskView(ResourceView):
    resource = TaskResource
    methods = [methods.Create, methods.Update, methods.Fetch, methods.List]

@api.register(name='action', url='/api/action/')
class ActionView(ResourceView):
    resource = ActionResource
    methods = [methods.Create, methods.Update, methods.Fetch, methods.List]


admin = admin.Admin(app, 'Simple Models')
admin.add_view(ModelView(User))
admin.add_view(ModelView(Project))
admin.add_view(ModelView(Task))
admin.add_view(ModelView(Action))





if __name__ == '__main__':
    app.run(debug=True)
