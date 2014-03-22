from celery import Celery
from app import app as flask_app
from app import Project


def make_celery(app):
    celery = Celery(app.import_name, backend='amqp', broker=app.config['CELERY_BROKER_URL'])
    celery.conf.update(app.config)
    celery.conf.update(
        CELERY_TASK_SERIALIZER='json',
        CELERY_ACCEPT_CONTENT=['json'],  # Ignore other content
        CELERY_RESULT_SERIALIZER='mongo_json',
        )
    TaskBase = celery.Task
    class ContextTask(TaskBase):
        abstract = True
        def __call__(self, *args, **kwargs):
            with app.app_context():
                return TaskBase.__call__(self, *args, **kwargs)
    celery.Task = ContextTask
    return celery
celery = make_celery(flask_app)

@celery.task
def get_projects_list():
    return map(lambda u: u._data, Project.objects)
