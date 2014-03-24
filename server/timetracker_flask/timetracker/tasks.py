from celery import Celery

from timetracker import app as flask_app
from timetracker.models import User, Project, Task, Action


def make_celery(app):
    celery = Celery(app.import_name,
                    backend='amqp',
                    broker=app.config['CELERY_BROKER_URL'])
    celery.conf.update(app.config)
    celery.conf.update(
        CELERY_TASK_SERIALIZER='json',
        CELERY_ACCEPT_CONTENT=['json'],  # Ignore other content
        CELERY_RESULT_SERIALIZER='mongo_json', )
    TaskBase = celery.Task

    class ContextTask(TaskBase):
        abstract = True

        def __call__(self, *args, **kwargs):
            with app.app_context():
                return TaskBase.__call__(self, *args, **kwargs)
    celery.Task = ContextTask
    return celery
celery = make_celery(flask_app)


def as_list(fun):
    def wrap(*args, **kwargs):
        return map(lambda u: u._data, fun(*args, **kwargs))
    wrap.__name__ = fun.__name__
    return wrap


@celery.task
@as_list
def get_projects_list():
    return Project.objects


@celery.task
@as_list
def get_users_list():
    return User.objects


@celery.task
@as_list
def get_tasks_for_project(project_id):
    return Task.objects.filter(project=project_id)


@celery.task
@as_list
def get_actions_list(from_time, to_time):
    return Action.objects.filter(created__gte=from_time,
                                 created__lte=to_time)


@celery.task
@as_list
def get_actions_for_task(task_id, from_time, to_time):
    return Action.objects.filter(task=task_id,
                                 created__gte=from_time,
                                 created__lte=to_time)


@celery.task
@as_list
def get_actions_for_project(project_id, from_time, to_time):
    return Action.objects.filter(
        task__in=Task.objects.filter(project=project_id),
        created__gte=from_time,
        created__lte=to_time)
