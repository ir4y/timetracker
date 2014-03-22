#!/bin/bash
cd timetracker_erlang
erl -pa ebin deps/*/ebin -s asteroid -config asteroid -detached
cd ../timetracker_flask
python app.py&
celery -A tasks worker -l info
killall -9 beam.smp
killall python
