#!/bin/bash
cd timetracker_erlang
erl -pa ebin deps/*/ebin -s asteroid -config asteroid -detached
cd ../timetracker_flask
python runserver.py&
celery -A timetracker.tasks worker -l info --autoreload
killall -9 beam.smp
killall python
