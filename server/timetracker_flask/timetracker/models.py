import json

import datetime
from flask.ext.mongoengine import MongoEngine
from mongoengine import signals

from timetracker import app
from timetracker.utils import TimezoneMongoEncoder, redis_client


db = MongoEngine(app)


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


class Screenshot(db.Document):
    image = db.ImageField()
