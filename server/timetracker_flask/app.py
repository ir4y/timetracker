from os.path import join
import datetime
import json
from flask import Flask, request
from flask.ext import admin
from flask.ext.admin.contrib.mongoengine import ModelView
from flask.ext.mongoengine import MongoEngine
from flask.ext.mongorest import MongoRest
from flask.ext.mongorest.views import ResourceView
from flask.ext.mongorest.resources import Resource
from flask.ext.mongorest import operators as ops
from flask.ext.mongorest import methods
from flask.ext.mongorest.utils import MongoEncoder
from kombu.serialization import register

def mongo_encoder(data):
    return json.dumps(data, cls=MongoEncoder)

register('mongo_json', mongo_encoder, json.loads, 'application/json')


app = Flask(__name__)
app.config['SECRET_KEY'] = '123456790'
app.config['MONGODB_SETTINGS'] = {'DB': 'timetracker'}
app.config['CELERY_BROKER_URL'] = 'amqp://'
app.config['UPLOAD_PATH'] = '/tmp/'

db = MongoEngine(app)
api = MongoRest(app)


class User(db.Document):
    name = db.StringField(max_length=40)
    email = db.StringField(max_length=40)
    password = db.StringField(max_length=40)

    def __unicode__(self):
        return self.name


class Project(db.Document):
    title = db.StringField()
    users = db.ListField(db.ReferenceField('User'))

    def __unicode__(self):
        return self.title


class Task(db.Document):
    project = db.ReferenceField('Project')
    title = db.StringField()
    completed = db.BooleanField(default=False)

    def __unicode__(self):
        return self.title


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
    image = db.ReferenceField('Screenshot')

class Screenshot(db.Document):
    image = db.ImageField()


class UserResource(Resource):
    document = User


class ProjectResource(Resource):
    document = Project


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


admin = admin.Admin(app, 'Simple Models')
admin.add_view(ModelView(User))
admin.add_view(ModelView(Project))
admin.add_view(ModelView(Task))
admin.add_view(ModelView(Action))
admin.add_view(ModelView(Screenshot))


if __name__ == '__main__':
    app.run(debug=True)
