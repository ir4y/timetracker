import json

from flask import Flask
from flask.ext.login import LoginManager
from kombu.serialization import register

from timetracker.utils import mongo_encoder


register('mongo_json', mongo_encoder, json.loads, 'application/json')


app = Flask(__name__)

app.config['SECRET_KEY'] = '123456790'
app.config['MONGODB_SETTINGS'] = {'DB': 'timetracker'}
app.config['CELERY_BROKER_URL'] = 'amqp://'
app.config['UPLOAD_PATH'] = '/tmp/'

login_manager = LoginManager()
login_manager.init_app(app)

import timetracker.models
import timetracker.api
import timetracker.admin
