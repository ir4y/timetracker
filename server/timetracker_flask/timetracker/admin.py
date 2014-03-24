from flask.ext import admin
from flask.ext.admin.contrib.mongoengine import ModelView

from timetracker import app
from timetracker.models import User, Project, Task, Action, Screenshot


admin = admin.Admin(app, 'Timetracker Models')
admin.add_view(ModelView(User))
admin.add_view(ModelView(Project))
admin.add_view(ModelView(Task))


class ActionView(ModelView):
    column_list = ('task', 'created', 'action', 'user', 'screenshot', )


admin.add_view(ActionView(Action))
admin.add_view(ModelView(Screenshot))
