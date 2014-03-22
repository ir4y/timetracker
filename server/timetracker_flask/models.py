import datetime
from flask.ext.mongoengine import MongoEngine
from app import app

db = MongoEngine()
db.init_app(app)


class User(db.Document):
    name = db.StringField(max_length=40)
    tags = db.ListField(db.ReferenceField('Tag'))
    password = db.StringField(max_length=40)


class Project(db.Document):
    title = db.StringFields()
    users = db.ListField(db.ReferenceField('User'))


class Task(db.Document):
    project = db.ReferenceField('Project')
    title = db.StringFields()
    compleated = db.BooleanField(default=False)


ACTION_CHOICES = (
    ('resume', 'Resume'),
    ('suspend', 'Suspend'),
    ('screenshot', 'Screenshot'),
)

class Action(db.Document):
    task = db.ReferenceField('Task')
    created = db.DateTimeField(default=datetime.datetime.now)
    action = db.StringFields(choices=ACTION_CHOICES)
    user = db.ReferenceField('User')
    image = db.ImageField()
