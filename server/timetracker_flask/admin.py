from flask.ext import admin
from app import app
from models import User, Project, Task, Action


admin = admin.Admin(app, 'Simple Models')

admin.
