import json
import datetime
import decimal
import redis

redis_client = redis.StrictRedis(host='localhost', port=6379, db=0)


class TimezoneMongoEncoder(json.JSONEncoder):
    def default(self, value, **kwargs):
        from timetracker.models import User, Project, Task, Action, Screenshot
        from bson.dbref import DBRef
        from bson.objectid import ObjectId

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
        if isinstance(value, (User, Project, Task, Action, Screenshot, )):
            return str(value.id)
        return super(TimezoneMongoEncoder, self).default(value, **kwargs)


def mongo_encoder(data):
    return json.dumps(data, cls=TimezoneMongoEncoder)
