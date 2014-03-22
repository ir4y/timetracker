Time tracker software
=====================

Project consists of:

1. Client app
2. Backend server
3. Web frontend


For 'Client app' we are using qt5.  
It has intefaces for creating tasks, changing user activity, sending screenshot to server.
Communication with server based on REST interface.

For 'Backend server' we are use MSA.  
First part is flask rest interface for client app communication.  
Second part is admin web interface builded with flask-admin.  
Third part is celery worker, it used for web frontend rpc handler. Celery tasks call trought RabbitMQ.  
Fourth part is erlang WebSocket to RabbitMQ RPC proxy. Web frondend call tasks through WebSocket. Erlang converts calls from WebSocket to celery task format and send it to RabbitMq.  
Fifth part is database. We are using mongodb.  

For 'Web frontend' we are using AngularJS.  
It is singla page application using WebSocket RPC for communication with server.
