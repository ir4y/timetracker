from os.path import join
import datetime
import json
import pytz
import redis
import decimal
from flask import Flask, request
from flask.ext import admin
from flask.ext import restful
from flask.ext.login import LoginManager, login_user, current_user
from flask.ext.admin.contrib.mongoengine import ModelView
from flask.ext.mongoengine import MongoEngine
from flask.ext.mongorest import MongoRest
from flask.ext.mongorest.views import ResourceView
from flask.ext.mongorest.resources import Resource
from flask.ext.mongorest import operators as ops
from flask.ext.mongorest import methods
from flask.ext.mongorest.authentication import AuthenticationBase
from kombu.serialization import register
from mongoengine import signals

from bson.dbref import DBRef
from bson.objectid import ObjectId

class TimezoneMongoEncoder(json.JSONEncoder):
    def default(self, value, **kwargs):
        if isinstance(value, ObjectId):
            return unicode(value)
        elif isinstance(value, DBRef):
            return value.id
        if isinstance(value, datetime.datetime):
            value -= datetime.timedelta(hours=8)
            return value.isoformat()
        if isinstance(value, datetime.date):
            return value.strftime("%Y-%m-%d")
        if isinstance(value, decimal.Decimal):
            return str(value)
        return super(MongoEncoder, self).default(value, **kwargs)


def mongo_encoder(data):
    return json.dumps(data, cls=TimezoneMongoEncoder)


register('mongo_json', mongo_encoder, json.loads, 'application/json')

redis_client = redis.StrictRedis(host='localhost', port=6379, db=0)

app = Flask(__name__)
app.config['SECRET_KEY'] = '123456790'
app.config['MONGODB_SETTINGS'] = {'DB': 'timetracker'}
app.config['CELERY_BROKER_URL'] = 'amqp://'
app.config['UPLOAD_PATH'] = '/tmp/'

db = MongoEngine(app)
api = MongoRest(app)
login_manager = LoginManager()
login_manager.init_app(app)


class User(db.Document):
    name = db.StringField(max_length=40)
    email = db.StringField(max_length=40)
    password = db.StringField(max_length=40)

    def is_active(self):
        return True

    def get_id(self):
        return str(self.id)

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

    @property
    def screenshot(self):
        if self.image:
            return self.image.image
        else:
            return None

    @classmethod
    def post_save(cls, sender, document, **kwargs):
        redis_client.publish('action', json.dumps(document._data,
                                                  cls=TimezoneMongoEncoder))

    meta = {
        'ordering': ['-created']
    }

signals.post_save.connect(Action.post_save, sender=Action)


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


class Screenshot(db.Document):
    image = db.ImageField()


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

admin = admin.Admin(app, 'Simple Models')
admin.add_view(ModelView(User))
admin.add_view(ModelView(Project))
admin.add_view(ModelView(Task))


class ActionView(ModelView):
    column_list = ('task', 'created', 'action', 'user', 'screenshot', )


admin.add_view(ActionView(Action))
admin.add_view(ModelView(Screenshot))


if __name__ == '__main__':
    app.run(debug=True)
