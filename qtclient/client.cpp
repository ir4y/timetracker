#include "client.h"

Client::Client(QString server_address, QString _name, QString _password, QObject *parent) :
    QObject(parent)
{
    this->manager = new QNetworkAccessManager(this);
    this->server_address = server_address;
    this->name = _name;
    this->password = _password;

    this->authenticate();

    if (this->is_authenticated())
    {
        this->active = true;
        this->timer = new QTimer(this);
        connect(this->timer, SIGNAL(timeout()),
                this, SLOT(timer_slot()));
        this->timer->start(PERIODIC_TIME_DEFAULT);

        this->get_projects();
        this->get_tasks();

        this->current_project = this->tasks_to_projects[this->current_task];
        if (this->current_task.length() > 0)
            this->send_action("resume");
    }
}

void Client::timer_slot()
{
    this->timer->stop();
    this->sendScreen();
    this->timer->start(PERIODIC_TIME_DEFAULT);
}


void Client::get_tasks()
{
    QString url = this->server_address + "/api/task/";
    QByteArray response = this->request_get(url);

    QJsonDocument document = QJsonDocument::fromJson(response);
    QJsonObject object = document.object();
    QJsonArray data = object["data"].toArray();

    foreach (QJsonValue field, data)
    {
        QJsonObject obj = field.toObject();
        this->tasks[obj["id"].toString()] = obj["title"].toString();
        this->tasks_to_projects[obj["id"].toString()] = obj["project"].toString();
    }
}

void Client::get_projects()
{
    QString url = this->server_address + "/api/project/";
    QByteArray response = this->request_get(url);

    QJsonDocument document = QJsonDocument::fromJson(response);
    QJsonObject object = document.object();
    QJsonArray data = object["data"].toArray();

    foreach (QJsonValue field, data)
    {
        QJsonObject obj = field.toObject();
        this->projects[obj["id"].toString()] = obj["title"].toString();
    }
}

QByteArray Client::request_get(QString url)
{
    QNetworkRequest request(url);
    request.setRawHeader("AUTHORIZATION", this->session_id.toUtf8());
    QNetworkReply* reply = this->manager->get(request);
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QByteArray data = reply->readAll();
    return data;
}

QByteArray Client::request_post(QString url, QByteArray &data, QString content_type_header)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, content_type_header);
    if (this->session_id.length() > 0)
        request.setRawHeader("AUTHORIZATION", this->session_id.toUtf8());
    QNetworkReply* reply = this->manager->post(request, data);
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QByteArray result = reply->readAll();
    return result;

}

// first, change status
void Client::change_status(QString action)
{
    QJsonObject document;
    document.insert("task", QJsonValue::fromVariant(this->current_task));
    document.insert("user", QJsonValue::fromVariant(this->session_id));
    document.insert("action", QJsonValue::fromVariant(action));
    QByteArray data = QJsonDocument(document).toJson();
    QString url = this->server_address + "/api/action/";

    this->request_post(url, data);
}

// second, send photo
void Client::sendScreen()
{
    if (this->current_task.length() == 0)
        return;

    QPixmap image = QPixmap::grabWindow(QApplication::desktop()->winId());

    QByteArray buffer_data;
    QBuffer buffer(&buffer_data);
    buffer.open(QIODevice::ReadWrite);
    image.save(&buffer, "PNG");

    QString url = this->server_address + "/api/upload/";
    QString image_url = this->request_post(url, buffer_data, "image/png");

    qDebug() << image_url;

    QJsonObject document;
    document.insert("task", QJsonValue::fromVariant(this->current_task));
    document.insert("user", QJsonValue::fromVariant(this->session_id));
    document.insert("image", QJsonValue::fromVariant(image_url));
    document.insert("action", QJsonValue::fromVariant("screenshot"));
    QByteArray data = QJsonDocument(document).toJson();
    url = this->server_address + "/api/action/";

    this->request_post(url, data);
}


void Client::updateTask(QString project, QString task)
{
    QString project_id = this->get_project_id_by_name(project);
    QString task_id = this->get_task_id_by_name(task, project_id);

    this->current_project = project_id;

    if (this->current_task != task_id)
    {   // task was changed
        // send suspend
        this->send_action("suspend");
        this->current_task = task_id;
        this->send_action("resume");
    }

    this->current_task = task_id;
}


QString Client::get_project_id_by_name(QString name)
{
    if (this->projects[this->current_project] == name)
        return this->current_project;

    foreach (QString id, this->projects.keys())
        if (this->projects[id] == name)
            return id;

    return "";
}


QString Client::get_task_id_by_name(QString task_name, QString project_id)
{
    if (this->tasks[this->current_task] == task_name)
        return this->current_task;

    QStringList results;
    foreach (QString id, this->tasks.keys())
        if (this->tasks[id] == task_name)
            results.append(id);

    foreach (QString id, results)
        if (this->tasks_to_projects[id] == project_id)
            return id;

    // okay, we need to create a new task
    QJsonObject send;
    send.insert("project", QJsonValue::fromVariant(project_id));
    send.insert("title", QJsonValue::fromVariant(task_name));
    QByteArray data = QJsonDocument(send).toJson();
    QString url = this->server_address + "/api/task/";
    QByteArray response = this->request_post(url, data);

    QJsonDocument document = QJsonDocument::fromJson(response);

    QString result = document.object()["id"].toString();
    this->tasks[result] = task_name;
    this->tasks_to_projects[result] = project_id;
    return result;
}

void Client::authenticate()
{
    QJsonObject send;
    send.insert("name", QJsonValue::fromVariant(this->name));
    send.insert("password", QJsonValue::fromVariant(this->password));
    QByteArray data = QJsonDocument(send).toJson();
    QString url = this->server_address + "/api/login/";
    QByteArray response = this->request_post(url, data);

    QJsonDocument document = QJsonDocument::fromJson(response);
    this->session_id = document.object()["user_id"].toString();
    this->current_task = document.object()["last_task_id"].toString();
    qDebug() << this->current_task;
}

bool Client::is_authenticated()
{
    return this->session_id.length() > 0;
}

bool Client::is_active()
{
    return this->active;
}

void Client::suspend()
{
    QString next_state;
    if (this->active)
    {
        next_state = "suspend";
        this->active = false;
        this->timer_remaining_time = this->timer->remainingTime();
        this->timer->stop();
    }
    else
    {
        next_state = "resume";
        this->active = true;
        this->timer->start(this->timer_remaining_time);
    }

    this->send_action(next_state);
}

void Client::send_action(QString action)
{
    QJsonObject document;
    document.insert("task", QJsonValue::fromVariant(this->current_task));
    document.insert("user", QJsonValue::fromVariant(this->session_id));
    document.insert("action", QJsonValue::fromVariant(action));
    QByteArray data = QJsonDocument(document).toJson();
    QString url = this->server_address + "/api/action/";

    this->request_post(url, data);
}

Client* client = NULL;
